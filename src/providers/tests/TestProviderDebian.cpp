
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
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "../ProviderDebian.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,getPackage_ok_full)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"debian/cmake"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,getPackage_ok_short)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"cmake"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,getPackage_not_ok_1)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"debian/aaaaaaaa"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,getPackage_not_ok_2)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"aaaaaaa"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,getPackage_check_content)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);
	
	PackageDef pack;
	ASSERT_TRUE(provider.getPackage(pack,"debian/cmake"));
	
	std::stringstream out;
	pack.save(out);
	
	System::writeFile(out.str(),TEST_DATA_PATH "/debian-full-cmake.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/debian-full-cmake.json"),out.str());
}

/*******************  FUNCTION  *********************/
/*TEST(ProviderDebian,updateDb)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix,true);

	provider.updateDb();
}*/

/*******************  FUNCTION  *********************/
TEST(ProviderDebian,search)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderDebian provider(&prefix);

	EXPECT_EQ("\x1B[35mdebian/cmake\x1B[0m",provider.search("cmake"));
	EXPECT_EQ("",provider.search("bash"));
}
