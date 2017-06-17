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

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class HostPkgChecker
{
	public:
		HostPkgChecker(const std::string & hostType);
		bool presentOnSystem(const Json::Value & hostDef,const std::string & packageName) const;
	private:
		bool presentOnSystemGentoo(const StringList & pkgList) const;
		bool presentOnSystemCentos(const StringList & pkgList) const;
		bool presentOnSystemDebian(const StringList & pkgList) const;
		bool presentOnSystemDefault(const StringList & pkgList) const;
	private:
		std::string hostType;
};

}

#endif //HL_HOST_PKG_CHECKER_HPP
