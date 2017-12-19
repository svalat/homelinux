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
#include <base/Config.hpp>
#include <base/Debug.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(Config,constructor)
{
	Config config;
}

/*******************  FUNCTION  *********************/
TEST(Config,parseArgs)
{
	Config config;
	
	const char * argv[] = {"homelinux","-v","gentoo,debien","-p","/home/USER/usr","install","gcc"};
	config.parseArgs(7,argv);
	
	EXPECT_EQ("install",config.command);
	EXPECT_EQ("/home/USER/usr",Helper::join(config.prefix,','));
	EXPECT_EQ("gcc",Helper::join(config.args,','));
}

/*******************  FUNCTION  *********************/
TEST(Config,loadConfigValid)
{
	Config config;
	config.load(TEST_DATA_PATH "/config.json");
	
	EXPECT_EQ("/home/USER/usr",Helper::getListEl(config.prefix,0));
	EXPECT_EQ(1,config.prefix.size());
	EXPECT_EQ("gentoo",config.host);
	ASSERT_EQ(2,config.modules.size());
	EXPECT_EQ("gcc-5.0",Helper::getListEl(config.modules,0));
	EXPECT_EQ("openmpi",Helper::getListEl(config.modules,1));
	EXPECT_TRUE(config.homecache);
	EXPECT_TRUE(config.ccache);
	EXPECT_FALSE(config.pyEnv);
	EXPECT_EQ("--enable-debug",config.packageOverride["hl/sys-devel/gcc"]["configure"][""][0].asString());
}

/*******************  FUNCTION  *********************/
TEST(Config,loadConfigInvaild)
{
	Config config;
	config.load(TEST_DATA_PATH "/config-no.json");
}

