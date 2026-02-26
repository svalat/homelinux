/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include <core/Prefix.hpp>
#include "CrawlerGitlab.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerGitlab::CrawlerGitlab(Prefix * prefix) 
		   :Crawler("github",prefix)
{
	
}

/*******************  FUNCTION  *********************/
void CrawlerGitlab::internalRun(std::string url)
{
	//vars
	std::string key;
	
	//debug
	HL_DEBUG_ARG("CrawlerGitlab","Crawling %1").arg(url).end();
	
	//check address
	if (Helper::startBy(url,"github://") == false)
	{
		HL_ERROR_ARG("Gitlab addresses should start by github://, as %1").arg(url).end();
		return;
	}

	//build name
	Helper::replaceInPlace(url,"github://","");
	
	//errors
	if (Helper::endBy(url,"/"))
	{
		HL_ERROR("Gitlab address must not end by /");
		return;
	}

	//get server name
	const ssize_t first_slash = url.find("/");
	assumeArg(first_slash != std::string::npos, "Fail to find server address, not / in : %1")
		.arg(url)
		.end();
	std::string server = url.substr(0, first_slash);
	Helper::replaceInPlace(url,server,"");

	//set id
	std::string project_id = url;
	Helper::replaceInPlace(project_id,"/","%2F");

	//check if force tag
	bool forceTag = options.get("forcetag",false).asBool();

	//fetch
	Json::Value json;
	if (forceTag == false && System::downloadJson(json,"https://" + server + "/api/v4/projects/" + url + "/releases") && json.size() > 0)
	{
		key = "name";
	} else if (System::downloadJson(json,"https://" + server + "/api/v4/projects/" + url + "/tags")) {
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
		for (Json::ArrayIndex i = 0 ; i < json.size() ; i++)
			scanValue(json[i]["name"].asString());
		for (Json::ArrayIndex i = 0 ; i < json.size() ; i++)
			scanValue(json[i][key].asString());
	}
}

}
