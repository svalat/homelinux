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
#include <portability/System.hpp>
#include <sstream>
#include "../EnvSetup.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(EnvSetup,constructor)
{
	Config config;
	EnvSetup env(&config);
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,loadCurrent)
{
	Config config;
	EnvSetup env(&config);
	env.loadCurrent();
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,addPrefix)
{
	Config config;
	EnvSetup env(&config, false);
	env.addPrefix("/home/USER/usr");
	std::stringstream out;
	env.print(out);
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/addPrefix.txt"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,removeExisting)
{
	Config config;
	EnvSetup env(&config, false);
	env.addPrefix("/home/USER/usr");
	env.removeExisting();
	std::stringstream out;
	env.print(out);
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/removeExisting.txt"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,hasPrefix)
{
	Config config;
	EnvSetup env(&config);
	
	EXPECT_FALSE(env.hasPrefix("/home/USER/usr"));
	env.addPrefix("/home/USER/usr");
	EXPECT_TRUE(env.hasPrefix("/home/USER/usr"));
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,loadModules)
{
	Config config;
	EnvSetup env(&config);
	config.modules.push_back("gcc");
	
	std::stringstream out;
	env.loadModules(true,out);
	EXPECT_EQ("if ! module 1>/dev/null 2>/dev/null ; then hl is-pack-installed sys-apps/modules > /dev/null && . $(hl prefix-of sys-apps/modules)/Modules/current/init/$(basename $SHELL); fi\nmodule load gcc\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,loadModulesUnload)
{
	Config config;
	EnvSetup env(&config);
	config.modules.push_back("gcc");
	
	std::stringstream out;
	env.loadModules(false,out);
	EXPECT_EQ("if ! module 1>/dev/null 2>/dev/null ; then hl is-pack-installed sys-apps/modules > /dev/null && . $(hl prefix-of sys-apps/modules)/Modules/current/init/$(basename $SHELL); fi\nmodule unload gcc\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,enableCCache)
{
	Config config;
	config.prefix.clear();
	config.prefix.push_back("/home/USER/usr");
	EnvSetup env(&config);
	
	std::stringstream out;
	env.enableCCache(out);
	EXPECT_EQ("",out.str());
	
	config.ccache = true;
	
	env.enableCCache(out);
	EXPECT_EQ("export PATH=/home/USER/usr/bin/ccache-links:$PATH\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(EnvSetup,enablePyEnv)
{
	Config config;
	config.prefix.clear();
	config.prefix.push_back("/home/USER/usr");
	config.pyEnv = false;
	EnvSetup env(&config);
	
	std::stringstream out;
	env.enablePyEnv(out);
	EXPECT_EQ("",out.str());
	
	config.pyEnv = true;
	
	env.enablePyEnv(out);
	EXPECT_EQ("export PATH=/home/USER/usr/bin/hl-py-env-bins:$PATH\n",out.str());
}
