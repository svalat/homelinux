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
#include "PackageRequest.hpp"
#include "PackageDef.hpp"
#include "Prefix.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class DepPackage;

/********************  TYPES  ***********************/
typedef std::map<std::string,PackageRequest> PackageRequestMap;
typedef std::list<DepPackage *> PackageList;
typedef std::map<std::string,DepPackage *> PackageMap;

/********************  STRUCT  **********************/
/**
 * Some infos used by deploader
**/
struct DepLoaderInfos
{
	DepLoaderInfos(void);
	int loopChecker;
	bool force;
	PackageMap deps;
	std::string present;
};

/********************  STRUCT  **********************/
/**
 * To aggregate all required infos in one struct.
**/
struct DepPackage
{
	PackageDef def;
	DepLoaderInfos infos;
	PackageRequestMap hints;
};

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
		void printList(std::ostream & out);
		void genScript(std::ostream & out,bool usePinstall);
		void genParallelMakefile(std::ostream & out,const std::string & tmpdir);
		void genParallelScripts(const std::string & tmpdir);
	private:
		void applyVSpecific(void);
		void applyVSpecific(DepPackage * pack);
		void applyVersionRules(DepPackage * pack,DepPackage * parent,PackageRequest & infos);
		void loadPackageDeps(DepPackage * pack);
		void checkStatus(void);
		void buildSched(void);
		void buildSchedChild(DepPackage * pack);
		void checkUseFlagHints(DepPackage * pack);
		void applyVersionHints(DepPackage * pack);
		void selectVSpecific(DepPackage * pack);
		std::string replaceParentUseFlags(const std::string,const DepPackage * parent);
		DepPackage * loadPackage(const std::string & request,DepPackage * parent,bool force);
	private:
		/** Keep access to the prefix to know how to locate the files **/
		Prefix * prefix;
		/** Map of packages already loaded to not load them again **/
		PackageMap packages;
		/** Scheduling gneerated meaning list of packages in order to install. **/
		StringList sched;
		/** List of packages epxlicitly required by user **/
		PackageList root;
};
	
}

#endif //HL_DEP_LOADER_HPP
