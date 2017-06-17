/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <base/Debug.hpp>
#include <base/Helper.hpp>
#include <portability/System.hpp>
#include "HostPkgChecker.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
HostPkgChecker::HostPkgChecker(const std::string & hostType)
{
	this->hostType = hostType;
}

/*******************  FUNCTION  *********************/
bool HostPkgChecker::presentOnSystem(const Json::Value & hostDef,const std::string & packageName) const
{
	//vars
	StringList pkgList;
	const Json::Value & entry = hostDef[hostType];

	//trivia
	if (entry.isNull())
		return false;
	if (entry.isBool())
		return entry.asBool();
	
	//extract list
	if (entry.isArray())
		Helper::jsonToObj(pkgList,entry);
	
	//if empty
	if (entry.empty())
		return false;
	
	//select type
	if (hostType == "default")
		return presentOnSystemDefault(pkgList);
	else if (hostType == "centos7")
		return presentOnSystemCentos(pkgList);
	else if (hostType == "debian8")
		return presentOnSystemDebian(pkgList);
	else if (hostType == "gentoo")
		return presentOnSystemGentoo(pkgList);
	else
		HL_FATAL_ARG("Unsupported host type : %1").arg(hostType).end();
}

/*******************  FUNCTION  *********************/
bool HostPkgChecker::presentOnSystemGentoo(const StringList & pkgList) const
{
	//loop
	for (auto & pack : pkgList)
	{
		//default
		bool found = false;
		
		//split
		StringVector names(2);
		Helper::stringSplit(pack,'/',[&names](const std::string & value){
			names.push_back(value);
		});
		
		//list dir
		System::readDir("/var/db/pkg/"+names[0],[&found,&names](const std::string & value){
			if (Helper::startBy(value,names[1]))
				found = true;
		});
		
		//check
		if (found == false)
			return false;
	}
	
	//ok
	return true;
}

/*******************  FUNCTION  *********************/
bool HostPkgChecker::presentOnSystemCentos(const StringList & pkgList) const
{
	//loop
	for (auto & pack : pkgList)
	{
		if (System::runCommand("rpm -V "+pack+" 2>/deb/null") != 0)
			return false;
	}
	
	//ok
	return true;
}

/*******************  FUNCTION  *********************/
bool HostPkgChecker::presentOnSystemDebian(const StringList & pkgList) const
{
	//loop
	for (auto & pack : pkgList)
	{
		if (System::runCommand("dpkg -s "+pack+" 2>/deb/null") != 0)
			return false;
	}
	
	//ok
	return true;
}

/*******************  FUNCTION  *********************/
bool HostPkgChecker::presentOnSystemDefault(const StringList & pkgList) const
{
	return false;
}

}

