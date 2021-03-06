/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
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
/**
 * Constructor.
 * @param hostType Define the type of host to consider (gentoo, centos7, 
 * debian8, default, true or false)
**/
HostPkgChecker::HostPkgChecker(const std::string & hostType)
{
	this->hostType = hostType;
}

/*******************  FUNCTION  *********************/
/**
 * Check if the given packages are already installed on the host system.
 * @param hostDef Host field from package def containling the list of packages 
 * names to check.
**/
bool HostPkgChecker::presentOnSystem(const Json::Value & hostDef) const
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

	HL_DEBUG_ARG("HostPkgChecker","Check dep entry : %1").arg(Helper::join(pkgList,' ')).end();
	
	//if empty
	if (entry.empty())
		return false;
	
	//select type
	if (hostType == "true")
		return true;
	else if (hostType == "false")
		return false;
	else if (hostType == "default")
		return presentOnSystemDefault(pkgList);
	else if (hostType == "centos7")
		return presentOnSystemCentos(pkgList);
	else if (hostType == "debian8")
		return presentOnSystemDebian(pkgList);
	else if (hostType == "gentoo")
		return presentOnSystemGentoo(pkgList);
	else
		HL_FATAL_ARG("Unsupported host type : %1").arg(hostType).end();
	
	//default to avoid warning at compile time
	return false;
}

/*******************  FUNCTION  *********************/
/**
 * Check package on a Gentoo host system.
 * @param hostDef Host field from package def containling the list of packages 
 * names to check.
**/
bool HostPkgChecker::presentOnSystemGentoo(const StringList & pkgList) const
{
	//loop
	//for (auto & pack : pkgList)
	forEachConst(StringList,pack,pkgList)
	{
		//default
		bool found = false;
		
		//split
		StringVector names(2);
		StringList lst = Helper::split(*pack,'/');
		forEach(StringList,value,lst)
			names.push_back(*value);
		
		//list dir
		lst = System::readDir("/var/db/pkg/"+names[0]);
		forEach(StringList,value,lst)
			if (Helper::startBy(*value,names[1]))
				found = true;
		
		//check
		if (found == false)
			return false;
	}
	
	//ok
	return true;
}

/*******************  FUNCTION  *********************/
/**
 * Check package on a Centos system.
 * @param hostDef Host field from package def containling the list of packages 
 * names to check.
**/
bool HostPkgChecker::presentOnSystemCentos(const StringList & pkgList) const
{
	//loop
	//for (auto & pack : pkgList)
	forEachConst(StringList,pack,pkgList)
	{
		#ifndef NDEBUG
			if (System::runCommand("yum list "+*pack+" 2>/dev/null >/dev/null") != 0)
				HL_WARNING_ARG("Package %1 does not exist on yum !").arg(*pack).end();
		#endif
		if (System::runCommand("rpm -V "+*pack+" 2>/dev/null > /dev/null") != 0)
			return false;
	}
	
	//ok
	return true;
}

/*******************  FUNCTION  *********************/
/**
 * Check package on a Debian system.
 * @param hostDef Host field from package def containling the list of packages 
 * names to check.
**/
bool HostPkgChecker::presentOnSystemDebian(const StringList & pkgList) const
{
	//loop
	//for (auto & pack : pkgList)
	forEachConst(StringList,pack,pkgList)
	{
		if (System::runCommand("dpkg -s "+*pack+" 2>/deb/null") != 0)
			return false;
	}
	
	//ok
	return true;
}

/*******************  FUNCTION  *********************/
/**
 * Default is a non system checking it only rely on default values defined
 * by packages and a user database.
**/
bool HostPkgChecker::presentOnSystemDefault(const StringList & pkgList) const
{
	return false;
		
}

}

