/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CrawlerFtp.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/********************  CONSTS  **********************/
const char cstValue1[] = "4.4 4.3.30 4.3 4.2.53 4.2 4.1 4.0 3.2.57 3.2.48 3.2 3.1 3.0.16 3.0 2.05 2.04 2.03 2.02.1 2.02 2.01.1 2.01 2.0 1.14.7";
const char cstValue2[] = "5.9.0 5.8.0 5.7.1 5.7.0 5.6.2 5.6.1 5.6.0 5.5.1 5.5.0 5.4.2 5.4.1 5.4.0 5.3.2 5.3.1 5.3.0 5.2.1 5.2.0 5.1.1 5.1.0 5.0.2 5.0.1 5.0.0 4.8.6 4.8.5 4.8.4 4.8.3 4.8.2 4.8.1 4.8.0";

/*******************  FUNCTION  *********************/
TEST(CrawlerFtp,constructor)
{
	CrawlerFtp crawler(NULL);
}

/*******************  FUNCTION  *********************/
TEST(CrawlerFtp,run)
{
	CrawlerFtp crawler(NULL);

	Json::Value config;
	config["mode"] = "ftp";
	config["url"] = "ftp://ftp.gnu.org/gnu/bash";
	config["regexp"] = "bash-([0-9]+.[0-9]+.?[0-9]*.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz)";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("bash",config,versions);

	EXPECT_EQ(cstValue1,Helper::join(versions,' '));
}

/*******************  FUNCTION  *********************/
/*TEST(CrawlerFtp,run_subdir)
{
	CrawlerFtp crawler(NULL);

	Json::Value config;
	config["mode"] = "ftp";
	config["url"] = "ftp://ftp.mirrorservice.org/sites/download.qt-project.org/archive/qt/${SVERSION}/";
	config["regexp"] = "([0-9]+.[0-9]+.?[0-9]*.?[0-9]*)";
	
	//subdir
	config["subdir"]["mode"] = "ftp";
	config["subdir"]["url"] = "ftp://ftp.mirrorservice.org/sites/download.qt-project.org/archive/qt/";
	config["subdir"]["regexp"] = "([0-9]+.[0-9]+)";

	StringList versions;
	versions.push_back("5.9.0");

	versions = crawler.run("qt",config,versions);

	EXPECT_EQ(cstValue2,Helper::join(versions,' '));
}*/

/*******************  FUNCTION  *********************/
TEST(CrawlerFtp,run_invalid)
{
	CrawlerFtp crawler(NULL);

	Json::Value config;
	config["mode"] = "ftp";
	config["url"] = "ftp://azertyuio.invalid/notok/";
	config["regexp"] = "bash-([0-9]+.[0-9]+.?[0-9]*.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz)";

	StringList versions;
	versions.push_back("4.3");

	versions = crawler.run("bash",config,versions);

	EXPECT_EQ("4.3",Helper::join(versions,' '));
}