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
#include <portability/System.hpp>
#include <base/Helper.hpp>
#include <base/Debug.hpp>
#include "EnvSetup.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************* CONST ************************/ 
/**
 * List of env variables to manage.
**/
static const char * gblVarNames[] = {
	"PATH","LD_LIBRARY_PATH","CPATH","MANPATH","PERL5LIB",
	"PKG_CONFIG_PATH","MODULEPATH","PYTHONPATH","CMAKE_PREFIX_PATH",
	"LD_RUN_PATH","HL_PREFIX_PATH"
};

/*******************  FUNCTION  *********************/
/**
* Init the EnvSetup class, mostly by loading the default values of each
* managed environnement variables.
* @param config Define the user config class to use;
**/
EnvSetup::EnvSetup(const Config * config)
{
	//check
	assert(config != NULL);
	
	//setup
	this->config = config;
	this->setupEmpty();
}

/*******************  FUNCTION  *********************/
/**
* More used for unit tests it setup the default values to empty
* for all used variables.
**/
void EnvSetup::setupEmpty(void)
{
	//set to empty
	for (auto & var : gblVarNames)
		env[var] = "";
}

/*******************  FUNCTION  *********************/
/**
* Load variables from env
**/
void EnvSetup::loadCurrent(void)
{
	//load from env
	for (auto & it : env)
		it.second = System::getEnv(it.first);
		
	//special for pkg-config otherwise we miss the system one
	if (env["PKG_CONFIG_PATH"].empty())
	{
		this->prepend("PKG_CONFIG_PATH","/usr/lib/pkgconfig");
		this->prepend("PKG_CONFIG_PATH","/usr/share/pkgconfig");
		this->prepend("PKG_CONFIG_PATH","/usr/local/lib/pkgconfig");
		this->prepend("PKG_CONFIG_PATH","/usr/local/share/pkgconfig");
	}
}

/*******************  FUNCTION  *********************/
/**
* Add a new prefix to the environnement variable. This function setup all
* the variables managed by homelinux for the given prefix.
* @param prefix Path of the prefix to use.
**/
void EnvSetup::addPrefix(const std::string & prefix)
{
	//check if already loaded
	if (this->hasPrefix(prefix))
		return;
	else
		this->loadedPrefix.push_back(prefix);
	
	//basic
	this->prepend("PATH",prefix + "/bin");
	this->prepend("PATH",prefix + "/sbin");
	this->prepend("LD_LIBRARY_PATH",prefix + "/lib");
	this->prepend("LD_LIBRARY_PATH",prefix + "/lib64");
	this->prepend("LD_RUN_PATH",prefix + "/lib");
	this->prepend("LD_RUN_PATH",prefix + "/lib64");
	
	//CMAKE_PREFIX_PATH
	this->prepend("CMAKE_PREFIX_PATH",prefix);
	
	//more advanced
	this->prepend("MANPATH",prefix + "/share/man");
	this->prepend("CPATH",prefix + "/include");
	
	//perl
	this->prepend("PERL5LIB",prefix + "/lib/perl5");
	this->prepend("PERL5LIB",prefix + "/lib/perl5/site_perl");
	
	//pkg-config
	this->prepend("PKG_CONFIG_PATH",prefix + "/lib/pkgconfig");
	this->prepend("PKG_CONFIG_PATH",prefix + "/share/pkgconfig");
	
	//python
	std::string pv = "2.7";
	//if (child_process.execSync != undefined)
	//    pv = child_process.execSync("python --version 2>&1 | cut -d ' ' -f 2 | cut -d '.' -f 1-2").toString().trim();
	this->prepend("PYTHONPATH",prefix + "/lib/python"+pv+"/site-packages/");
	
	//module
	this->prepend("MODULEPATH",prefix + "/Modules/modulefiles");
	
	//hl
	this->prepend("HL_PREFIX_PATH",prefix);
}

/*******************  FUNCTION  *********************/
/**
* Remove removeExisting paths for the given variable
**/
void EnvSetup::removeExisting(const std::string & varname,const char separator)
{
	//check
	assumeArg(env.find(varname) != env.end(),"Fail to find env variable %1").arg(varname).end();
	
	//remove
	std::list<std::string> out;
	Helper::split(env[varname],separator,[&out,this](const std::string & value){
		bool status = true;
		for (auto & prefix : loadedPrefix)
		{
			if (Helper::contain(value,prefix+"/") || value == prefix)
				status = false;
		}
		if (status)
			out.push_back(value);
	});
	
	//repush
	env[varname] = Helper::join(out,separator);
}

/*******************  FUNCTION  *********************/
/**
* Remove the existing paths from homelinux
**/
void EnvSetup::removeExisting(void)
{
	for (auto & var : env)
		this->removeExisting(var.first,':');
	this->loadedPrefix.clear();
}

/*******************  FUNCTION  *********************/
/**
* Print the nevironenemnt variable setup (mostly export calls)
* to be used by shell scripts (bash/sh)
**/
void EnvSetup::print(std::ostream & out)
{
	for (auto & var : env)
		out << "export " << var.first << "=" << var.second << std::endl;
}

/*******************  FUNCTION  *********************/
/**
* Check if the given prefix has been loaded.
* @param prefix Prefix to check
**/
bool EnvSetup::hasPrefix(const std::string & prefix)
{
	for (auto & p : loadedPrefix)
		if (p == prefix)
			return true;
	return false;
}

/*******************  FUNCTION  *********************/
/**
* Load of unload the modules
**/
void EnvSetup::loadModules(bool load,std::ostream & out)
{
    //module command
    out << "module 1>/dev/null 2>/dev/null || hl is-pack-installed sys-apps/modules > /dev/null && . $(hl prefix-of sys-apps/modules)/Modules/current/init/$(basename $SHELL)" << std::endl;
    
    //modules
	for (auto & module : config->modules)
		out << "module " << (load?"load ":"unload ")  << module << std::endl;
}

/*******************  FUNCTION  *********************/
/**
* Setup ccache
**/
void EnvSetup::enableCCache(std::ostream & out)
{
	if (config->ccache)
		out << "export PATH="<<config->prefix.front() << "/bin/ccache-links:$PATH" << std::endl;
}

/*******************  FUNCTION  *********************/
/**
* Enable usage of hl-py-env wrapper
**/
void EnvSetup::enablePyEnv(std::ostream & out)
{
	if (config->pyEnv)
		out << "export PATH="<<config->prefix.front() << "/bin/hl-py-env-bins:$PATH" << std::endl;
}

/*******************  FUNCTION  *********************/
/**
 * Prepend a value to the list of paths.
 * @param varname Name of the env variable to edit
 * @param value Value to prepend
 * @param sep Separator to use.
**/
void EnvSetup::prepend(const std::string & varname,const std::string & value, const std::string & sep)
{
	//check
	assumeArg(env.find(varname) != env.end(),"Fail to find variable %1").arg(varname).end();
	
	//setup
	if (this->env[varname].empty() == false)
		this->env[varname] = value + sep + this->env[varname];
	else
		this->env[varname] = value;
}

/*******************  FUNCTION  *********************/
/**
 * Append a value to the list of paths.
 * @param varname Name of the env variable to edit
 * @param value Value to append
 * @param sep Separator to use.
**/
void EnvSetup::append(const std::string & varname,const std::string & value, const std::string & sep)
{
	//check
	assumeArg(env.find(varname) != env.end(),"Fail to find variable %1").arg(varname).end();
	
	//setup
	if (this->env[varname].empty() == false)
		this->env[varname] =  this->env[varname] + sep + value;
	else
		this->env[varname] = value;
}
	
}
