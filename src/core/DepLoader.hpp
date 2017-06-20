/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_DEP_LOADER_HPP
#define HL_DEP_LOADER_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <vector>
//internal
#include "PackageDef.hpp"
#include "Prefix.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************* TYPES ************************/
typedef std::map<std::string,PackageDef *> PackageMap;
typedef std::list<PackageDef *> PackageList;

/*********************  CLASS  **********************/
/**
 * The dep loader class is responsible to load all the packages
 * needed to build the requested list of root packages.
 * It build a tree which is cut by checking the available packages
 * on the host system, then build job list to build all the packages.
 * It also apply some version filtering depending on the requirement
 * of parent packages.
**/
class DepLoader
{
	public:
		DepLoader(Prefix * prefix);
		~DepLoader(void);
		void loadRequest(const StringList & packageList);
	private:
		Prefix * prefix;
		PackageMap packages;
		PackageList sched;
		PackageList root;
};
	
}

#endif //HL_DEP_LOADER_HPP
