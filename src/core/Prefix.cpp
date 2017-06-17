/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cassert>
#include <portability/System.hpp>
#include "Prefix.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
Prefix::Prefix(const Config * config,const std::string & prefix, bool master)
{
    //checks
    assert(config != NULL);
	assume(prefix.empty() == false,"Invalid empty prefix");
    
    //setup
    this->config = config;
    this->prefix = prefix;
    this->master = false;
    
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
    Helper::jsonToObj(prefixConfig.inherit,json["inherit"]);
    Helper::jsonToObj(prefixConfig.flags,json["flags"]);
    Helper::jsonToObj(prefixConfig.override,json["override"]);
    Helper::jsonToObj(prefixConfig.versions,json["versions"]);
    Helper::jsonToObj(prefixConfig.use,json["user"]);
    Helper::jsonToObj(prefixConfig.modules,json["modules"]);
    Helper::jsonToObj(prefixConfig.packageOverride,json["packageOverride"]);
    prefixConfig.gentoo = json["gentoo"];
    prefixConfig.debian = json["debian"];
    Helper::jsonToObj(prefixConfig.providers,json["providers"]);
    prefixConfig.providers.push_front("models");
    prefixConfig.useGnuStow = json.get("useGnuStow",false).asBool();
}

/*******************  FUNCTION  *********************/
std::string Prefix::getFilePath(const std::string path) const
{
    return this->prefix + "/" + path;
}

}
