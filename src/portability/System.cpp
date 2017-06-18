/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fstream>
#include <base/Debug.hpp>
#include <json/json.h>
#include <dirent.h>
#include <cstring>
#include "System.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
std::string System::getHomeDir(void)
{
	return getenv("HOME");
}

/*******************  FUNCTION  *********************/
std::string System::getEnv(const std::string & name,const std::string & defaultValue)
{
	char * value = getenv(name.c_str());
	if (value == NULL)
		return defaultValue;
	else
		return value;
}

/*******************  FUNCTION  *********************/
bool System::fileExist(const std::string & path)
{
	return ( access( path.c_str(), F_OK ) != -1 );
}

/*******************  FUNCTION  *********************/
std::string System::loadFile(const std::string & path)
{
	//check
	assume(path.empty() == false,"Invalid empty path !");
	
	//open
	FILE * fp = fopen(path.c_str(),"r");
	assumeArg(fp != NULL,"Fail to open file %1 : %2").arg(path).argStrErrno().end();
	
	//move to end
	int status = fseek(fp,0,SEEK_END);
	assumeArg(status == 0,"Fail to seek to end : %1").argStrErrno().end();
	
	long size = ftell(fp);
	assumeArg(size >= 0,"Fail to get size via ftell : %1").argStrErrno().end();
	
	//move to start
	status = fseek(fp,0,SEEK_SET);
	assumeArg(status == 0,"Fail to seek to start : %1").argStrErrno().end();
	
	//allocate
	char * buffer = new char[size+1];
	
	//read
	size_t s = fread(buffer,1,size,fp);
	assumeArg(s == size,"Failed to read all file : %1").argStrErrno().end();
	buffer[s] = '\0';
	
	//close
	fclose(fp);

	//convert
	std::string res = buffer;
	delete[] buffer;
	
	//return
	return res;
}

/*******************  FUNCTION  *********************/
void System::loadJson(Json::Value & out,const std::string & path)
{
	//compute file path
	assume(path.empty() == false,"Invalid empty path");
	
	//check if exist
	assumeArg(System::fileExist(path),"File %1 does not exist !").arg(path).end();

	//open file
	std::ifstream file(path.c_str(),std::ifstream::binary);
	assumeArg(file.is_open(),"Fail to open configuration file : %1 : %2").arg(path).argStrErrno().end();
	
	//parse
	Json::Reader reader;
	bool status = reader.parse(file, out);
	assumeArg(status,"Fail to load configuration file '%1' : \n%2")
		.arg(path)
		.arg(reader.getFormattedErrorMessages())
		.end();
}

/*******************  FUNCTION  *********************/
void System::writeJson(const Json::Value & json,const std::string & path)
{
	//compute file path
	assume(path.empty() == false,"Invalid empty path");
	assume(json.isNull() == false,"Invalid NULL json");
	
	//open file
	std::ofstream file(path.c_str(),std::ifstream::binary);
	assumeArg(file.is_open(),"Fail to open configuration file : %1 : %2").arg(path).argStrErrno().end();
	
	//dump
	file << json;
	
	//close
	file.close();
}

/*******************  FUNCTION  *********************/
void System::readDir(const std::string & path,std::function<void(const std::string &)> callback)
{
	//open
	DIR * dir = opendir(path.c_str());
	assumeArg(dir != NULL,"Fail to scan directory : %1. %2").arg(path).argStrErrno().end();
	
	//loop
	for(;;)
	{
		dirent * d = readdir(dir);
		if (d == NULL)
			break;
		else
			callback(d->d_name);
	}
	
	//close
	closedir(dir);
}

/*******************  FUNCTION  *********************/
int System::runCommand(const std::string & cmd)
{
	return system(cmd.c_str());
}

/*******************  FUNCTION  *********************/
void System::findFiles(const std::string & path,std::function<void(const std::string &)> callback,const std::string & subdir)
{
	//open
	DIR * dir = opendir(path.c_str());
	assumeArg(dir != NULL,"Fail to scan directory : %1. %2").arg(path).argStrErrno().end();
	
	//loop
	for(;;)
	{
		dirent * d = readdir(dir);
		if (d == NULL)
		{
			break;
		} else {
			if (d->d_type == DT_DIR && strcmp(d->d_name,".") != 0 &&  strcmp(d->d_name,"..") != 0 ) 
			{
				//compute sub
				std::string sub = subdir;
				if (sub.empty() == false)
					sub += "/";
				sub += d->d_name;
				
				//recurse
				findFiles(path+std::string("/")+d->d_name,callback,sub);
			} else if (d->d_type == DT_REG) {
				if (subdir.empty())
					callback(d->d_name);
				else
					callback(subdir + std::string("/") + d->d_name);
			}
		}
	}
	
	//close
	closedir(dir);
}

}
