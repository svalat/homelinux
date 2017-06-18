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
TEST(System,findFiels)
{
	std::map<std::string,std::string> ref;
	ref["TestSystem.cpp"] = "TestSystem.cpp";
	ref["CMakeLists.txt"] = "CMakeLists.txt";
	ref["data/test.txt"] = "data/test.txt";
	ref["data/test.json"] = "data/test.json";
	
	System::findFiles(TEST_DATA_PATH "/..",[&](const std::string & value){
		EXPECT_EQ(ref[value],value);
	});
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
