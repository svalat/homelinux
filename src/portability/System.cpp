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
#include <base/Helper.hpp>
#include <json/json.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include "System.hpp"

/********************  MACROS  **********************/
#define HAVE_CURL
#define HAVE_WGET

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
void System::writeFile(const std::string & content,const std::string & path)
{
	//check
	assume(path.empty() == false,"Invalid empty path !");
	
	//open
	FILE * fp = fopen(path.c_str(),"w");
	assumeArg(fp != NULL,"Fail to open file %1 : %2").arg(path).argStrErrno().end();
	
	//read
	size_t s = fwrite(content.c_str(),1,content.size(),fp);
	assumeArg(s == content.size(),"Failed to read all file : %1").argStrErrno().end();
	
	//close
	fclose(fp);
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
	
	//debug
	HL_DEBUG_ARG("System","Load JSON file %1").arg(path).end();
	
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
	HL_DEBUG_ARG("System","Run command : %1").arg(cmd).end();
	return system(cmd.c_str());
}

/*******************  FUNCTION  *********************/
void System::removeFile(const std::string & path)
{
	if (unlink(path.c_str()) != 0)
		HL_ERROR_ARG("Fail to remove file %1 : %2").arg(path).argStrErrno().end();
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
			//stats
			struct stat s;
			std::string full = path+std::string("/")+d->d_name;
			assumeArg(stat(full.c_str(),&s) == 0,"Fail to stat path %1 : %2").arg(full).argStrErrno().end();

			//apply
			if (S_ISDIR(s.st_mode) && strcmp(d->d_name,".") != 0 &&  strcmp(d->d_name,"..") != 0 ) 
			{
				//compute sub
				std::string sub = subdir;
				if (sub.empty() == false)
					sub += "/";
				sub += d->d_name;
				
				//recurse
				findFiles(full,callback,sub);
			} else if (S_ISREG(s.st_mode)) {
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

/*******************  FUNCTION  *********************/
bool System::hasCommand(const std::string & cmd)
{
	return system((cmd + "> /dev/null 2>/dev/null").c_str()) == 0;
}

/*******************  FUNCTION  *********************/
bool System::downloadJson(Json::Value & out,const std::string & url)
{
	static int id;
	
	//gen filename
	char buffer[128];
	sprintf(buffer,"/tmp/hl-internal-download-%d.json",++id);
	//std::string tmp = mktemp(buffer);
	std::string tmp = buffer;
	std::string cmd;

	//debug
	HL_DEBUG_ARG("System","Download JSON : %1").arg(url).end();
	
	//download
	#ifdef HAVE_WGET
		cmd = "wget "+url+" -O "+tmp+" > /dev/null 2>/dev/null";
	#elif defined(HAVE_CURL)
		cmd = "curl "+url+" -o "+tmp+" > /dev/null 2>/dev/null";
	#else
		HL_FATAL("Cannot download file without curl or wget on system !");
	#endif
	
	//debug
	HL_DEBUG_ARG("System","Downloading using command %1").arg(cmd).end();
	
	//run
	int status = system(cmd.c_str());
	
	//load
	if (status == 0)
	{
		loadJson(out,tmp);
		//delete
		unlink(tmp.c_str());
	}
	
	return status == 0;
}

/*******************  FUNCTION  *********************/
bool System::runAndRead(std::string & out,const std::string & cmd)
{
	static int id;
	
	//gen filename
	char buffer[128];
	sprintf(buffer,"/tmp/hl-internal-run-and-read-%d.out",id++);
	//std::string tmp = mktemp(buffer);
	std::string tmp = buffer;
	
	//run
	int status = system((cmd + " > "+tmp).c_str());
	
	//load
	if (status == 0)
	{
		out = loadFile(tmp);
		//delete
		unlink(tmp.c_str());
	}
	
	return status == 0;
}

/*******************  FUNCTION  *********************/
bool System::ftpListFiles(const std::string & url,std::function<void(const std::string &)> callback)
{
	//check
	#ifndef HAVE_WGET
		#error "Not support curl or other way to list files in FTP directory"
	#endif

	//check
	assume(url.empty() == false,"Get empty URL !");

	//vars
	static int id;
	
	//gen filename
	char buffer[128];
	sprintf(buffer,"/tmp/hl-internal-ftp-list-files-%d.json",id++);
	//std::string tmp = mktemp(buffer);
	std::string tmp = buffer;
	std::string cmd = "LC_ALL=C wget -O "+tmp+".html "+url+" > /dev/null 2> /dev/null; "
	                + "cat "+tmp+ ".html | grep File | cut -f 2 -d '>' | cut -f 1 -d '<' > "+ tmp;

	//run
	int status = system(cmd.c_str());
	if (status != 0)
	{
		HL_ERROR_ARG("Fail to fetch FTP files from  %1").arg(url).end();
		return false;
	}

	//parse
	std::string content = loadFile(tmp);
	Helper::split(content,'\n',[&callback](const std::string & value){
		callback(value);
	});

	//unlink
	unlink(tmp.c_str());
	unlink((tmp+".html").c_str());

	return true;
}

/*******************  FUNCTION  *********************/
int System::getCoreCount(void)
{
	std::string out;
	if (runAndRead(out,"cat /proc/cpuinfo | grep processor | wc -l") == 0)
		return atoi(out.c_str());
	else
		return 4;
}

}
