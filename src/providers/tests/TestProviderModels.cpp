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
#include "../ProviderModels.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(ProviderModels,constructor)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderModels provider(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(ProviderModels,getPackage_no_models)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderModels provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"hl/app-shell/bash"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderModels,getPackage_no_exist)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderModels provider(&prefix);
	
	PackageDef pack;
	EXPECT_FALSE(provider.getPackage(pack,"model/fake"));
}

/*******************  FUNCTION  *********************/
TEST(ProviderModels,getPackage_ok)
{
    Config config;
	Prefix prefix(&config,TEST_DATA_PATH);
	
	ProviderModels provider(&prefix);
	
	PackageDef pack;
	EXPECT_TRUE(provider.getPackage(pack,"models/default"));
}
