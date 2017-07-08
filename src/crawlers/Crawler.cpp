/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <core/VersionMatcher.hpp>
#include "Crawler.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  FUNCTION  *********************/
Crawler::Crawler(const std::string & name,Prefix * prefix)
{
	this->name = name;
	this->prefix = prefix;
	this->regexp = NULL;
}

/********************  FUNCTION  *********************/
Crawler::~Crawler(void)
{
	if (regexp != NULL)
		delete regexp;
}

/********************  FUNCTION  *********************/
StringList Crawler::run(const std::string & packageName,Json::Value & params,const StringList & origVersions)
{
	//check
	assert(params["mode"].asString() == name);

	//setup
	this->packageName = packageName;

	//extract urls
	StringList urls;
	if (params["url"].isString())
		urls.push_back(params["url"].asString());
	else if (params["url"].isArray())
		for (Json::ArrayIndex i = 0 ; i < params["url"].size() ; i++)
			urls.push_back(params["url"][i].asString());

	//manage subdir
	if (params["subdir"].isObject())
	{
		//move
		StringList tmp = urls;
		urls.clear();

		//run subdir
		StringList none;
		StringList subVersions = run(packageName,params["subdir"],none);

		//fill
		//for (auto & v : subVersions)
		forEach(StringList,v,subVersions)
		{
			//for (auto u : tmp)
			forEach(StringList,u,tmp)
			{
				Helper::replaceInPlace(*u,"${SVERSION}",*v);
				urls.push_back(*u);
			}
		}
	}
		
	//extract regexp
	std::string r = params.get("regexp","").asString();
	if (Helper::contain(r,"^") || Helper::contain(r,"$"))
	{
		HL_WARNING_ARG("Character '^' and '$' are not needed as we already apply full match %1 : %2. Removed them.").arg(packageName).arg(r).end();
		Helper::replaceInPlace(r,"^","");
		Helper::replaceInPlace(r,"$","");
	}
	if (params.get("escapePoint",true).asBool() && Helper::contain(r,"\\") == false)
		Helper::replaceInPlace(r,".","\\.");
	this->regexp = new Regexp(r);
	this->txtRegexp = r;
	
	//set options
	options = params;

	//run
	versions = origVersions;
	//for (auto & url : urls)
	forEach(StringList,url,urls)
	{
		HL_DEBUG_ARG("Crawler","Crawl %1").arg(*url).end();
		internalRun(*url);
	}
	
	//clear
	delete regexp;
	regexp = NULL;
	
	if (origVersions.size() == versions.size() && getName() != "dummy")
		HL_WARNING_ARG("Crawler don't find any versions for package %1. Check address and regexp !").arg(packageName).end();

	//sort
	versions = VersionMatcher::sortList(versions);

	//return
	return makeUniq(versions);
}

/********************  FUNCTION  *********************/
StringList Crawler::makeUniq(StringList & lst)
{
	std::string last;
	StringList clean;
	//for (auto & it : lst)
	forEach(StringList,it,lst)
	{
		if (*it != last)
		{
			clean.push_back(*it);
			last = *it;
		}
	}

	return clean;
}

/********************  FUNCTION  *********************/
void Crawler::scanValue(const std::string & value)
{
	//check
	assert(regexp != NULL);

	//debug
	HL_DEBUG_ARG("CrawlerDetails","Scan value : %1").arg(value).end();

	//run
	std::string version;
	if (regexp->match(value,version))
		versions.push_back(version);
	
	//HL_DEBUG_ARG("Crawler","%1 reject %2").arg(packageName).arg(value).end();
}

/********************  FUNCTION  *********************/
const std::string & Crawler::getName(void) const
{
	return name;
}

}
