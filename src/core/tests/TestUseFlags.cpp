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
