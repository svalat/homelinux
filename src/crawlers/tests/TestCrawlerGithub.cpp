/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CrawlerGithub.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerGithub,constructor)
{
	CrawlerGithub crawler(NULL);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGithub,run)
{
	CrawlerGithub crawler(NULL);

	Json::Value config;
	config["mode"] = "github";
	config["url"] = "github://svalat/svUnitTest";
	config["regexp"] = "v([0-9]+.[0-9]+.[0-9]+)";

	StringList versions;
	versions.push_back("0.4.0");

	versions = crawler.run("github/svalat/svUnitTest",config,versions);

	EXPECT_EQ("0.4.0 0.3.0 0.2.0 0.1.0",Helper::join(versions,' '));
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGithub,run_invalid)
{
	CrawlerGithub crawler(NULL);

	Json::Value config;
	config["mode"] = "github";
	config["url"] = "github://aaaaaaaaaaa546464/bbbb65466465";
	config["regexp"] = "v([0-9]+.[0-9]+.[0-9]+)";

	StringList versions;
	versions.push_back("2.47");

	versions = crawler.run("github/aaaaaaaaaaa546464/bbbb65466465",config,versions);

	EXPECT_EQ("2.47",Helper::join(versions,' '));
}
