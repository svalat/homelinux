/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
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
};

}

#endif //HL_OPTIONS_HPP
