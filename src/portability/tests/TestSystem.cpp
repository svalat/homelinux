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
#include "../System.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(System,getHomeDir)
{
    std::string home = System::getHomeDir();
    EXPECT_NE("",home);
}

/*******************  FUNCTION  *********************/
TEST(System,fileExist)
{
    EXPECT_TRUE(System::fileExist(TEST_DATA_PATH "/test.txt"));
    EXPECT_FALSE(System::fileExist(TEST_DATA_PATH "/no.txt"));
}