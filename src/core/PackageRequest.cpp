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
//extern
#include <re2/re2.h>
//internal
#include "HostPkgChecker.hpp"
#include "DepLoader.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
/**
 * Parse the question string and then convert it into a more usable
 * struct for the rest of the app.
 * @param value Define the string to parse. The format can be on of those
 * examples :
 *    - git : Only simple name
 *    - dev-vcs/git : Name with source (can be gentoo/, urls/....)
 *    - dev-vcs/git[+debug,-gzip] : Force some useflags on the package
 *    - dev-vcs/git <3.5 >2.5 : version hints
 *    - dev-vcs/git =3.5.6 : Force specific vesrion
 *    - dev-vcs/git :3 : Define the slot to be used.
 *    - dev-vcs/git ~3.[0-9]+ : with regexp
 *    - git? dev-vcs/git : Only if has git flag
 *
 * You can see doc directory to get more infos on the format.
 * @param parent Pointer to the parent package
**/
PackageRequest::PackageRequest(const std::string & value,DepPackage * parent)
{
	//setup some regexp to be reused
	static RE2 regexp1("([0-9a-zA-Z+._/&-]+)(\\[[0-9A-Za-z#_+,-]+\\])?([ @].+)?");
	static RE2 regexp2("([a-zA-Z0-9-&._+-]+)\\? ([0-9a-zA-Z_/-]+)(\\[[0-9A-Za-z#_+,-]+\\])?([ @].+)?");

	//check if has ? for package dep depending on flags
	if (Helper::contain(value,"?"))
	{
		assumeArg(RE2::FullMatch(value,regexp2,&use,&name,&iuse,&version),"Invalid format : %1")
			.arg(value)
			.end();
	} else {
		assumeArg(RE2::FullMatch(value,regexp1,&name,&iuse,&version),"Invalid format : %1")
			.arg(value)
			.end();
	}

	//replace []
	Helper::replaceInPlace(iuse,"[","");
	Helper::replaceInPlace(iuse,"]","");

	//setup
	this->parent = parent;
}

/*******************  FUNCTION  *********************/
PackageRequest::PackageRequest(void)
{
	parent = NULL;
}

/*******************  FUNCTION  *********************/
PackageRequest::PackageRequest(const PackageRequest & req)
{
	name = req.name;
	use = req.use;
	iuse = req.iuse;
	version = req.version;
	parent = req.parent;
}

}
