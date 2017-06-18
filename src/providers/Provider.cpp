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
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
Provider::Provider(const std::string & name,Prefix * prefix)
{
	assert(prefix != NULL);
	this->name = name;
	this->prefix = prefix;
}

/*******************  FUNCTION  *********************/
Provider::~Provider(void)
{
	
}

/*******************  FUNCTION  *********************/
const std::string Provider::getName(void)
{
	return name;
}

}

