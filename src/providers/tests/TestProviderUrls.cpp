
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
#include "../ProviderUrls.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(ProviderUrls,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderUrls provider(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(ProviderUrls,getPackage_ok_full)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderUrls provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"urls/dash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderUrls,getPackage_ok_short)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderUrls provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"dash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderUrls,getPackage_check_content)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderUrls provider(&prefix);
	
	PackageDef pack;
	ASSERT_TRUE(provider.getPackage(pack,"urls/dash"));
	
	std::stringstream out;
	pack.save(out);
	
	//System::writeFile(out.str(),TEST_DATA_PATH "/url-full-dash.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/url-full-dash.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(ProviderUrls,search)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderUrls provider(&prefix);

	EXPECT_EQ("\x1B[35murls/dash-0.5.5.1.tar.gz\x1B[0m\n\x1B[35murls/dash-0.5.5.2.tar.gz\x1B[0m",provider.search("dash"));
	EXPECT_EQ("",provider.search("bash"));
}
