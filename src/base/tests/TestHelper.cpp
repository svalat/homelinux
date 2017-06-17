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

/*******************  FUNCTION  *********************/
TEST(Helper,jsonToObj_StringList)
{
	Json::Value json;
	json.append("a");
	json.append("b");
	
	StringList lst;
	Helper::jsonToObj(lst,json);
	
	EXPECT_EQ("a",Helper::getListEl(lst,0));
	EXPECT_EQ("b",Helper::getListEl(lst,1));
}

/*******************  FUNCTION  *********************/
TEST(Helper,jsonToObj_StringMap)
{
	Json::Value json;
	json["a"] = "aa";
	json["b"] = "bb";
	
	StringMap m;
	Helper::jsonToObj(m,json);
	
	EXPECT_EQ("aa",m["a"]);
	EXPECT_EQ("bb",m["b"]);
}

/*******************  FUNCTION  *********************/
TEST(Helper,jsonToObj_StringMapList)
{
	Json::Value json;
	json["a"].append("aa1");
	json["a"].append("aa2");
	json["b"].append("bb1");
	json["b"].append("bb2");
	
	StringMapList m;
	Helper::jsonToObj(m,json);
	
	EXPECT_EQ("aa1",Helper::getListEl(m["a"],0));
	EXPECT_EQ("aa2",Helper::getListEl(m["a"],1));
	EXPECT_EQ("bb1",Helper::getListEl(m["b"],0));
	EXPECT_EQ("bb2",Helper::getListEl(m["b"],1));
}

/*******************  FUNCTION  *********************/
TEST(Helper,toJson_StringList)
{
	Json::Value json;
	
	StringList lst;
	lst.push_back("a");
	lst.push_back("b");
	
	Helper::toJson(json,lst);
	
	std::stringstream out;
	out << json;
	
	EXPECT_EQ("[ \"a\", \"b\" ]\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(Helper,toJson_StringMap)
{
	Json::Value json;
	
	StringMap lst;
	lst["a"] = "aa";
	lst["b"] = "bb";
	
	Helper::toJson(json,lst);
	
	std::stringstream out;
	out << json;
	
	EXPECT_EQ("\n{\n\t\"a\" : \"aa\",\n\t\"b\" : \"bb\"\n}\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(Helper,toJson_StringMapList)
{
	Json::Value json;
	
	StringMapList lst;
	lst["a"].push_back("aa1");
	lst["a"].push_back("aa2");
	lst["b"].push_back("bb1");
	lst["b"].push_back("bb2");
	
	Helper::toJson(json,lst);
	
	std::stringstream out;
	out << json;
	
	EXPECT_EQ("\n{\n\t\"a\" : [ \"aa1\", \"aa2\" ],\n\t\"b\" : [ \"bb1\", \"bb2\" ]\n}\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(Helper,merge_StringList_1)
{
	StringList l1;
	l1.push_back("a");
	l1.push_back("b");
	
	StringList l2;
	l2.push_back("c");
	l2.push_back("d");
	
	Helper::merge(l1,l2);
	
	EXPECT_EQ("a",Helper::getListEl(l1,0));
	EXPECT_EQ("b",Helper::getListEl(l1,1));
	EXPECT_EQ("c",Helper::getListEl(l1,2));
	EXPECT_EQ("d",Helper::getListEl(l1,3));
}

/*******************  FUNCTION  *********************/
TEST(Helper,merge_StringList_2)
{
	StringList l1;
	l1.push_back("a");
	l1.push_back("b");
	
	StringList l2;
	l2.push_back("c");
	l2.push_back("d");
	l2.push_back("!a");
	
	Helper::merge(l1,l2);
	
	EXPECT_EQ("b",Helper::getListEl(l1,0));
	EXPECT_EQ("c",Helper::getListEl(l1,1));
	EXPECT_EQ("d",Helper::getListEl(l1,2));
}
