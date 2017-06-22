/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cerrno>
#include <sstream>
#include <gtest/gtest.h>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "../ProviderHomelinux.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,updateCache)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
	provider.updateCache();
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,getPackage_ok_full)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"hl/app-shells/bash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,getPackage_ok_partial)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"app-shells/bash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,getPackage_ok_short)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"bash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,getPackage_no_exist)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"csh"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,getPackage_check_content)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);
	
	PackageDef pack;
	ASSERT_TRUE(provider.getPackage(pack,"hl/app-shells/bash"));
	
	std::stringstream out;
	//pack.save(TEST_DATA_PATH "/hl-full-bash.json");
	pack.save(out);
	
	//System::writeFile(out.str(),TEST_DATA_PATH "/hl-full-bash.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/hl-full-bash.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(ProviderHomelinux,search)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderHomelinux provider(&prefix);

	EXPECT_EQ("\x1B[32mhl/app-shells/bash:0\x1B[0m-\x1B[36m4.3.48\x1B[0m [\x1B[34m4.3.48 4.4.35 4.5.20\x1B[0m]",provider.search("bash"));
	EXPECT_EQ("",provider.search("csh"));
}
