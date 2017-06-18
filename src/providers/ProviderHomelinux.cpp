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
#include <base/Helper.hpp>
#include <base/Debug.hpp>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "ProviderHomelinux.hpp"

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
	std::string path = prefix->getFilePath("/homelinux/packages/db/cache.lst");
	if (System::fileExist(path))
	{
		//load
		std::string content = System::loadFile(path);
	
		//split
		Helper::stringSplit(content,'\n',[this](const std::string & value){
			this->cache.push_back(value);
		});
	} else {
		if (!warn)
	    	HL_FATAL("No cache file available, consider to call 'hl update-cache' at least once");
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
		shortName = this->searchInCache(shortName);
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
	std::string path = prefix->getFilePath("/homelinux/package/db/"+shortName+".json");
	if (System::fileExist(path))
	{
		pack.load(path);
		
		//apply version
		this->loadVersions();
		if (versions.find(packageName) != versions.end())
			pack.versions = versions["packageName"];
		
		return true;
	} else {
		return false;
	}
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::updateCache(void)
{
	
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::updateDb(void)
{
	
}

/*******************  FUNCTION  *********************/
std::string ProviderHomelinux::search(const std::string & name)
{
	
}

/*******************  FUNCTION  *********************/
std::string ProviderHomelinux::searchInCache(const std::string & name)
{
	
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::loadVersions(void)
{
	
}

}
