/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include "CrawlerGithub.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerGithub::CrawlerGithub(Prefix * prefix) 
		   :Crawler("github",prefix)
{
	
}

/*******************  FUNCTION  *********************/
void CrawlerGithub::internalRun(std::string url)
{
	//vars
	std::string key;

	//build name
	Helper::replaceInPlace(url,"github://","");

	//fetch
	Json::Value json;
	if (System::downloadJson(json,"https://api.github.com/repos/"+url+"/releases"))
	{
		key = "tag_name";
	} else if (System::downloadJson(json,"https://api.github.com/repos/"+url+"/tags")) {
		key = "name";
	} else {
		HL_ERROR_ARG("Package %1 fail to get github versions via release/tags : %2")
			.arg(packageName)
			.arg("github://"+url).end();
		return;
	}

	//load
	if (json.isArray())
	{
		for (int i = 0 ; i < json.size() ; i++)
			scanValue(json[i][key].asString());
	}
}

}
