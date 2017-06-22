/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CrawlerHtml.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(CrawlerHtml,constructor)
{
	CrawlerHtml crawler(NULL);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerHtml,run)
{
	CrawlerHtml crawler(NULL);

	Json::Value config;
	config["mode"] = "html";
	config["url"] = "https://ftp.gnu.org/gnu/bash/";
	config["regexp"] = "bash-([0-9]+.[0-9]+.?[0-9]*).tar.gz";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("bash",config,versions);

	EXPECT_EQ("4.3",Helper::join(versions,' '));
}

/*******************  FUNCTION  *********************/
TEST(CrawlerHtml,run_invalid)
{
	CrawlerHtml crawler(NULL);

	Json::Value config;
	config["mode"] = "html";
	config["url"] = "http://blabla.bof/fail";
	config["regexp"] = "bash-([0-9]+.[0-9]+.?[0-9]*).tar.gz";

	StringList versions;
	versions.push_back("2.47");

	versions = crawler.run("bash",config,versions);

	EXPECT_EQ("2.47",Helper::join(versions,' '));
}
