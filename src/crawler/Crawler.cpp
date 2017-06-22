/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
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
StringList Crawler::run(Json::Value & params,const StringList & origVersions)
{
	//check
	assert(params["mode"].asString() == name);

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
		StringList subVersions = run(params["subdir"],none);

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
	this->regexp = new RE2(params.get("regexp","^$").asString());

	//run
	versions = origVersions;
	for (auto & url : urls)
		internalRun(url);
	
	//clear
	delete regexp;
	regexp = NULL;

	//return
	return versions;
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
}

/********************  FUNCTION  *********************/
const std::string & Crawler::getName(void) const
{
	return name;
}

}
