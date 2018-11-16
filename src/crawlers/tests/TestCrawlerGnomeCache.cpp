/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <base/Config.hpp>
#include <core/Prefix.hpp>
#include "../CrawlerGnomeCache.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerGnomeCache,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerGnomeCache crawler(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGnomeCache,run)
{
	Config cfg;
	Prefix prefix(&cfg,TEST_DATA_PATH);
	CrawlerGnomeCache crawler(&prefix);

	Json::Value config;
	config["mode"] = "gnome-cache";
	config["url"] = "http://ftp.gnome.org/pub/gnome/sources/glibmm/cache.json";
	config["regexp"] = "";

	StringList versions;
	versions.push_back("2.47");

	versions = crawler.run("hl/dev-cpp/glibmm",config,versions);

	EXPECT_EQ("2.3.0",versions.back());
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGnomeCache,run_invalid)
{
	Config cfg;
	Prefix prefix(&cfg,TEST_DATA_PATH);
	CrawlerGnomeCache crawler(&prefix);

	Json::Value config;
	config["mode"] = "gnome-cache";
	config["url"] = "http://dddftp.gnomesd.sdorg/pub/gnome/sources/glibmm/cache.json";
	config["regexp"] = "";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("hl/dev-cpp/glibmm",config,versions);

	EXPECT_EQ("4.3",Helper::join(versions,' '));
}