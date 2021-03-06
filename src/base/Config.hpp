/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_OPTIONS_HPP
#define HL_OPTIONS_HPP

/********************  HEADERS  *********************/
//must be as first
#include "config.h"
//std
#include <string>
#include <ctime>
#include <vector>
//internal
#include <base/Helper.hpp>
//from jsoncpp
#include <json/json.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
/**
* Structure to store the configuration of HLPIPE.
**/
struct Config
{
	Config(bool loadUserConfig = false);
	~Config(void);
	void loadDefault(void);
	void load(std::string path = "");
	void load(Json::Value & config);
	void parseArgs(int argc, const char ** argv);
	void save(void);
	//vars
	/** Display debugging category at exit **/
	bool showDebugCat;
	/** List of prefix to load (in priority order). The first one will be master. **/
	StringList prefix;
	/** Host type to check pkg db (centos7, debian8, gentoo, default) **/
	std::string host;
	/** List of modules to enable in `hl env` **/
	StringList modules;
	/** Enable exporting path for ccache **/
	bool ccache;
	/** Enable usage of pyEnv **/
	bool pyEnv;
	/** Use home cache (usefull for dev) **/
	bool homecache;
	/** Override content of packages **/
	Json::Value packageOverride;
	/** Temporary directory to be used **/
	std::string temp;
	/** Command to run */
	std::string command;
	/** List of arguements **/
	StringList args;
	/** If need to show help **/
	bool showHelp;
	/** For Github crawler **/
	std::string clientId;
	/** For Github crawler **/
	std::string clientSecret;
	/** Crawler threader **/
	int crawlerThreads;
	/** Crawler timeout **/
	int crawlerTimeout;
	/** editor to open packages **/
	std::string editor;
	/** -j ooptions for mak **/
	int makeJ;
};

}

#endif //HL_OPTIONS_HPP
