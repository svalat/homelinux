/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PACKAGE_DEF_HPP
#define HL_PACKAGE_DEF_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <map>
//from jsoncpp
#include <json/json.h>
//intenral
#include "VersionMatcher.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
struct VersionFetcher
{
	std::string mode;
	std::string url;
	std::string regexp;
};

/********************  STRUCT  **********************/
struct PackageDef
{
	//funcs
	void load(const std::string & path);
	void loadJson(const Json::Value & json);
	void save(const std::string & path);
	void save(std::ostream & out);
	void merge(const PackageDef & def);
	void save(Json::Value & json);
	//members
	std::string name;
	std::string homepage;
	std::string inherit;
	VersionList versions;
	VersionFetcher vfetcher;
	StringMap md5;
	std::string subdir;
	StringList deps;
	Json::Value host;
	StringMapList configure;
	JsonMap vspecific;
	StringMapList steps;
	StringList conflicts;
	StringList use;
	StringList warn;
	std::string module;
	StringList scripts;
	StringMap vars;
	StringMapList flags;
	StringList urls;
	StringList patch;
};

}

#endif //HL_PACKAGE_DEF_HPP
