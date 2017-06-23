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
#include <algorithm>
#include <base/Helper.hpp>
#include <base/Debug.hpp>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "ProviderHomelinux.hpp"
#include <re2/re2.h>
#include <base/Colors.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
ProviderHomelinux::ProviderHomelinux(Prefix * prefix)
               :Provider("homelinux",prefix)
{
	this->warn = false;
}

/*******************  FUNCTION  *********************/
ProviderHomelinux::~ProviderHomelinux(void)
{
	
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::loadCache(void)
{
	//already done
	if (cache.empty() == false)
		return;
	
	//load
	std::string path = prefix->getFilePath("/homelinux/packages/db/cache.json");
	if (System::fileExist(path))
	{
		//load
		Json::Value json;
		System::loadJson(json,path);
		Helper::jsonToObj(cache,json);
	} else {
		if (!warn)
	    	HL_ERROR("No cache file available, consider to call 'hl update-cache' at least once");
		warn = true;
	}
}

/*******************  FUNCTION  *********************/
bool ProviderHomelinux::getPackage(PackageDef & out,const std::string & name)
{
	//build name
	std::string shortName;
	std::string packageName = name;
	if (Helper::startBy(name,"hl/"))
	{
		shortName = packageName.substr(3);
	} else {
		shortName = packageName;
		packageName = "hl/"+shortName;
	}
	
	//search in cache
	if (Helper::contain(shortName,"/") == false)
	{
		//search
		shortName = this->searchInCache("/"+shortName);
		
		//not fount
		if (shortName.empty())
			return false;
		
		//build full name
		packageName = "hl/"+shortName;
	}
	
	//check in cache
	if (packageCache.find(packageName) != packageCache.end())
	{
		out.merge(packageCache[packageName]);
		return true;
	}
	
	//load from cache
	PackageDef & pack = packageCache[packageName];
	std::string path = prefix->getFilePath("/homelinux/packages/db/"+shortName+".json");
	HL_DEBUG_ARG("ProviderHomelinux","Try to load file : %1").arg(path).end();
	if (System::fileExist(path))
	{
		pack.load(path);
		
		//apply version
		this->loadVersions();
		if (versions.find(packageName) != versions.end())
			pack.versions = versions[packageName];

		//apply name
		pack.name = packageName;
		
		//merge
		out.merge(pack);
		
		return true;
	} else {
		return false;
	}
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::updateCache(void)
{
	//setup
	RE2 regexp("([A-Za-z0-9_-]+/[A-Za-z0-9+_-]+)\\.json");
	std::string path = prefix->getFilePath("/homelinux/packages/db");
	
	//check
	assumeArg(System::fileExist(path),"Fail to find homelinux db : %1 !").arg(path).end();
	
	//scan
	System::findFiles(path,[this,&regexp](const std::string & file){
		std::string packageName;
		if (RE2::FullMatch(file,regexp,&packageName))
			cache.push_back(packageName);
	});
	
	//convert to json
	Json::Value json;
	Helper::toJson(json,cache);
	
	//write
	std::string jsonPath = prefix->getFilePath("/homelinux/packages/db/cache.json");
	System::writeJson(json,jsonPath);
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::updateDb(void)
{
	//TODO
}

/*******************  FUNCTION  *********************/
std::string ProviderHomelinux::search(const std::string & name)
{
	//vars
	StringList lst;
	
	//ensure load
	this->loadCache();
	
	//conver
	std::string lowerName = Helper::toLower(name);
	for (auto & entry : cache)
	{
		//get name
		std::string pn;
		Helper::split(entry,'/',[&pn](const std::string & value){
			pn = value;
		});
		
		//to lower
		pn = Helper::toLower(pn);
		if (Helper::contain(pn,name))
		{
			//@TODO replace by prefix.loadPackage
			PackageDef def;
			entry = "hl/"+entry;
			bool status = getPackage(def,entry);
			assumeArg(status,"Fail to load package %1").arg(entry).end();
			
			//sort versions
			def.sortVersions();
			
			//get vars
			std::string slot = def.getSlot();
			std::string version = def.getVersion();
			std::string versions = def.getNVersions(10);
			
			//build full
			std::string full = Colors::green(entry+":"+slot)+"-"+Colors::cyan(version)+" ["+Colors::blue(versions)+"]";
			lst.push_back(full);
		}
	}
	
	return Helper::join(lst,'\n');
}

/*******************  FUNCTION  *********************/
std::string ProviderHomelinux::searchInCache(const std::string & name)
{
	//vars
	StringList lst;
	
	//get cache
	this->loadCache();
	
	//loop
	for (auto entry : cache)
		if (Helper::endBy(entry,name))
			lst.push_back(entry);
			
	//check
	if (lst.empty())
	{
		return "";
	} else if (lst.size() == 1) {
		return lst.front();
	} else {
		HL_FATAL_ARG("Fail to find your package, multiple match : %1").arg(Helper::join(lst,',')).end();
		return "";
	}
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::loadVersions(void)
{
	//path
	std::string path = prefix->getFilePath("/homelinux/packages/db/versions.json");
	
	//check if exist
	Json::Value json;
	System::loadJson(json,path);
	
	//convert
	Helper::jsonToObj(versions,json);
}

}
