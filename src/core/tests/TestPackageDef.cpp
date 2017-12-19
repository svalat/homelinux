/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cerrno>
#include <sstream>
#include <gtest/gtest.h>
#include <base/Helper.hpp>
#include <portability/System.hpp>
#include <core/Prefix.hpp>
#include "../PackageDef.hpp"

/***************** USING NAMESPACE ******************/
using namespace hl;

/*******************  FUNCTION  *********************/
TEST(PackageDef,constructor)
{
	PackageDef pack;
}

/*******************  FUNCTION  *********************/
TEST(PackageDef,load_and_save)
{
	PackageDef def;
	def.load(TEST_DATA_PATH "/spec.json");
	
	ASSERT_EQ("app-shell/bash",def.name);
	
	std::stringstream out;
	def.save(out);
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/spec.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(PackageDef,merge)
{
	PackageDef def;
	def.load(TEST_DATA_PATH "/spec.json");
	
	ASSERT_EQ("app-shell/bash",def.name);
	
	PackageDef child;
	child.load(TEST_DATA_PATH "/spec-inherit.json");
	def.merge(child);
	
	std::stringstream out;
	def.save(out);
	//System::writeFile(out.str(),TEST_DATA_PATH "/spec-merged.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/spec-merged.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(PackageDef,applySpecific)
{
	PackageDef def;
	def.load(TEST_DATA_PATH "/spec.json");
	
	ASSERT_EQ("app-shell/bash",def.name);
	
	PackageDef specific;
	specific.loadJson(def.vspecific["4.2"]);
	
	def.merge(specific);
	
	std::stringstream out;
	def.save(out);
	//System::writeFile(out.str(),TEST_DATA_PATH "/spec-specific.json");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/spec-specific.json"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(PackageDef,genScript)
{
	Config config;
	Prefix prefix(&config,TEST_DATA_PATH);

	//get package
	PackageDef pack;
	prefix.loadPackage(pack,"hl/app-shells/bash");

	//avoid to have paths changing between users
	prefix.setPrefixForTests("/MY_PREFIX/");
	config.temp = "/MY_TMP/";

	//convert
	std::stringstream out;
	pack.genScript(out,prefix,false);

	//System::writeFile(out.str(),TEST_DATA_PATH "/gen-pack-bash-no-parallel.sh");
	EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/gen-pack-bash-no-parallel.sh"),out.str());
}

