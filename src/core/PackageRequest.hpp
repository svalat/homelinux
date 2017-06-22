/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PACKAGE_REQUEST_HPP
#define HL_PACKAGE_REQUEST_HPP

/********************  HEADERS  *********************/
//std
#include <string>
//internal
#include "PackageDef.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class DepPackage;

/*********************  STRUCT  *********************/
struct PackageRequest
{
	//funcs
	PackageRequest(const std::string & value,DepPackage * parent = NULL);
	PackageRequest(const PackageRequest & req);
	PackageRequest(void);
	//vars
	std::string name;
	std::string use;
	std::string iuse;
	std::string version;
	DepPackage * parent;
};
	
}

#endif //HL_PACKAGE_REQUEST_HPP
