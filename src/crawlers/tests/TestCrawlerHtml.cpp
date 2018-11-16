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
#include <json/json.h>
#include "../CrawlerHtml.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  CONSTS  ***********************/
static const char cstValue1[] = "4.4.18 4.4.12 4.4 4.3.30 4.3 4.2.53 4.2 4.1 4.0 3.2.57 3.2.48 3.2 3.1 3.0.16 3.0 2.05 2.04 2.03 2.02.1 2.02 2.01.1 2.01 2.0 1.14.7";

/*******************  FUNCTION  *********************/
TEST(CrawlerHtml,constructor)
{
	Debug::enableAll();
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerHtml crawler(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerHtml,run)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerHtml crawler(&prefix);

	Json::Value cfg;
	cfg["mode"] = "html";
	cfg["url"] = "https://ftp.gnu.org/gnu/bash/";
	cfg["regexp"] = "bash-([0-9]+.[0-9]+.?[0-9]*).tar.gz";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("bash",cfg,versions);

	EXPECT_EQ(cstValue1,Helper::join(versions,' '));
}

/*******************  FUNCTION  *********************/
TEST(CrawlerHtml,run_invalid)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	CrawlerHtml crawler(&prefix);

	Json::Value cfg;
	cfg["mode"] = "html";
	cfg["url"] = "http://blabla.bof/fail";
	cfg["regexp"] = "bash-([0-9]+.[0-9]+.?[0-9]*).tar.gz";

	StringList versions;
	versions.push_back("2.47");

	versions = crawler.run("bash",cfg,versions);

	EXPECT_EQ("2.47",Helper::join(versions,' '));
}
