/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_SYSTEM_HPP
#define HL_SYSTEM_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <functional>

/*******************  TYPES  ************************/
//to avoid to include json.hpp everywhere
namespace Json
{
    class Value;
}

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
struct System
{
	//env
	static std::string getHomeDir(void);
	static std::string getEnv(const std::string & name,const std::string & defaultValue = "");
	//json
	static void loadJson(Json::Value & out,const std::string & path);
	static void writeJson(const Json::Value & json,const std::string & path);
	//dirs & files
	static std::string loadFile(const std::string & path);
	static void writeFile(const std::string & content,const std::string & path);
	static bool fileExist(const std::string & path);
	static void readDir(const std::string & path,std::function<void(const std::string &)> callback);
	static void findFiles(const std::string & path,std::function<void(const std::string &)> callback,const std::string & subdir = "");
	static void removeFile(const std::string & path);
	//run
	static bool hasCommand(const std::string & cmd);
	static int runCommand(const std::string & cmd);
	static bool runAndRead(std::string & out,const std::string & cmd);
	//download
	static bool downloadJson(Json::Value & out,const std::string & url);
	static bool ftpListFiles(const std::string & url,std::function<void(const std::string &)> callback);
	//cpu
	static int getCoreCount(void);
};

}

#endif //HL_SYSTEM_HPP
