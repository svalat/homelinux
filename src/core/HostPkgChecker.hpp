/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_HOST_PKG_CHECKER_HPP
#define HL_HOST_PKG_CHECKER_HPP

/********************  HEADERS  *********************/
//std
#include <string>
//from jsoncpp
#include <json/json.h>
//insternal
#include <base/Helper.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
/**
 * Check if packages are already installed into the host system.
 * This class manage multiple system which are selected by the config file.
**/
class HostPkgChecker
{
	public:
		HostPkgChecker(const std::string & hostType);
		bool presentOnSystem(const Json::Value & hostDef) const;
	private:
		bool presentOnSystemGentoo(const StringList & pkgList) const;
		bool presentOnSystemCentos(const StringList & pkgList) const;
		bool presentOnSystemDebian(const StringList & pkgList) const;
		bool presentOnSystemDefault(const StringList & pkgList) const;
	private:
		/** Type of host to consider **/
		std::string hostType;
};

}

#endif //HL_HOST_PKG_CHECKER_HPP
