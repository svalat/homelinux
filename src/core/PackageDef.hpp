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
	void save(const std::string & path);
	void save(std::ostream & out);
	void merge(const PackageDef & def);
	void save(Json::Value & json);
	//helpers
	static void jsonToObj(StringList & out,const Json::Value & json);
	static void jsonToObj(StringMap & out,const Json::Value & json);
	static void jsonToObj(StringMapList & out, const Json::Value & json);
	static void jsonToObj(JsonMap & out,const Json::Value & json);
	static void toJson(Json::Value & out,const StringList & list);
	static void toJson(Json::Value & out,const StringMap & map);
	static void toJson(Json::Value & out,const StringMapList & mapList);
	static void toJson(Json::Value & out,const JsonMap & map);
	static void merge(StringMap & out,const StringMap & override);
	static void merge(StringList & out,const StringList & override);
	static void merge(StringMapList & out,const StringMapList & override,bool erase);
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
};

}

#endif //HL_PACKAGE_DEF_HPP
