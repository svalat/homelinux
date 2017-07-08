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
#include "EnvSetup.hpp"
#include "QuickPackage.hpp"
#include "PackageDef.hpp"
#include <providers/Provider.hpp>

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

/*********************  TYPES  **********************/
typedef std::map<std::string,Provider*> ProviderMap;
typedef std::list<Prefix*> PrefixList;

/*********************  CLASS  **********************/
class Prefix
{
	public:
		Prefix(const Config *config, const std::string & prefix, bool master = false);
		~Prefix(void);
		std::string getFilePath(const std::string path) const;
		void loadPackage(PackageDef & out,const std::string & packageName);
		void getQuickPackage(PackageDef & out,const std::string & packageName);
		PrefixConfig & getConfig(void);
		const Config & getUserConfig(void) const;
		void updateDb(void);
		void updateCache(void);
		void fillEnv(EnvSetup & env);
		std::string listInstalled(void);
		bool isInstalled(const std::string & value);
		std::string prefixOf(const std::string & packageName);
		bool isInstalled(const PackageDef & pack);
		void ls(std::ostream & out = std::cout);
		const std::string & getPrefix(void) const;
		void setPrefixForTests(const std::string & prefix);
		void search(std::ostream & out,const std::string & value);
		void exportConfig(std::ostream & out = std::cout);
		void validate(void);
		void crawl(void);
	private:
		bool isLocalyInstalled(const PackageDef & pack);
		bool loadPackageNoInherit(PackageDef & out,const std::string & packageName);
		Provider & getProvider(const std::string & name);
		void loadConfig(void);
		QuickPackage & getQuickPackage(void);
	private:
		const Config * config;
		QuickPackage * quickPackage;
		std::string prefix;
		PrefixConfig prefixConfig;
		bool master;
		ProviderMap providers;
		PrefixList inheritedPrefix;
};

}

#endif //HL_PREFIX_HPP
