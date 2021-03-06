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
#include <base/Helper.hpp>
#include <json/json.h>
#include "../System.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(System,getEnv)
{
	EXPECT_EQ("DEFAULT",System::getEnv("Abacadabra","DEFAULT"));
	EXPECT_NE("DEFAULT",System::getEnv("HOME","DEFAULT"));
}

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

/*******************  FUNCTION  *********************/
TEST(System,loadFile)
{
	EXPECT_EQ("This is a test !",System::loadFile(TEST_DATA_PATH "/test.txt"));
}

/*******************  FUNCTION  *********************/
TEST(System,findFiles)
{
	std::map<std::string,std::string> ref;
	ref["TestSystem.cpp"] = "TestSystem.cpp";
	ref["TestRegexp.cpp"] = "TestRegexp.cpp";
	ref["CMakeLists.txt"] = "CMakeLists.txt";
	ref["data/test.txt"] = "data/test.txt";
	ref["data/test.json"] = "data/test.json";
	
	StringList lst = System::findFiles(TEST_DATA_PATH "/..");
	forEach(StringList,value,lst)
		EXPECT_EQ(ref[*value],*value);
}

/*******************  FUNCTION  *********************/
TEST(System,loadJson)
{
	Json::Value json;
	System::loadJson(json,TEST_DATA_PATH "/test.json");
	
	EXPECT_EQ("Hello",json["test"].asString());
}

/*******************  FUNCTION  *********************/
TEST(System,writeJson)
{
	Json::Value json;
	json["test"] = "Hello";
	System::writeJson(json,"/tmp/hl-system-test-save-json.json");
	
	Json::Value json2;
	System::loadJson(json2,TEST_DATA_PATH "/test.json");
	EXPECT_EQ("Hello",json2["test"].asString());
}

/*******************  FUNCTION  *********************/
TEST(System,downloadJson)
{
	Json::Value json;
	EXPECT_TRUE(System::downloadJson(json,"https://api.github.com/repos/svalat/homelinux/releases/latest"));
	EXPECT_EQ("svalat",json["author"]["login"].asString());
}
