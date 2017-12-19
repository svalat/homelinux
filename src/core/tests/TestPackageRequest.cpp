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
#include <core/PackageRequest.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(PackageRequest,constructor_1)
{
    PackageRequest req("dev-vcs/git");
    EXPECT_EQ("dev-vcs/git",req.name);
    EXPECT_EQ("",req.use);
    EXPECT_EQ("",req.iuse);
    EXPECT_EQ("",req.version);
}

/*******************  FUNCTION  *********************/
TEST(PackageRequest,constructor_2)
{
    PackageRequest req("dev-vcs/git[+static,-qt]");
    EXPECT_EQ("dev-vcs/git",req.name);
    EXPECT_EQ("+static,-qt",req.iuse);
    EXPECT_EQ("",req.use);
    EXPECT_EQ("",req.version);
}

/*******************  FUNCTION  *********************/
TEST(PackageRequest,constructor_3)
{
    PackageRequest req("dev-vcs/git <=2.5 >=1.2");
    EXPECT_EQ("dev-vcs/git",req.name);
    EXPECT_EQ("",req.iuse);
    EXPECT_EQ("",req.use);
    EXPECT_EQ(" <=2.5 >=1.2",req.version);
}

/*******************  FUNCTION  *********************/
TEST(PackageRequest,constructor_4)
{
    PackageRequest req("dev-vcs/git ~2.5");
    EXPECT_EQ("dev-vcs/git",req.name);
    EXPECT_EQ("",req.use);
    EXPECT_EQ("",req.iuse);
    EXPECT_EQ(" ~2.5",req.version);
}

/*******************  FUNCTION  *********************/
TEST(PackageRequest,constructor_5)
{
    PackageRequest req("dev-vcs/git !2.5");
    EXPECT_EQ("dev-vcs/git",req.name);
    EXPECT_EQ("",req.use);
    EXPECT_EQ("",req.iuse);
    EXPECT_EQ(" !2.5",req.version);
}

/*******************  FUNCTION  *********************/
TEST(PackageRequest,constructor_6)
{
    PackageRequest req("+qt&-kde? dev-vcs/git");
    EXPECT_EQ("dev-vcs/git",req.name);
    EXPECT_EQ("",req.iuse);
    EXPECT_EQ("+qt&-kde",req.use);
    EXPECT_EQ("",req.version);
}
