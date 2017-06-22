/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cerrno>
#include <cstdio>
#include <gtest/gtest.h>
#include <core/DepLoader.hpp>
#include <portability/System.hpp>
#include <sstream>

/***************** USING NAMESPACE ******************/
using namespace hl;

const char cstVal1[] = "\x1B[33m----------------------REUSE HOST--------------------------\x1B[0m\n\x1B[33m----------------------INSTALLED---------------------------\x1B[0m\n\x1B[33m----------------------TO INSTALL--------------------------\x1B[0m\n\x1B[32mhl/sys-libs/ncurses:0\x1B[0m-\x1B[35m6.0\x1B[0m USE=\"\x1B[36m+cxx\x1B[0m \x1B[34mada\x1B[0m \x1B[34mdebug\x1B[0m \x1B[34mdoc\x1B[0m \x1B[34mgpm\x1B[0m \x1B[34mminimal\x1B[0m \x1B[34mprofile\x1B[0m \x1B[34mstatic-libs\x1B[0m \x1B[34mtest\x1B[0m \x1B[34mthreads\x1B[0m \x1B[34mtinfo\x1B[0m \x1B[34mtrace\x1B[0m \x1B[34municode\x1B[0m\"\n\x1B[32mhl/app-shells/bash:0\x1B[0m-\x1B[35m4.5.20\x1B[0m [\x1B[31mreinstall\x1B[0m] USE=\"\x1B[36m+afs\x1B[0m \x1B[36m+static\x1B[0m \x1B[31m-net\x1B[0m \x1B[34mgentoo\x1B[0m \x1B[34mls\x1B[0m \x1B[34mmem-scramble\x1B[0m \x1B[34mnls\x1B[0m \x1B[34mreadline\x1B[0m\"\n\x1B[33m----------------------------------------------------------\x1B[0m\n";
const char cstVal2[] = "\x1B[33m----------------------REUSE HOST--------------------------\x1B[0m\n\x1B[33m----------------------INSTALLED---------------------------\x1B[0m\n\x1B[33m----------------------TO INSTALL--------------------------\x1B[0m\n\x1B[32mhl/sys-libs/ncurses:0\x1B[0m-\x1B[35m6.0\x1B[0m USE=\"\x1B[36m+cxx\x1B[0m \x1B[34mada\x1B[0m \x1B[34mdebug\x1B[0m \x1B[34mdoc\x1B[0m \x1B[34mgpm\x1B[0m \x1B[34mminimal\x1B[0m \x1B[34mprofile\x1B[0m \x1B[34mstatic-libs\x1B[0m \x1B[34mtest\x1B[0m \x1B[34mthreads\x1B[0m \x1B[34mtinfo\x1B[0m \x1B[34mtrace\x1B[0m \x1B[34municode\x1B[0m\"\n\x1B[32mhl/app-shells/bash:0\x1B[0m-\x1B[35m4.4.35\x1B[0m [\x1B[31mreinstall\x1B[0m] USE=\"\x1B[36m+afs\x1B[0m \x1B[36m+static\x1B[0m \x1B[31m-net\x1B[0m \x1B[34mgentoo\x1B[0m \x1B[34mls\x1B[0m \x1B[34mmem-scramble\x1B[0m \x1B[34mnls\x1B[0m \x1B[34mreadline\x1B[0m\"\n\x1B[33m----------------------------------------------------------\x1B[0m\n";

/*******************  FUNCTION  *********************/
TEST(Prefix,constructor)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
    DepLoader loader(&prefix);
}

/*******************  FUNCTION  *********************/
TEST(Prefix,printList)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
    DepLoader loader(&prefix);

    StringList lst;
    lst.push_back("bash");
    loader.loadRequest(lst);

    std::stringstream out;
    loader.printList(out);
    
    //printf("%s",cstVal1);
    EXPECT_EQ(cstVal1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(Prefix,printList_verison)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
    DepLoader loader(&prefix);

    StringList lst;
    lst.push_back("bash<=4.4.35");
    loader.loadRequest(lst);

    std::stringstream out;
    loader.printList(out);
    
    //printf("%s",cstVal1);
    EXPECT_EQ(cstVal2,out.str());
}

/*******************  FUNCTION  *********************/
TEST(Prefix,genScript)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
    DepLoader loader(&prefix);

    StringList lst;
    lst.push_back("bash=4.3.48");
    loader.loadRequest(lst);

    prefix.setPrefixForTests("/TEST/");
    config.temp = "/TMP/";

    std::stringstream out;
    loader.genScript(out,false);
    
    //printf("%s",cstVal1);
    //System::writeFile(out.str(),TEST_DATA_PATH "/deploader-full-bash.sh");
    EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/deploader-full-bash.sh"),out.str());
}

/*******************  FUNCTION  *********************/
TEST(Prefix,genParallelMakefile)
{
    Config config(false);
	Prefix prefix(&config,TEST_DATA_PATH);
    DepLoader loader(&prefix);

    StringList lst;
    lst.push_back("bash=4.3.48");
    loader.loadRequest(lst);

    prefix.setPrefixForTests("/TEST/");
    config.temp = "/TMP/";

    std::stringstream out;
    loader.genParallelMakefile(out,"/TMP/work-1234/");
    
    //printf("%s",cstVal1);
    //System::writeFile(out.str(),TEST_DATA_PATH "/deploader-full-bash-p-makefile.sh");
    EXPECT_EQ(System::loadFile(TEST_DATA_PATH "/deploader-full-bash-p-makefile.sh"),out.str());
}