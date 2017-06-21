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
#include <list>
//from jsoncpp
#include <json/json.h>
//intenral
#include <base/Helper.hpp>
#include "UseFlags.hpp"
#include "VersionMatcher.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  MACROS  **********************/
#define HL_JHSON_API 1

/********************  CLASS  ***********************/
//to break header deps
class Prefix;

/********************  STRUCT  **********************/
/**
 * Parameters for version fetching
**/
struct VersionFetcher
{
	/**
	 * Fetcher mode, can be `http`, `ftp`, `github`, `http-gnome-cache`.
	**/
	std::string mode;
	/**
	 * Url to use to crawl.
	**/
	std::string url;
	/**
	 * Regexp to use in the page to extract package versions.
	**/
	std::string regexp;
};

/********************  STRUCT  **********************/
/**
 * Package definition containing all the information needed to build a package.
 * It also mange inheritance by providing function to merge content between
 * packages.
**/
struct PackageDef
{
	//funcs
	PackageDef(void);
	void load(const std::string & path);
	void loadJson(const Json::Value & json);
	void save(const std::string & path);
	void save(std::ostream & out);
	void merge(const PackageDef & def);
	void save(Json::Value & json);
	void sortVersions(void);
	std::string getVersion(void) const;
	std::string getSlot(void) const;
	std::string getNVersions(int cnt) const;
	std::string getSlotName(void) const;
	std::string getStowName(void) const;
	std::string getShortName(void) const;
	std::string getShortVersion(void) const;
	std::string getRealPrefix(const std::string & prefix,bool stow) const;
	std::string getBuildOptions(void) const;
	std::string getPackInstalled(const std::string & prefix) const;
	void genScript(std::ostream & out,const Prefix & prefix,bool parallelInstall);
	//members
	/** API in case of update **/
	int api;
	/** Name of the package. **/
	std::string name;
	/** Home page of the project **/
	std::string homepage;
	/** Inherit from given package **/
	std::string inherit;
	/** List of version available for this package **/
	VersionList versions;
	/** Information, to crawl versions **/
	VersionFetcher vfetcher;
	/** Md5 sums to check files **/
	StringMap md5;
	/**  Subdir in archive **/
	std::string subdir;
	/** 
	 * List of deps. It uses a special format possibly filtering the deps.
	 * `+flag? depPackageName:SLOT [+flag1,-flag2] <=3.5 >=2.4 !2.4.4`
	**/
	StringList deps;
	/** Name of this package on various hosts to check if present on system **/
	Json::Value host;
	/**
	 * Map of configure arguemnt to use on configure script. Tha key represent
	 * a filter based on flags and the values are list of string giving the list
	 * of options to be used.
	**/
	StringMapList configure;
	/**
	 * The values of this map can contain all entries to ovveride in package
	 * it is applied only if the key (version selecor) match the selected version.
	 * It permit to define special commands to apply for specific versions.
	 * This make the global package unit for all versions.
	**/
	JsonMap vspecific;
	/**
	 * Override some steps to provide commands to run durign install.
	 * Refer to package doc for more informations.
	**/
	StringMapList steps;
	/** 
	 * List of package which conflict with this one, this will produce failure
	 * at install if those package are present.
	**/
	StringList conflicts;
	/** 
	 * List of use flags allowed in this package, also provide their default
	 * status.
	**/
	UseFlags use;
	/** Warning message to display before installing this package **/
	StringList warn;
	/** 
	 * Install this package in module directory using the given name (which
	 * can contain `${SLOT}` or `${VESION}`).
	**/
	std::string module;
	/** Load some script to have more bash commands in the package **/
	StringList scripts;
	/** Define some variable to be used by the bash commands of the package **/
	StringMap vars;
	/** 
	 * List of flags (CFLAGS, CXXFLAGS) to append to system one. Can use
	 * `!-O3` to disable `-O3` defined by system.
	**/
	StringMapList flags;
	/** 
	 * List of URLs to use to download the package (can contain `${VERSION}`)?
	 * This is called in bash to it can also contain sub scripts to reformat
	 * version number for example with _ instead of .
	**/
	StringList urls;
	/** List of patch to apply. Prefer to use this in version specific**/
	StringList patch;
	/** List of slots **/
	StringMap slots;
};

}

#endif //HL_PACKAGE_DEF_HPP
