/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cstdlib>
#include <unistd.h>
#include "System.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
std::string System::getHomeDir(void)
{
    return getenv("HOME");
}

/*******************  FUNCTION  *********************/
bool System::fileExist(const std::string & path)
{
    return ( access( path.c_str(), F_OK ) != -1 );
}

}
