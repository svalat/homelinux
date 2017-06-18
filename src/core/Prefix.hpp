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
#include <base/Config.hpp>
#include <json/json.h>
#include "QuickPackage.hpp"
#include "PackageDef.hpp"

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
		Prefix(const Config *config, const std::string & prefix, bool master = false);
		std::string getFilePath(const std::string path) const;
		void loadPackage(PackageDef & out,const std::string packageName);
		void getQuickPackage(PackageDef & out,const std::string & packageName);
	private:
		void loadConfig(void);
		QuickPackage & getQuickPackage(void);
	private:
		const Config * config;
		QuickPackage * quickPackage;
		std::string prefix;
		PrefixConfig prefixConfig;
		bool master;
};

}

#endif //HL_PREFIX_HPP
