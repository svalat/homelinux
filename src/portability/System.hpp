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
};

}

#endif //HL_SYSTEM_HPP
