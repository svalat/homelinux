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
TEST(UseFlags,constructor_1)
{
    UseFlags flags;
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,constructor_2)
{
    UseFlags flags("qt gtk");
    EXPECT_EQ("gtk qt",flags.toString());
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,getFlagName)
{
    EXPECT_EQ("qt",UseFlags::getFlagName("qt"));
    EXPECT_EQ("qt",UseFlags::getFlagName("+qt"));
    EXPECT_EQ("qt",UseFlags::getFlagName("-qt"));
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,merge)
{
    UseFlags flags;
    flags.merge("-gtk +qt  svg ");
    flags.merge("kde");
    EXPECT_EQ("+qt -gtk kde svg",flags.toString());
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,toString)
{
    UseFlags flags("-gtk +qt  svg ");
    EXPECT_EQ("+qt -gtk svg",flags.toString());
    EXPECT_EQ("+qt +svg -gtk",flags.toString(true));
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,getStatus)
{
    UseFlags flags("-gtk +qt  svg ");
    EXPECT_EQ(FLAG_ENABLED,flags.getStatus("qt"));
    EXPECT_EQ(FLAG_DISABLED,flags.getStatus("gtk"));
    EXPECT_EQ(FLAG_AUTO,flags.getStatus("svg"));
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,getApplyStatus)
{
    UseFlags flags("+qt -gtk svg");
    
    EXPECT_EQ(FLAG_ENABLED,flags.getApplyStatus("qt"));
    EXPECT_EQ(FLAG_ENABLED,flags.getApplyStatus("+qt"));
    EXPECT_EQ(FLAG_DISABLED,flags.getApplyStatus("-qt"));
    
    EXPECT_EQ(FLAG_DISABLED,flags.getApplyStatus("gtk"));
    EXPECT_EQ(FLAG_DISABLED,flags.getApplyStatus("+gtk"));
    EXPECT_EQ(FLAG_ENABLED,flags.getApplyStatus("-gtk"));
    
    EXPECT_EQ(FLAG_AUTO,flags.getApplyStatus("svg"));
    EXPECT_EQ(FLAG_DISABLED,flags.getApplyStatus("+svg"));
    EXPECT_EQ(FLAG_DISABLED,flags.getApplyStatus("-svg"));
}

/*******************  FUNCTION  *********************/
TEST(UseFlags,getApplyStatusWithAnd)
{
    UseFlags flags("+qt +kde -gtk svg");
    
    EXPECT_EQ(FLAG_ENABLED,flags.getApplyStatusWithAnd("+qt&-gtk"));
    EXPECT_EQ(FLAG_DISABLED,flags.getApplyStatusWithAnd("+qt&+gtk"));
    EXPECT_EQ(FLAG_ENABLED,flags.getApplyStatusWithAnd("qt&kde"));
    EXPECT_EQ(FLAG_AUTO,flags.getApplyStatusWithAnd("qt&svg"));
}
