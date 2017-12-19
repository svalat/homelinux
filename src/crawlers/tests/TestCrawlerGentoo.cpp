/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <core/Prefix.hpp>
#include <json/json.h>
#include "../CrawlerGentoo.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerGentoo,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGentoo crawler(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGentoo,run)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGentoo crawler(&prefix);

	Json::Value cfg;
	cfg["mode"] = "gentoo";
	cfg["url"] = "";
	cfg["regexp"] = "csh-([0-9]+.[0-9]+.[0-9]+).tar.gz";

	StringList versions;
	versions.push_back("0.4.1");

	versions = crawler.run("csh",cfg,versions);

	EXPECT_EQ("0.4.2 0.4.1",Helper::join(versions,' '));
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGentoo,run_invalid)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGentoo crawler(&prefix);

	Json::Value cfg;
	cfg["mode"] = "gentoo";
	cfg["url"] = "";
	cfg["regexp"] = "bash-([0-9]+.[0-9]+.[0-9]+).tar.gz";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("csh",cfg,versions);

	EXPECT_EQ("4.3",Helper::join(versions,' '));
}
