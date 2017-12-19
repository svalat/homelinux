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
#include <base/Helper.hpp>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "ProviderModels.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
ProviderModels::ProviderModels(Prefix * prefix)
               :Provider("models",prefix)
{
	
}

/*******************  FUNCTION  *********************/
ProviderModels::~ProviderModels(void)
{
	
}

/*******************  FUNCTION  *********************/
bool ProviderModels::getPackage(PackageDef & out,const std::string & name)
{
	//check if correct
	if (Helper::startBy(name,"models/") == false)
		return false;
		
	//check if cache
	if (cache.find(name) != cache.end())
	{
		out.merge(cache[name]);
		return true;
	}
	
	//get path
	std::string path = this->prefix->getFilePath("/homelinux/packages/"+name+".json");
	
	//check if exist and load
	if (System::fileExist(path))
	{
		cache[name].load(path);
		out.merge(cache[name]);
		return true;
	} else {
		return false;
	}
}

/*******************  FUNCTION  *********************/
void ProviderModels::updateCache(void)
{
	//nothing
}

/*******************  FUNCTION  *********************/
void ProviderModels::updateDb(void)
{
	//nothing
}

/*******************  FUNCTION  *********************/
std::string ProviderModels::search(const std::string & name)
{
	return "";
}

}
