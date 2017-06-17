/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PREFIX_HPP
#define HL_PREFIX_HPP

/********************  HEADERS  *********************/
//std
#include <base/Helper.hpp>
#include <json/json.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
struct PrefixConfig
{
    StringList inherit;
    StringMapList flags;
    StringList override;
    JsonMap packageOverride;
    StringMap versions;
    StringMapList use;
    StringMap modules;
    Json::Value gentoo;
    Json::Value debian;
    StringList providers;
    bool useGnuStow;
};

/*********************  CLASS  **********************/
class Prefix
{
    public:
        Prefix(const std::string & prefix);
    private:
        void loadConfig(void);
    private:
        std::string prefix;
        PrefixConfig config;
};

}

#endif //HL_PREFIX_HPP
