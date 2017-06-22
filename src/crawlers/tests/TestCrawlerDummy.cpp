/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CrawlerDummy.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerDummy,constructor)
{
	CrawlerDummy crawler(NULL);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerDummy,run)
{
	CrawlerDummy crawler(NULL);

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
