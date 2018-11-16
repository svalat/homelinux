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
#include "../CrawlerDummy.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerDummy,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerDummy crawler(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerDummy,run)
{
	Config cfg;
	Prefix prefix(&cfg,TEST_DATA_PATH);
	CrawlerDummy crawler(&prefix);

	Json::Value config;
	config["mode"] = "dummy";
	config["url"] = "";
	config["regexp"] = "";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("bash",config,versions);

	ASSERT_EQ(1,versions.size());
	EXPECT_EQ("4.3",versions.front());
}
