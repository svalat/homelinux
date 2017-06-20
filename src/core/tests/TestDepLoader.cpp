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
#include <core/DepLoader.hpp>
#include <portability/System.hpp>
#include <sstream>

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(Prefix,constructor)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
    DepLoader loader(&prefix);
}
