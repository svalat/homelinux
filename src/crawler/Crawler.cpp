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
Crawler::Crawler(const std::string & name)
{
	this->name = name;
	this->regexp = NULL;
}

/********************  FUNCTION  *********************/
Crawler::~Crawler(void)
{
	if (regexp != NULL)
		delete regexp;
}

/********************  FUNCTION  *********************/
StringList Crawler::run(const std::string & packagName,Json::Value & params,const StringList & origVersions)
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
		for (int i = 0 ; i < params["url"].size() ; i++)
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
		for (auto & v : subVersions)
		{
			for (auto u : tmp)
			{
				Helper::replaceInPlace(u,"${SVERSION}",v);
				urls.push_back(u);
			}
		}
	}
		
	//extract regexp
	std::string r = params.get("regexp","^$").asString();
	if (params.get("escapePoint",true).asBool())
		Helper::replaceInPlace(r,".","\\.");
	this->regexp = new RE2(r);

	//run
	versions = origVersions;
	for (auto & url : urls)
	{
		HL_DEBUG_ARG("Crawler","Crawl %1").arg(url).end();
		internalRun(url);
	}
	
	//clear
	delete regexp;
	regexp = NULL;

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
	for (auto & it : lst)
	{
		if (it != last)
		{
			clean.push_back(it);
			last = it;
		}
	}

	return clean;
}

/********************  FUNCTION  *********************/
void Crawler::scanValue(const std::string & value)
{
	//check
	assert(regexp != NULL);

	//run
	std::string version;
	if (RE2::FullMatch(value,*regexp,&version))
		versions.push_back(version);
	
	//HL_DEBUG_ARG("Crawler","%1 reject %2").arg(packageName).arg(value).end();
}

/********************  FUNCTION  *********************/
const std::string & Crawler::getName(void) const
{
	return name;
}

}
