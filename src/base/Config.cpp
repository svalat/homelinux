/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>
//unix
#include <unistd.h>
#include <getopt.h>
//internals
#include "Debug.hpp"
#include <portability/System.hpp>
//current
#include "Config.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
Config::Config(void)
{
    this->showDebugCat = false;
    this->loadDefault();
    this->load();
}

/*******************  FUNCTION  *********************/
Config::~Config(void)
{
    if (showDebugCat)
        Debug::showList();
}

/*******************  FUNCTION  *********************/
void Config::loadDefault()
{
    this->prefix = System::getHomeDir()+"/usr";
    this->host = "default";
    this->ccache = false;
    this->pyEnv = true;
    this->homecache = false;
}

/*******************  FUNCTION  *********************/
void Config::load(Json::Value & config)
{
    //basics
    this->prefix = config.get("prefix",this->prefix).asString();
    this->host = config.get("host",this->host).asString();
    this->homecache = config.get("homecache",this->homecache).asBool();
    this->ccache = config.get("ccache",this->ccache).asBool();
    this->pyEnv = config.get("pyEnv",this->pyEnv).asBool();
    
    //modules
    Json::Value mods = config["modules"];
    if (mods.isArray()) {
        for (auto & m : mods)
            this->modules.push_back(m.asString());
    }
    
    //override
    this->packageOverride = config["packagesOverride"];
}

/*******************  FUNCTION  *********************/
void Config::load(std::string path)
{
    //compute file path
    if (path.empty())
        path = System::getHomeDir()+"/.homelinux.json";
    
    //check if exist
    if (System::fileExist(path) == false)
    {
        HL_WARNING_ARG("Caution, %1 does not exist, using default config !").arg(path).end();
        return;
    }
    
    //open file
    std::ifstream file(path.c_str(),std::ifstream::binary);
    assumeArg(file.is_open(),"Fail to open configuration file : %1 : %2").arg(path).argStrErrno().end();
    
    //load
    Json::Value tmpObj;
    
    //parse
    Json::Reader reader;
    bool status = reader.parse(file, tmpObj);
    assumeArg(status,"Fail to load configuration file '%1' : \n%2")
        .arg(path)
        .arg(reader.getFormattedErrorMessages())
        .end();

    //apply
    this->load(tmpObj);
}

/*******************  FUNCTION  *********************/
void Config::parseArgs(int argc, const char ** argv)
{
    for (int i = 1 ; i < argc -1 ; i++)
    {
        if (strcmp(argv[i],"-v")==0)
        {
            if (strcmp(argv[i+1],"help"))
                this->showDebugCat = true;
            else
                Debug::setVerbosity(argv[i+1]);
        } 
    }
}

}
