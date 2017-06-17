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

}
