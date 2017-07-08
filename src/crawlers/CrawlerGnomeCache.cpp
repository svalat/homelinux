/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include "CrawlerGnomeCache.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerGnomeCache::CrawlerGnomeCache(Prefix * prefix) 
		   :Crawler("gnome-cache",prefix)
{
	
}

/*******************  FUNCTION  *********************/
void CrawlerGnomeCache::internalRun(std::string url)
{
	//add / at end otherwise curl fail
	if (Helper::endBy(url,"/cache.json") == false)
	{
		HL_ERROR_ARG("Package %1 has invalid url for gnome cache : %2")
			.arg(packageName)
			.arg(url)
			.end();
		return;
	}

	//debug
	HL_DEBUG_ARG("CrawlerGnomeCache","Crawling %1").arg(url).end();

	//load
	Json::Value json;
	bool status = System::downloadJson(json,url);

	//shortname
	std::string shortName = Helper::last(packageName,'/');

	//from user
	std::string pname = options.get("name","").asString();
	if (pname.empty() == false)
		shortName = pname;

	//check status
	if (!status)
	{
		HL_ERROR_ARG("Download failure for package %1 : %2").arg(packageName).arg(url).end();
	} else {
		if (json.isArray() && json[0].asInt() == 4)
		{
			Json::Value node = json[1][shortName];
			for (Json::Value::iterator it = node.begin() ; it != node.end() ; ++it)
				versions.push_back(it.key().asString());
		} else {
			HL_ERROR_ARG("Package %1 has invalid gnome cache format on %2").arg(packageName).arg(url).end();
		}
	}
}

}
