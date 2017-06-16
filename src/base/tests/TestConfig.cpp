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
	
	const char * argv[] = {"homelinux","install","-v","gentoo,debien","gcc"};
	config.parseArgs(5,argv);
}

/*******************  FUNCTION  *********************/
TEST(Config,loadConfigValid)
{
    Config config;
    config.load(TEST_DATA_PATH "/config.json");
    
    EXPECT_EQ("/home/USER/usr",config.prefix);
    EXPECT_EQ("gentoo",config.host);
    ASSERT_EQ(2,config.modules.size());
    EXPECT_EQ("gcc-5.0",config.modules[0]);
    EXPECT_EQ("openmpi",config.modules[1]);
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