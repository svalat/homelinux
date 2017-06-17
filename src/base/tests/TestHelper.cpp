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
#include <base/Helper.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(Helper,stringSplit_1)
{
	std::string res;
	Helper::stringSplit("a1 a2  a3",' ',[&res](const std::string & value){res += "(";res+=value;res+=")";});
	EXPECT_EQ("(a1)(a2)(a3)",res);
}

/*******************  FUNCTION  *********************/
TEST(Helper,stringSplit_2)
{
	std::string res;
	Helper::stringSplit("a1 a2  a3   ",' ',[&res](const std::string & value){res += "(";res+=value;res+=")";});
	EXPECT_EQ("(a1)(a2)(a3)",res);
}

/*******************  FUNCTION  *********************/
TEST(Helper,contain)
{
	EXPECT_TRUE(Helper::contain("Hello world !","world"));
	EXPECT_FALSE(Helper::contain("Hello world !","homelinux"));
}

/*******************  FUNCTION  *********************/
TEST(Helper,join)
{
	std::list<std::string> lst;
	lst.push_back("Hello");
	lst.push_back("world");
	lst.push_back("!");
	
	EXPECT_EQ("Hello world !",Helper::join(lst,' '));
}
