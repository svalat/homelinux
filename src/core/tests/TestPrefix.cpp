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
#include <sstream>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(Prefix,constructor)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
}

/*******************  FUNCTION  *********************/
TEST(Prefix,loadPackage_ok)
{
    Config config(false);
    config.load(TEST_DATA_PATH "/user-homelinux.json");
	Prefix prefix(&config,TEST_DATA_PATH);

    PackageDef pack;
    prefix.loadPackage(pack,"hl/app-shells/bash");

    std::stringstream out;
    //pack.save(TEST_DATA_PATH "/full-get-package-bash.json");
    pack.save(out);

    EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/full-get-package-bash.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(Prefix,isInstalled_ok)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);

    EXPECT_TRUE(prefix.isInstalled("hl/app-shells/bash"));
}

/*******************  FUNCTION  *********************/
TEST(Prefix,isInstalled_not_ok)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);

    EXPECT_FALSE(prefix.isInstalled("urls/dash"));
}
