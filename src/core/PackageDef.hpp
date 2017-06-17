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

/********************  ENUM  ************************/

/********************* TYPES ************************/
typedef std::map<std::string,std::string> StringMap;
typedef std::map<std::string,std::list<std::string>> StringMapList;
typedef std::list<std::string> StringList;
typedef std::map<std::string,Json::Value> JsonMap;

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
	void load(Json::Value & json);
	void save(const sts::string & path);
	void save(std::ostream & out);
	void apply(const PackageDef & def);
	//members
	std::string name;
	std::string homepage;
	std::string inherit;
	VersionList versions;
	StringMap md5;
	StringList subdir;
	StringList deps;
	StringMapList host;
	StringMapList configure;
	JsonMap vspecofic;
	StringMapList steps;
	StringList conflicts;
	StringList use;
	std::string warn;
	std::string module;
	StringList scripts;
};

}

#endif //HL_PACKAGE_DEF_HPP
