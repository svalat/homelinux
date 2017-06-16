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
#include <core/VersionMatcher.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,constructor_1)
{
    VersionMatcher matcher("=2.3.1");
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,fillNumber)
{
    EXPECT_EQ("00100000",VersionMatcher::fillNumber("10",4));
    EXPECT_EQ("00010000",VersionMatcher::fillNumber("1",4));
    EXPECT_EQ("0001a000",VersionMatcher::fillNumber("1a",4));
    
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,compareVersion)
{
    EXPECT_EQ(0,VersionMatcher::compareVersion("1.35.6","1.35.6"));
    EXPECT_EQ(1,VersionMatcher::compareVersion("1.35.7","1.35.6"));
    EXPECT_EQ(-1,VersionMatcher::compareVersion("1.35.6","1.35.7"));
    EXPECT_EQ(1,VersionMatcher::compareVersion("1.45.7","1.35.6"));
    EXPECT_EQ(1,VersionMatcher::compareVersion("2.34.7","1.35.6"));
    EXPECT_EQ(1,VersionMatcher::compareVersion("1.35.6a","1.35.6"));
    EXPECT_EQ(-1,VersionMatcher::compareVersion("1.35.6a","1.35.6b"));
    EXPECT_EQ(-1,VersionMatcher::compareVersion("1.35.6a-r2","1.35.6a-r3"));
}