/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CrawlerGnomeCache.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerGnomeCache,constructor)
{
	CrawlerGnomeCache crawler;
}

/*******************  FUNCTION  *********************/
TEST(CrawlerGnomeCache,run)
{
	CrawlerGnomeCache crawler;

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
	CrawlerGnomeCache crawler;

	Json::Value config;
	config["mode"] = "gnome-cache";
	config["url"] = "http://dddftp.gnomesd.sdorg/pub/gnome/sources/glibmm/cache.json";
	config["regexp"] = "";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("hl/dev-cpp/glibmm",config,versions);

	EXPECT_EQ("4.3",Helper::join(versions,' '));
}