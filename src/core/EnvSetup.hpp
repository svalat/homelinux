/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_ENV_SETUP_HPP
#define HL_ENV_SETUP_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <map>
#include <vector>
#include <iostream>
//internal
#include <base/Config.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************* TYPES ************************/
typedef std::map<std::string,std::string> EnvVarMap;
typedef std::vector<std::string> EnvPrefixVector;

/*********************  CLASS  **********************/
/**
* This script in responsible of the management of the environnement setup
* mostly to build all the export variables.
**/
class EnvSetup
{
	public:
		EnvSetup(const Config * config, bool loadActualEnv = true);
		void loadCurrent(void);
		void addPrefix(const std::string & prefix);
		void removeExisting(void);
		void print(std::ostream & out = std::cout);
		bool hasPrefix(const std::string & prefix);
		void loadModules(bool load = true,std::ostream & out = std::cout);
		void enableCCache(std::ostream & out = std::cout);
		void enablePyEnv(std::ostream & out = std::cout);
	private:
		void setupEmpty(bool loadActualEnv = true);
		void prepend(const std::string & varname,const std::string & value, const std::string & sep = ":");
		void append(const std::string & varname,const std::string & value, const std::string & sep = ":");
		void removeExisting(const std::string & varname,const char separator = ':');
	private:
		const Config * config;
		EnvPrefixVector loadedPrefix;
		EnvVarMap env;
};

	
}

#endif //HL_ENV_SETUP_HPP
