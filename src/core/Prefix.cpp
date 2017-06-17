/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <portability/System.hpp>
#include "Prefix.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
Prefix::Prefix(const std::string & prefix)
{
    //compute file path
	assume(prefix.empty() == false,"Invalid empty prefix");
    
    //setup
    this->prefix = prefix;
    
    //load config
    this->loadConfig();
}

/*******************  FUNCTION  *********************/
void Prefix::loadConfig(void)
{
    //compute path
    std::string path = prefix + "/homelinux.json";
    
    //load
    Json::Value json;
    System::loadJson(json,path);
    
    //apply
    Helper::jsonToObj(config.inherit,json["inherit"]);
    Helper::jsonToObj(config.flags,json["flags"]);
    Helper::jsonToObj(config.override,json["override"]);
    Helper::jsonToObj(config.versions,json["versions"]);
    Helper::jsonToObj(config.use,json["user"]);
    Helper::jsonToObj(config.modules,json["modules"]);
    Helper::jsonToObj(config.packageOverride,json["packageOverride"]);
    config.gentoo = json["gentoo"];
    config.debian = json["debian"];
    Helper::jsonToObj(config.providers,json["providers"]);
    config.useGnuStow = json.get("useGnuStow",false).asBool();
}

}
