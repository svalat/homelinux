
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
#include "../ProviderGithub.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,getPackage_ok_full)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"github/svalat/svpgm2txt"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,getPackage_ok_short)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"svalat/svpgm2txt"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,getPackage_not_ok_1)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"svalat/svpgm2txt360"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,getPackage_not_ok_2)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"svpgm2txt"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,getPackage_check_content)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);
	
	PackageDef pack;
	ASSERT_TRUE(provider.getPackage(pack,"github/svalat/svpgm2txt"));
	
	std::stringstream out;
	pack.save(out);
	
	//System::writeFile(out.str(),TEST_DATA_PATH "/github-full-dash.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/github-full-dash.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(ProviderGithub,search)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderGithub provider(&prefix);

	EXPECT_EQ("",provider.search("bash"));
}
