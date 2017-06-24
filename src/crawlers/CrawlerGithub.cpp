/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include <core/Prefix.hpp>
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
	
	//debug
	HL_DEBUG_ARG("CrawlerGithub","Crawling %1").arg(url).end();

	//build name
	Helper::replaceInPlace(url,"github://","");

	//id to get bigger limit rate on API
	std::string oauth;
	std::string clientId = prefix->getUserConfig().clientId;
	std::string clientSecret = prefix->getUserConfig().clientSecret;
	if (clientId.empty() == false && clientSecret.empty() == false)
		oauth = "?client_id="+clientId+"&client_secret="+clientSecret;

	//fetch
	Json::Value json;
	if (System::downloadJson(json,"https://api.github.com/repos/"+url+"/releases"+oauth))
	{
		key = "tag_name";
	} else if (System::downloadJson(json,"https://api.github.com/repos/"+url+"/tags"+oauth)) {
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
