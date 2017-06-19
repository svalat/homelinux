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
#include <providers/ProviderGentoo.hpp>
#include <providers/ProviderDebian.hpp>
#include <providers/ProviderUrls.hpp>
#include <providers/ProviderModels.hpp>
#include <providers/ProviderHomelinux.hpp>
#include <providers/ProviderGithub.hpp>
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
	for (auto & it : providers)
		delete it.second;
	providers.clear();

	//inherited prefix
	for (auto & it :inheritedPrefix)
		delete it;
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
	Helper::jsonToObj(prefixConfig.use,json["user"]);
	Helper::jsonToObj(prefixConfig.modules,json["modules"]);
	Helper::jsonToObj(prefixConfig.packageOverride,json["packageOverride"]);
	prefixConfig.gentoo = json["gentoo"];
	prefixConfig.debian = json["debian"];
	Helper::jsonToObj(prefixConfig.providers,json["providers"]);
	prefixConfig.providers.push_front("models");
	prefixConfig.useGnuStow = json.get("useGnuStow",false).asBool();

	//load inherited prefix
	for (auto p : prefixConfig.inherit)
		inheritedPrefix.push_back(new Prefix(config,p,false));
}

/*******************  FUNCTION  *********************/
std::string Prefix::getFilePath(const std::string path) const
{
	return this->prefix + "/" + path;
}

/*******************  FUNCTION  *********************/
void Prefix::loadPackage(PackageDef & out,const std::string packageName)
{
	HL_FATAL("TODO");
}

/*******************  FUNCTION  *********************/
void Prefix::getQuickPackage(PackageDef & out,const std::string & packageName)
{
	getQuickPackage().genPackage(out,packageName);
}

/*******************  FUNCTION  *********************/
const PrefixConfig & Prefix::getConfig(void)
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
	auto it = providers.find(name);
	
	//ok
	if (it != providers.end())
		return *(it->second);
	
	//need to allocate
	Provider * ret = NULL;
	if (name == "gentoo")
		ret = providers["gentoo"] = new ProviderGentoo(this);
	else if (name == "debian")
		ret = providers["debian"] = new ProviderDebian(this);
	else if (name == "model")
		ret = providers["model"] = new ProviderModels(this);
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
bool Prefix::hasInstalledPackage(const std::string & value)
{
	//TODO
	return false;
}

/*******************  FUNCTION  *********************/
//@TODO make parallel
void Prefix::updateDb(void)
{
	for (auto prov : prefixConfig.providers)
	{
		Provider & p = getProvider(prov);
		HL_MESSAGE_ARG("Update DB for provier %1").arg(prov).end();
		p.updateDb();
	}
}

/*******************  FUNCTION  *********************/
void Prefix::fillEnv(EnvSetup & env)
{
	for (auto it :inheritedPrefix)
		it->fillEnv(env);
	env.addPrefix(this->prefix);
}

}
