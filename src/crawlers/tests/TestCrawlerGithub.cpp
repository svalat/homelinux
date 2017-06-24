/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <core/Prefix.hpp>
#include "../CrawlerGithub.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerGithub,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGithub crawler(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGithub,run)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGithub crawler(&prefix);

	Json::Value cfg;
	cfg["mode"] = "github";
	cfg["url"] = "github://svalat/svUnitTest";
	cfg["regexp"] = "v([0-9]+.[0-9]+.[0-9]+)";

	StringList versions;
	versions.push_back("0.4.0");

	versions = crawler.run("github/svalat/svUnitTest",cfg,versions);

	EXPECT_EQ("0.4.0 0.3.0 0.2.0 0.1.0",Helper::join(versions,' '));
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGithub,run_invalid)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGithub crawler(&prefix);

	Json::Value cfg;
	cfg["mode"] = "github";
	cfg["url"] = "github://aaaaaaaaaaa546464/bbbb65466465";
	cfg["regexp"] = "v([0-9]+.[0-9]+.[0-9]+)";

	StringList versions;
	versions.push_back("2.47");

	versions = crawler.run("github/aaaaaaaaaaa546464/bbbb65466465",cfg,versions);

	EXPECT_EQ("2.47",Helper::join(versions,' '));
}
