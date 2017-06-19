/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cerrno>
#include <gtest/gtest.h>
#include <portability/System.hpp>
#include <core/Prefix.hpp>
#include <core/QuickPackage.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(QuickPackage,constructor)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	QuickPackage quick(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(QuickPackage,getQuickInfo)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	QuickPackage quick(&prefix);
	
	//empty
	EXPECT_TRUE(quick.getQuickInfo("config","none/empty").empty());
	
	//get
	const StringList & lst = quick.getQuickInfo("config","gentoo/csh");
	EXPECT_EQ("--enable-debug",Helper::getListEl(lst,0));
}

/*******************  FUNCTION  *********************/
TEST(QuickPackage,genPackage_empty)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	QuickPackage quick(&prefix);
	
	PackageDef pack;
	quick.genPackage(pack,"none/empty");
	
	std::stringstream out;
	pack.save(out);
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/quick-package-empty.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(QuickPackage,genPackage_test)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	QuickPackage quick(&prefix);
	
	PackageDef pack;
	quick.genPackage(pack,"none/test");
	
	std::stringstream out;
	//pack.save(TEST_DATA_PATH "/quick-package-test.json");
	pack.save(out);
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/quick-package-test.json"),out.str());
}
