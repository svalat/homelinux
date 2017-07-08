/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>
//unix
#include <unistd.h>
#include <getopt.h>
//internals
#include "Debug.hpp"
#include <portability/System.hpp>
//current
#include "Config.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
/**
 * Config constructor.
**/
Config::Config(bool loadUserConfig)
{
	this->loadDefault();
	if (loadUserConfig)
		this->load();
}

/*******************  FUNCTION  *********************/
/**
 * Config destructor
**/
Config::~Config(void)
{
	if (showDebugCat)
		Debug::showList();
}

/*******************  FUNCTION  *********************/
/**
 * Load default values in the config to start.
**/
void Config::loadDefault()
{
	//this->prefix.push_back(System::getHomeDir()+"/usr");
	this->prefix.push_back(PREFIX);
	this->temp = "/tmp/homelinux-"+System::getEnv("USER");
	this->host = "default";
	this->ccache = false;
	this->pyEnv = true;
	this->homecache = false;
	this->showHelp = false;
	this->showDebugCat = false;
	this->crawlerThreads = 4;
}

/*******************  FUNCTION  *********************/
/**
 * Load the JSON values into the config.
 * @param config JSON tree to load.
**/
void Config::load(Json::Value & config)
{
	//basics
	Helper::jsonToObj(this->prefix,config["prefix"]);
	this->host = config.get("host",this->host).asString();
	this->homecache = config.get("homecache",this->homecache).asBool();
	this->ccache = config.get("ccache",this->ccache).asBool();
	this->pyEnv = config.get("pyEnv",this->pyEnv).asBool();
	this->temp = config.get("temp",this->temp).asString();
	this->clientId = config["github"].get("clientId","").asString();
	this->clientSecret = config["github"].get("clientSecret","").asString();
	this->crawlerThreads = config.get("crawlerThreads",4).asInt();
	this->crawlerTimeout = config.get("crawlerTimeout",60).asInt();
	
	//modules
	Json::Value mods = config["modules"];
	if (mods.isArray()) {
		for (Json::Value::iterator it = mods.begin() ; it != mods.end() ; ++it)
			this->modules.push_back((*it).asString());
	}
	
	//override
	this->packageOverride = config["packageOverride"];
}

/*******************  FUNCTION  *********************/
void Config::save(void)
{
	//vars
	Json::Value json;
	
	//convert
	Helper::toJson(json["prefix"],prefix);
	json["host"] = host;
	json["homecache"] = homecache;
	json["ccache"] = ccache;
	json["pyEnv"] = pyEnv;
	json["temp"] = temp;
	json["github"]["clientId"] = clientId;
	json["github"]["clientSecret"] = clientSecret;
	json["crawlerThreads"] = crawlerThreads;
	json["crawlerTimeout"] = crawlerTimeout;
	Helper::toJson(json["modules"],modules);
	json["packageOverride"] = packageOverride;
	
	//write
	System::writeJson(json,System::getHomeDir()+"/.homelinux.json");
}

/*******************  FUNCTION  *********************/
/**
 * Load the given config file (json format).
 * @param path Path to the file to load.
**/
void Config::load(std::string path)
{
	//compute file path
	if (path.empty())
		path = System::getHomeDir()+"/.homelinux.json";
	
	//check if exist
	if (System::fileExist(path) == false)
	{
		HL_WARNING_ARG("Caution, %1 does not exist, creating default one !").arg(path).end();
		this->save();
		return;
	}
	
	//open file
	std::ifstream file(path.c_str(),std::ifstream::binary);
	assumeArg(file.is_open(),"Fail to open configuration file : %1 : %2").arg(path).argStrErrno().end();
	
	//load
	Json::Value tmpObj;
	
	//parse
	Json::Reader reader;
	bool status = reader.parse(file, tmpObj);
	assumeArg(status,"Fail to load configuration file '%1' : \n%2")
		.arg(path)
		.arg(reader.getFormattedErrorMessages())
		.end();

	//apply
	this->load(tmpObj);
}

/*******************  FUNCTION  *********************/
/**
 * Parse arguements.
 * @param argc Number of arguments from main.
 * @param argv List of arguments from main.
**/
void Config::parseArgs(int argc, const char ** argv)
{
	for (int i = 1 ; i < argc ; i++)
	{
		if (strcmp(argv[i],"-v")==0)
		{
			assume(i < argc - 1,"Option -v require a parameter !");
			if (strcmp(argv[i+1],"help") == 0)
				this->showDebugCat = true;
			else
				Debug::setVerbosity(argv[i+1]);
			i++;
		}  else if (strcmp(argv[i],"--version") == 0) {
			std::cout << "Homelinux " << HOMELINUX_VERSION << std::endl;
			exit(0);
		}  else if (strcmp(argv[i],"-p") == 0 || strcmp(argv[i],"--prefix") == 0) {
			assume(i < argc - 1,"Option -p/--prefix require a parameter !");
			prefix.clear();
			prefix.push_back(argv[i+1]);
			i++;
		} else if (strcmp("--help",argv[i]) == 0 || strcmp("-h",argv[i]) == 0) {
			showHelp = true;
		} else if (strcmp(argv[i],"--no-user-config") == 0) {
			//nothing
		} else if (command.empty()) {
			command = argv[i];
		} else {
			args.push_back(argv[i]);
		}
	}
}

}
