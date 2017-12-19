/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_QUICK_PACKAGE_HPP
#define HL_QUICK_PACKAGE_HPP

/********************  HEADERS  *********************/
//std
#include <base/Helper.hpp>
#include "PackageDef.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
//to break loop deps between headers
class Prefix;

/*********************  CLASS  **********************/
class QuickPackage
{
	public:
		QuickPackage(const Prefix * prefix);
		const StringList & getQuickInfo(const std::string & property,const std::string packageName) const;
		std::string getQuickInfoFirst(const std::string & property,const std::string packageName,const std::string & defaultValue="") const;
		void genPackage(PackageDef & pack,const std::string & name) const;
	private:
		void loadQuickFile(const std::string & property,const std::string & dir);
	private:
		const Prefix * prefix;
		std::map<std::string,StringMapList> db;
};

}

#endif //HL_QUICK_PACKAGE_HPP
