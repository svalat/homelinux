/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_OPTIONS_HPP
#define HL_OPTIONS_HPP

/********************  HEADERS  *********************/
//must be as first
#include "config.h"
//std
#include <string>
#include <ctime>
#include <vector>
//from jsoncpp
#include <json/json.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************* TYPES ************************/
typedef std::vector<std::string> ModuleList;

/********************  STRUCT  **********************/
/**
 * Structure to store the configuration of HLPIPE.
**/
struct Config
{
    Config(void);
    ~Config(void);
    void loadDefault(void);
    void load(std::string path = "");
    void load(Json::Value & config);
    void parseArgs(int argc, const char ** argv);
    //vars
    bool showDebugCat;
    std::string prefix;
    std::string host;
    ModuleList modules;
    bool ccache;
    bool pyEnv;
    bool homecache;
    Json::Value packageOverride;
};

}

#endif //HL_OPTIONS_HPP
