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
#include <core/UseFlags.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(UseFlags,getFlagName)
{
    EXPECT_EQ("qt",UseFlags::getFlagName("qt"));
    EXPECT_EQ("qt",UseFlags::getFlagName("+qt"));
    EXPECT_EQ("qt",UseFlags::getFlagName("-qt"));
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,flagList_1)
{
    FlagList flags;
    flags << "+qt -gtk cairo  pango ";
    std::string res;
    res << flags;
    EXPECT_EQ("+qt -gtk cairo pango",res);
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,flagList_2)
{
    FlagList flags;
    flags << "+qt -gtk cairo  pango";
    std::string res;
    res << flags;
    EXPECT_EQ("+qt -gtk cairo pango",res);
}

/*******************  FUNCTION  *********************/
TEST(UserFlage,mergeString)
{
    EXPECT_EQ("gtk qt",UseFlags::mergeString("qt","gtk"));
    EXPECT_EQ("+gtk +qt",UseFlags::mergeString("qt","gtk",true));
    EXPECT_EQ("gtk qt",UseFlags::mergeString("qt gtk","gtk"));
    EXPECT_EQ("-gtk +qt",UseFlags::mergeString("+qt +gtk","-gtk"));
}
