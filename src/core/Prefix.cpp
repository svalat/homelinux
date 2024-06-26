/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cassert>
#include <cstdio>
#include <portability/System.hpp>
#include <providers/ProviderGentoo.hpp>
#include <providers/ProviderDebian.hpp>
#include <providers/ProviderUrls.hpp>
#include <providers/ProviderModels.hpp>
#include <providers/ProviderHomelinux.hpp>
#include <providers/ProviderGithub.hpp>
#include <base/Colors.hpp>
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
	this->quickPackage = NULL;
	
	//load config
	this->loadConfig();
}

/*******************  FUNCTION  *********************/
Prefix::~Prefix(void)
{
	//provider
	//for (auto & it : providers)
	forEach(ProviderMap,it,providers)
		delete it->second;
	providers.clear();

	//inherited prefix
	//for (auto & it :inheritedPrefix)
	forEach(PrefixList,it,inheritedPrefix)
		delete *it;
	inheritedPrefix.clear();
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
	Helper::jsonToObj(prefixConfig.use,json["use"]);
	Helper::jsonToObj(prefixConfig.modules,json["modules"]);
	Helper::jsonToObj(prefixConfig.packageOverride,json["packageOverride"]);
	prefixConfig.gentoo = json["gentoo"];
	prefixConfig.debian = json["debian"];
	Helper::jsonToObj(prefixConfig.providers,json["providers"]);
	Helper::jsonToObj(prefixConfig.mirrors,json["mirrors"]);
	prefixConfig.providers.push_front("models");
	prefixConfig.useGnuStow = json.get("useGnuStow",false).asBool();

	//load inherited prefix
	//for (auto p : prefixConfig.inherit)
	forEach(StringList,p,prefixConfig.inherit)
		inheritedPrefix.push_back(new Prefix(config,*p,false));
}

/*******************  FUNCTION  *********************/
std::string Prefix::getFilePath(const std::string path) const
{
	return this->prefix + "/" + path;
}

/*******************  FUNCTION  *********************/
void Prefix::loadPackage(PackageDef & out,const std::string & packageName)
{
	//load package
	PackageDef pack;
	if (loadPackageNoInherit(pack,packageName) == false)
		HL_FATAL_ARG("Fail to load package %1. Not found in all DB.").arg(packageName).end();

	//load inherit
	if (pack.inherit.empty() == false)
	{
		PackageDef inherit;
		loadPackage(inherit,pack.inherit);
		out.merge(inherit);
	} else {
		//apply makeopts -j
		char tmp[32];
		int makeJ = config->makeJ == 0 ? System::getCoreCount() : config->makeJ;
		sprintf(tmp,"-j%d",makeJ);
		out.flags["MAKEOPTS"].push_back(tmp);

		//apply system cflags on root
		Helper::merge(out.flags,prefixConfig.flags,false);
		
		//apply prefix -I/-L
		//out.flags["CFLAGS"].push_back("-I"+getFilePath("/include"));
		//out.flags["LDFLAGS"].push_back("-L"+getFilePath("/lib"));
	}

	//apply pack
	out.merge(pack);

	//apply quickpackahe
	PackageDef quickPack;
	getQuickPackage().genPackage(quickPack,pack.name);
	if (out.subdir.empty() == false)
		quickPack.subdir = "";
	out.merge(quickPack);

	//apply override from prefix config
	JsonMap::iterator it = prefixConfig.packageOverride.find(pack.name);
	if (it != prefixConfig.packageOverride.end())
	{
		PackageDef prefixPack;
		prefixPack.loadJson(it->second);
		out.merge(prefixPack);
	}

	//apply use flags "" from prefix
	//for (auto &it : prefixConfig.use[""])
	forEach(StringList,it,prefixConfig.use[""])
		out.use.merge(*it,true);

	//apply use flags "all" from prefix
	UseFlags toMerge;
	//for (auto &it : prefixConfig.use["all"])
	forEach(StringList,it,prefixConfig.use["all"])
		toMerge.merge(*it);

	//apply use flags for packagName from prefix
	//for (auto &it : prefixConfig.use[out.name])
	forEach(StringList,it,prefixConfig.use[out.name])
		toMerge.merge(*it);

	//we need to force status, auto is considered as enabled
	toMerge.setAuto(FLAG_ENABLED);
	
	//we can merge
	out.use.merge(toMerge,true);

	//apply override from user config
	const Json::Value & node = config->packageOverride[pack.name];
	if (node.isObject())
	{
		PackageDef userPack;
		userPack.loadJson(node);
		out.merge(userPack);
	}

	//apply mirrors
	applyMirrors(out.urls);
	applyMirrors(out.vfetcher["urls"]);
	if (out.vfetcher["subdir"].isObject())
		applyMirrors(out.vfetcher["subdir"]["urls"]);
}

/*******************  FUNCTION  *********************/
std::string Prefix::applyMirrors(const std::string & url)
{
	//search matching
	forEach(StringMap,it,prefixConfig.mirrors)
	{
		//what to match
		std::string mirrorName = "mirror://"+it->first+"/";

		//ok match, so replace
		if (Helper::startBy(url,mirrorName))
		{
			std::string tmp = url;
			Helper::replaceInPlace(tmp,mirrorName,it->second+"/");
			return tmp;
		}
	}

	//check if contain mirror
	if (Helper::startBy(url,"mirror://"))
		HL_FATAL_ARG("Failre to find mirror to build URL : %1, please check your prefix config file (PREFIX/homelinux.json)").arg(url).end();

	//default
	return url;
}

/*******************  FUNCTION  *********************/
void Prefix::applyMirrors(Json::Value &urls)
{
	//nothing to do
	if (urls.isArray() == false)
	{
		//loop
		for (int i = 0 ; i < urls.size() ; i++)
			urls[i] = applyMirrors(urls[i].asString());
	} else if (urls.isString()) {
		urls = applyMirrors(urls.asString());
	}
}

/*******************  FUNCTION  *********************/
void Prefix::applyMirrors(StringList &urls)
{
	forEach(StringList,it,urls)
		*it = applyMirrors(*it);
}

/*******************  FUNCTION  *********************/
bool Prefix::loadPackageNoInherit(PackageDef & out,const std::string & packageName)
{
	//get package
	//for (auto & prov : prefixConfig.providers)
	forEach(StringList,prov,prefixConfig.providers)
		if (getProvider(*prov).getPackage(out,packageName))
			return true;
	return false;
}

/*******************  FUNCTION  *********************/
void Prefix::getQuickPackage(PackageDef & out,const std::string & packageName)
{
	getQuickPackage().genPackage(out,packageName);
}

/*******************  FUNCTION  *********************/
PrefixConfig & Prefix::getConfig(void)
{
	return this->prefixConfig;
}

/*******************  FUNCTION  *********************/
QuickPackage & Prefix::getQuickPackage(void)
{
	if (quickPackage == NULL)
		quickPackage = new QuickPackage(this);
	return *quickPackage;
}

/*******************  FUNCTION  *********************/
Provider & Prefix::getProvider(const std::string & name)
{
	//search
	ProviderMap::iterator it = providers.find(name);
	
	//ok
	if (it != providers.end())
		return *(it->second);
	
	//need to allocate
	Provider * ret = NULL;
	if (name == "gentoo")
		ret = providers["gentoo"] = new ProviderGentoo(this);
	else if (name == "debian")
		ret = providers["debian"] = new ProviderDebian(this);
	else if (name == "models")
		ret = providers["models"] = new ProviderModels(this);
	else if (name == "urls")
		ret = providers["urls"] = new ProviderUrls(this);
	else if (name == "homelinux")
		ret = providers["homelinux"] = new ProviderHomelinux(this);
	else if (name == "github")
		ret = providers["github"] = new ProviderGithub(this);
	else
		HL_FATAL_ARG("Invalid package provider : %1").arg(name).end();

	//ok
	return *ret;
}

/*******************  FUNCTION  *********************/
bool Prefix::isInstalled(const std::string & value)
{
	//get package
	PackageDef pack;
	loadPackage(pack,value);
	if (pack.name == "")
		return false;
	return isInstalled(pack);
}

/*******************  FUNCTION  *********************/
std::string Prefix::prefixOf(const std::string & packageName)
{
	//get package
	PackageDef pack;
	loadPackage(pack,packageName);
	
	//check local
	std::string pref;
	if (isLocalyInstalled(pack))
		pref = this->getPrefix();

	//check inherit
	//for (auto & prefix : inheritedPrefix)
	forEach(PrefixList,prefix,inheritedPrefix)
		if ((*prefix)->isLocalyInstalled(pack))
			pref = (*prefix)->getPrefix();
	
	//not found
	if (pref.empty())
		return "";
	
	//ok
	return pack.getRealPrefix(pref,prefixConfig.useGnuStow);
}

/*******************  FUNCTION  *********************/
bool Prefix::isInstalled(const PackageDef & pack)
{
	//check local
	if (isLocalyInstalled(pack))
		return true;

	//check inherit
	//for (auto & prefix : inheritedPrefix)
	forEach(PrefixList,prefix,inheritedPrefix)
		if ((*prefix)->isLocalyInstalled(pack))
			return true;
	
	//not found
	return false;
}

/*******************  FUNCTION  *********************/
//@TODO : check version
bool Prefix::isLocalyInstalled(const PackageDef & pack)
{
	//build path
	std::string path = pack.getPackInstalled(getFilePath(""));

	//check exist
	return System::fileExist(path);
}

/*******************  FUNCTION  *********************/
void Prefix::crawl(void)
{
	updateDb();
	Provider & p = getProvider("homelinux");
	HL_MESSAGE_ARG("Update DB of provider %1").arg("homelinux").end();
	p.updateDb();
}

/*******************  FUNCTION  *********************/
//@TODO make parallel
void Prefix::updateDb(void)
{
	//need to push gentoo first as some hl packages revers to gentoo versions
	bool present = false;
	for (StringList::iterator it = prefixConfig.providers.begin() ; it != prefixConfig.providers.end() ; ++it)
	{
		if (*it == "gentoo")
		{
			it = prefixConfig.providers.erase(it);
			present = true;
		}
	}
	if (present)
		prefixConfig.providers.push_front("gentoo");

	//for (auto prov : prefixConfig.providers)
	forEach(StringList,prov,prefixConfig.providers)
	{
		if (*prov != "homelinux")
		{
			Provider & p = getProvider(*prov);
			HL_MESSAGE_ARG("Update DB of provider %1").arg(*prov).end();
			p.updateDb();
		}
	}
}

/*******************  FUNCTION  *********************/
void Prefix::exportConfig(std::ostream & out)
{
	//build json
	Json::Value final;
	
	//load config file
	System::loadJson(final["prefixConfig"],this->getFilePath("/homelinux.json"));
	
	//export packages
	Json::Value & packs = final["installed"];
	StringList lst = System::readDir(getFilePath("/homelinux/install-db/"));
	forEach(StringList,file,lst)
	{
		if (Helper::endBy(*file,".json"))
		{
			Json::Value json;
			System::loadJson(json,getFilePath("/homelinux/install-db/"+*file));
			std::string name = json.get("name","").asString();
			std::string version = json.get("version","unknown").asString();
			packs[name] = version;
		}
	}
	
	//out
	out << final;
}

/*******************  FUNCTION  *********************/
void Prefix::validate(void)
{
	//vars
	std::string host = config->host;
	std::string path = getFilePath("homelinux/packages/validated/"+host+".json");
	Json::Value db;

	//load
	if (System::fileExist(path))
		System::loadJson(db,path);
	
	//apply current
	StringList lst = System::readDir(getFilePath("/homelinux/install-db/"));
	forEach(StringList,file,lst)
	{
		if (Helper::endBy(*file,".json"))
		{
			//load
			Json::Value json;
			System::loadJson(json,getFilePath("/homelinux/install-db/"+*file));
			std::string version = json.get("version","unknown").asString();
			
			//merge
			std::string name = json.get("name","unknwon").asString();
			std::string validated = db.get(name,"unknown").asString();
			if (validated == "unknown" || VersionMatcher::compareVersion(version,validated) > 1)
				db[name] = version;
		}
	}

	//save
	System::writeJson(db,path);
}

/*******************  FUNCTION  *********************/
void Prefix::ls(std::ostream & out)
{
	//ls childs
	//for (auto prefix : inheritedPrefix)
	forEach(PrefixList,prefix,inheritedPrefix)
		(*prefix)->ls(out);

	//for all files
	out << Colors::yellow("================ "+this->prefix+" =================") << std::endl;
	StringList lst = System::readDir(getFilePath("/homelinux/install-db/"));
	forEach(StringList,file,lst)
	{
		if (Helper::endBy(*file,".json"))
		{
			Json::Value json;
			System::loadJson(json,getFilePath("/homelinux/install-db/"+*file));
			std::string name = json.get("name","").asString();
			std::string version = json.get("version","unknown").asString();
			out << Colors::green(name) << " " << Colors::cyan(version) << std::endl;
		}
	}
}

/*******************  FUNCTION  *********************/
void Prefix::search(std::ostream & out,const std::string & value)
{
	prefixConfig.providers.reverse();
	//for (auto prov : prefixConfig.providers)
	forEach(StringList,prov,prefixConfig.providers)
	{
		if( *prov != "models")
		{
			Provider & p = getProvider(*prov);
			out << Colors::yellow("================ "+*prov+" =================") << std::endl;
			out << p.search(value) << std::endl;
		}
	}
}

/*******************  FUNCTION  *********************/
//@TODO make parallel
void Prefix::updateCache(void)
{
	//for (auto prov : prefixConfig.providers)
	forEach(StringList,prov,prefixConfig.providers)
	{
		Provider & p = getProvider(*prov);
		HL_MESSAGE_ARG("Update cache of provider %1").arg(*prov).end();
		p.updateCache();
	}
}

/*******************  FUNCTION  *********************/
void Prefix::fillEnv(EnvSetup & env)
{
	forEach(PrefixList,it,inheritedPrefix)
		(*it)->fillEnv(env);
	env.addPrefix(this->prefix);
}

/*******************  FUNCTION  *********************/
const std::string & Prefix::getPrefix(void) const
{
	return prefix;
}

/*******************  FUNCTION  *********************/
const Config & Prefix::getUserConfig(void) const
{
	return *config;
}

/*******************  FUNCTION  *********************/
void Prefix::setPrefixForTests(const std::string & prefix)
{
	this->prefix = prefix;
}

/*******************  FUNCTION  *********************/
std::string Prefix::listInstalled(void)
{
	//vars
	std::string out;

	//loop on files
	StringList lst = System::readDir(getFilePath("/homelinux/install-db/"));
	forEach(StringList,file,lst)
	{
		PackageDef pack;
		pack.load(getFilePath("/homelinux/install-db/"+*file));
		out += Colors::green(pack.getSlotName())+" "+Colors::cyan(pack.getVersion());
		out += "\n";
	}

	//ok
	return out;
}

}
