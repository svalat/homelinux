/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cassert>
//internal
#include "DepLoader.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
DepLoader::DepLoader(Prefix * prefix)
{
	assert(prefix != NULL);
	this->prefix = prefix;
}

/*******************  FUNCTION  *********************/
DepLoader::~DepLoader(void)
{
	for (auto & it : packages)
		delete it.second;
	packages.clear();
	sched.clear();
}

/*******************  FUNCTION  *********************/
void DepLoader::loadRequest(const StringList & packageList)
{
	//clear

}

}
