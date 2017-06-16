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
#include <cstdio>
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

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_equal)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("=3.5","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("=3.5","3.6"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("=3.5","4.5"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_sup)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator(">3.5","3.6"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator(">3.5","3.5.1"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator(">3.5","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator(">3.5","3.4"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator(">3.5.1","3.5"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_sup_equal)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator(">=3.5","3.6"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator(">=3.5","3.5.1"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator(">=3.5","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator(">=3.5","3.4"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator(">=3.5.1","3.5"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_inf)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("<3.5","3.4"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("<3.5.1","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("<3.5","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("<3.4","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("<3.5","3.5.1"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_inf_equal)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("<=3.6","3.5"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("<=3.5.1","3.5"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("<=3.5","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("<=3.4","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("<=3.5","3.5.1"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_not)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("!3.6","3.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("!3.6","3.6"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,applyVersionOperator_regex)
{
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("~4","4"));
    EXPECT_TRUE(VersionMatcher::applyVersionOperator("~4","4.5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("~4","5"));
    EXPECT_FALSE(VersionMatcher::applyVersionOperator("~4","5.4"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,match)
{
    VersionMatcher match(">=3.5 <=3.6.5");
    EXPECT_TRUE(match.match("3.5"));
    EXPECT_TRUE(match.match("3.5.3"));
    EXPECT_TRUE(match.match("3.6.2"));
    EXPECT_FALSE(match.match("3.6.6"));
    EXPECT_FALSE(match.match("3.7"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,getSlot_1)
{
    SlotDef slots;
    slots["3"] = ">=3.0 <4";
    slots["4"] = ">=4.0 <5";
    
    EXPECT_EQ("3",VersionMatcher::getSlot(slots,"3.1"));
    EXPECT_EQ("3",VersionMatcher::getSlot(slots,"3.0"));
    EXPECT_EQ("3",VersionMatcher::getSlot(slots,"3.9.1"));
    EXPECT_EQ("4",VersionMatcher::getSlot(slots,"4.1"));
}

/*******************  FUNCTION  *********************/
TEST(VersionMatcher,getSlot_2)
{
    SlotDef slots;
    slots["~"] = "([0-9]+.[0-9]+)";
    
    EXPECT_EQ("3.1",VersionMatcher::getSlot(slots,"3.1"));
    EXPECT_EQ("3.0",VersionMatcher::getSlot(slots,"3.0"));
    EXPECT_EQ("3.9",VersionMatcher::getSlot(slots,"3.9.1"));
    EXPECT_EQ("4.1",VersionMatcher::getSlot(slots,"4.1.8.6"));
    
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
	EXPECT_DEATH(VersionMatcher::getSlot(slots,"4"),"");
}