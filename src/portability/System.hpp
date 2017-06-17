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
	static std::string getHomeDir(void);
	static bool fileExist(const std::string & path);
	static std::string getEnv(const std::string & name,const std::string & defaultValue = "");
	static std::string loadFile(const std::string & path);
	static void loadJson(Json::Value & out,const std::string & path);
	static void readDir(const std::string & path,std::function<void(const std::string &)> callback);
	static int runCommand(const std::string & cmd);
};

}

#endif //HL_SYSTEM_HPP
