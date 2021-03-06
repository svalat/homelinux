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
#include <algorithm>
#include <base/Helper.hpp>
#include <base/Debug.hpp>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "ProviderGithub.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
ProviderGithub::ProviderGithub(Prefix * prefix)
               :Provider("github",prefix)
{
}

/*******************  FUNCTION  *********************/
bool ProviderGithub::getPackage(PackageDef & out,const std::string & name)
{
	//if no /, wrong
	if (Helper::contain(name,"/") == false)
		return false;
	
	//build name
	std::string shortName;
	std::string packageName = name;
	if (Helper::startBy(name,"github/"))
	{
		shortName = packageName.substr(7);
	} else {
		return false;
	}
	
	//get quickpack
	PackageDef quickpack;
	prefix->getQuickPackage(quickpack,packageName);
	
	//id to get bigger limit rate on API
	std::string oauth;
	std::string clientId = prefix->getUserConfig().clientId;
	std::string clientSecret = prefix->getUserConfig().clientSecret;
	if (clientId.empty() == false && clientSecret.empty() == false)
		oauth = "?client_id="+clientId+"&client_secret="+clientSecret;
	
	//fetch from API
	Json::Value json;
	bool status = System::downloadJson(json,"https://api.github.com/repos/"+shortName+"/releases/latest"+oauth);
	
	//get what we whant
	if (status && json["tag_name"].isString())
	{
		std::string tag = json["tag_name"].asString();
		if (Helper::startBy(tag,"v"))
			quickpack.versions.push_back(tag.substr(1));
		else if (Helper::startBy(tag,"release_"))
			quickpack.versions.push_back(tag.substr(8));
		else
			quickpack.versions.push_back(tag);
		quickpack.urls.push_back("https://github.com/"+shortName+"/archive/"+tag+".tar.gz");
		quickpack.steps["download"].push_back("hl_github_download");
		quickpack.name = packageName;
		out.merge(quickpack);
		return true;
	}
	
	//second chance with taggs if there is no release
	status = System::downloadJson(json,"https://api.github.com/repos/"+shortName+"/tags");
	if (status)
	{
		std::string tag = json[0]["name"].asString();
		if (tag.empty() == false && tag[0] == 'v')
		{
			quickpack.versions.push_back(tag.substr(1));
			quickpack.urls.push_back("https://github.com/"+shortName+"/archive/"+tag+".tar.gz");
			quickpack.steps["download"].push_back("hl_github_download");
			quickpack.name = packageName;
			out.merge(quickpack);
			return false;
		} else {
			return false;
		}
	}
	
	//ok this time we fail
	return false;
}

/*******************  FUNCTION  *********************/
void ProviderGithub::updateCache(void)
{
	
}

/*******************  FUNCTION  *********************/
void ProviderGithub::updateDb(void)
{
	//TODO
}

/*******************  FUNCTION  *********************/
std::string ProviderGithub::search(const std::string & name)
{
	return "";
}

}
