
/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cerrno>
#include <gtest/gtest.h>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include "../ProviderGentoo.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,getPackage_ok_full)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"gentoo/csh"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,getPackage_ok_short)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"csh"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,getPackage_not_ok_1)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"gentoo/bash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,getPackage_not_ok_2)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"bash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,getPackage_check_content)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);
	
	PackageDef pack;
	ASSERT_TRUE(provider.getPackage(pack,"gentoo/csh"));
	
	std::stringstream out;
	pack.save(out);
	
	//System::writeFile(out.str(),TEST_DATA_PATH "/gentoo-full-csh.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/gentoo-full-csh.json"),out.str());
}

/*******************  FUNCTION  *********************/
/*TEST(ProviderGentoo,updateDb)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix,true);

	provider.updateDb();
}*/

/*******************  FUNCTION  *********************/
TEST(ProviderGentoo,search)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGentoo provider(&prefix);

	EXPECT_EQ("\x1B[35mgentoo/csh-0.4.2.tar.gz\x1B[0m\n\x1B[35mgentoo/csh-0.4.1.tar.gz\x1B[0m",provider.search("csh"));
	EXPECT_EQ("",provider.search("bash"));
}
