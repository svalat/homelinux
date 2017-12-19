/*****************************************************
			PROJECT  : homelinux
			VERSION  : 2.0.0
			DATE     : 06/2017
			AUTHOR   : Valat Sébastien
			LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../Regexp.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_1)
{
	Regexp reg("[A-Z]+\\.[0-9]+.*");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_2)
{
	Regexp reg("([0-9a-zA-Z+._/&-]+)?");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_3)
{
	Regexp reg("([ @]?.+)?");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_4)
{
	Regexp reg("([0-9a-zA-Z_/-]+)");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_5)
{
	Regexp reg("(\\[[0-9A-Za-z#_+,-]+\\])?");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_6)
{
	Regexp reg("(\\[[0-9A-Za-z#_+,-]+\\])?([ @]?.+)?");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,constructor_7)
{
	Regexp reg("([a-zA-Z0-9&._+-]+)\\? ([0-9a-zA-Z_/-]+)(\\[[0-9A-Za-z#_+,-]+\\])?([ @]?.+)?");
}

/*******************  FUNCTION  *********************/
TEST(Regexp,match_1)
{
	Regexp reg("[0-9]+\\.[0-9]+");

	EXPECT_TRUE(reg.match("0.8"));
	EXPECT_FALSE(reg.match("a.8"));
	EXPECT_FALSE(reg.match("a0.8"));
}

/*******************  FUNCTION  *********************/
TEST(Regexp,match_2)
{
	Regexp reg("libxml2-([0-9]+\\.[0-9]+)\\.tar");

	std::string out;
	EXPECT_TRUE(reg.match("libxml2-0.8.tar",out));
	EXPECT_EQ("0.8",out);
}

/*******************  FUNCTION  *********************/
TEST(Regexp,match_3)
{
	Regexp reg("([A-Za-z0-9]+)-([0-9]+\\.[0-9]+)\\.tar");

	std::string out1;
	std::string out2;
	EXPECT_TRUE(reg.match("libxml2-0.8.tar",out1,out2));
	EXPECT_EQ("libxml2",out1);
	EXPECT_EQ("0.8",out2);
}