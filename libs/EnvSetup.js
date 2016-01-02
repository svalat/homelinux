/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

var child_process = require('child_process');

/***********************  DOC  **********************/
/**
 * This script in responsible of the management of the environnement setup
 * mostly to build all the export variables.
**/

/*********************  CLASS  **********************/
/**
 * Init the EnvSetup class, mostly by loading the default values of each
 * managed environnement variables.
 * @param userConfig Define the user config class to use;
**/
function EnvSetup(userConfig)
{
	this.userConfig = userConfig;
	this.loadedPerfix = [];
	this.vars = {
		'PATH':process.env.PATH,
		'LD_LIBRARY_PATH':process.env.LD_LIBRARY_PATH,
		'CPATH':process.env.CPATH,
		'MANPATH':process.env.MANPATH,
		'PERL5LIB':process.env.PERL5LIB,
		'PKG_CONFIG_PATH':process.env.PKG_CONFIG_PATH,
		'MODULEPATH':process.env.MODULEPATH,
		'PYTHONPATH':process.env.PYTHONPATH,
		'CMAKE_PREFIX_PATH':process.env.CMAKE_PREFIX_PATH,
		'LD_RUN_PATH':process.env.LD_RUN_PATH,
		'HL_PREFIX_PATH':process.env.HL_PREFIX_PATH,
	};
	
	if (this.vars.PKG_CONFIG_PATH == "")
	{
		//pkg-config
		this.prepend('PKG_CONFIG_PATH',"/usr/lib/pkgconfig");
		this.prepend('PKG_CONFIG_PATH',"/usr/share/pkgconfig");
	}
}

/*******************  FUNCTION  *********************/
EnvSetup.prototype.loadModules = function(load)
{
	for (var i in this.userConfig.config.modules)
		if (load == undefined || load == true)
			console.log("module load "+this.userConfig.config.modules[i]);
		else
			console.log("module unload "+this.userConfig.config.modules[i]);
}

/*******************  FUNCTION  *********************/
/**
 * Enable usage of ccache by adding its path to the global PATH in order
 * to override the compiler commands.
**/
EnvSetup.prototype.enableCCache = function()
{
	if (this.userConfig.config.ccache)
		this.prepend('PATH',this.userConfig.config.prefix + "/bin/ccache-links");
};

/*******************  FUNCTION  *********************/
/**
 * Enable usage of hl-py-env wrapper
**/
EnvSetup.prototype.enablePyEnv = function()
{
	if (this.userConfig.config.pyEnv)
		this.prepend('PATH',this.userConfig.config.prefix + "/bin/hl-py-env-bins");
};

/*******************  FUNCTION  *********************/
/**
 * Prepend a value to the given variable
 * @param varname Name of the variable to prepend.
 * @param value Value to prepend.
 * @param sep separator to use (: if undefined)
**/
EnvSetup.prototype.prepend = function(varname,value,sep)
{
	//select
	if (sep == undefined)
		sep = ":";

	//apply
	if (this.vars[varname] != '' && this.vars[varname] != undefined)
		this.vars[varname] = value + sep + this.vars[varname];
	else
		this.vars[varname] = value;
};

/*******************  FUNCTION  *********************/
/**
 * Check if a given array contain some values;
**/
function contains(arr,obj) {
    var i = arr.length;
    while (i--) {
        if (arr[i] === obj) {
            return true;
        }
    }
    return false;
}

/*******************  FUNCTION  *********************/
/**
 * Add a new prefix to the environnement variable. This function setup all
 * the variables managed by homelinux for the given prefix.
 * @param prefix Path of the prefix to use.
**/
EnvSetup.prototype.addPrefix = function(prefix)
{
	//check if already loaded
	if (contains(this.loadedPerfix,prefix))
		return;
	else
		this.loadedPerfix.push(prefix);
	
	//basic
	this.prepend('PATH',prefix + "/bin");
	this.prepend('PATH',prefix + "/sbin");
	this.prepend('LD_LIBRARY_PATH',prefix + "/lib");
	this.prepend('LD_LIBRARY_PATH',prefix + "/lib64");
	this.prepend('LD_RUN_PATH',prefix + "/lib");
	this.prepend('LD_RUN_PATH',prefix + "/lib64");
	
	//CMAKE_PREFIX_PATH
	this.prepend('CMAKE_PREFIX_PATH',prefix);
	
	//more advanced
	this.prepend('MANPATH',prefix + "/share/man");
	this.prepend('CPATH',prefix + "/include");
	
	//perl
	this.prepend('PERL5LIB',prefix + "/lib/perl5");
	this.prepend('PERL5LIB',prefix + "/lib/perl5/site_perl");
	
	//pkg-config
	this.prepend('PKG_CONFIG_PATH',prefix + "/lib/pkgconfig");
	this.prepend('PKG_CONFIG_PATH',prefix + "/share/pkgconfig");
	
	//python
	var pv = "2.7";
	if (child_process.execSync != undefined)
		pv = child_process.execSync("python --version 2>&1 | cut -d ' ' -f 2 | cut -d '.' -f 1-2").toString().trim();
	this.prepend('PYTHONPATH',prefix + "/lib/python"+pv+"/site-packages/");
	
	//module
	this.prepend('MODULEPATH',prefix + "/Modules/modulefiles");
	
	//hl
	this.prepend('HL_PREFIX_PATH',prefix);
};

/*******************  FUNCTION  *********************/
/**
 * Remove removeExisting paths for the given variable
**/
EnvSetup.prototype.removeExistingForVar = function(varname,separator)
{
	//get list of active prefix
	var lst = this.vars.HL_PREFIX_PATH.split(':');
	
	if (separator == undefined)
		separator = ':';
	
	//remove
	var values = this.vars[varname].split(separator);
	var out = [];
	for (var i in values)
	{
		var status = true;
		for (var j in lst)
			if (values[i].indexOf(lst[j]+"/") == 0 || values[i] == lst[j])
				status = false;
		if (status)
			out.push(values[i]);
	}
	
	this.vars[varname] = out.join(separator);
};

/*******************  FUNCTION  *********************/
/**
 * Remove the existing paths from homelinux
**/
EnvSetup.prototype.removeExisting = function()
{
	for (var i in this.vars)
		this.removeExistingForVar(i,':');
};

/*******************  FUNCTION  *********************/
EnvSetup.prototype.switch = function(userConfig,prefix)
{
	//
};

/*******************  FUNCTION  *********************/
/**
 * Print the nevironenemnt variable setup (mostly export calls)
 * to be used by shell scripts (bash/sh)
**/
EnvSetup.prototype.print = function()
{
	for (var i in this.vars)
		console.log("export "+i+"="+this.vars[i]+"");
};

/*******************  FUNCTION  *********************/
module.exports = EnvSetup;
