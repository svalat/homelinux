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
#include <base/Debug.hpp>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(DEBUG,debug)
{
	HL_DEBUG("test","test debug");
	HL_DEBUG_ARG("test","test debug %1").arg("value").end();
}

/*******************  FUNCTION  *********************/
TEST(DEBUG,debugCat)
{
	Debug::showCat("test");
	HL_DEBUG("test","test debug");
	HL_DEBUG_ARG("test","test debug %1").arg("value").end();
	Debug::showList();
	EXPECT_NE((void*)NULL,(void*)Debug::getCatMap());
}

/*******************  FUNCTION  *********************/
TEST(DEBUG,warn)
{
	HL_WARNING("test debug");
	HL_WARNING_ARG("test debug %1").arg("value").end();
}

/*******************  FUNCTION  *********************/
TEST(DEBUG,error)
{
	HL_ERROR("test debug");
	HL_ERROR_ARG("test debug %1").arg("value").end();
}

/*******************  FUNCTION  *********************/
TEST(DEBUG,fatal)
{
	::testing::FLAGS_gtest_death_test_style = "threadsafe";
	ASSERT_DEATH(HL_FATAL("test debug"),"");
	ASSERT_DEATH(HL_FATAL_ARG("test debug %1").arg("value").end(),"");
}
