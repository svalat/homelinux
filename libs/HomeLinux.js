/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var UserConfig = require('./UserConfig');
var Prefix = require('./Prefix');

/***********************  DOC  **********************/
/**
 * This is the main object to handle all the activities of
 * HomeLinux, it mostly aggregate the user config, a prefix
 * to manage and the perfix where homelinux is installed.
 * It manage all the file access to manage potential merging
 * and overriding between directories.
**/

/*********************  CLASS  **********************/
function HomeLinux(hlPrefix)
{
	this.hlPrefix = hlPrefix;
	this.userConfig = new UserConfig();
	this.prefix = new Prefix(this.userConfig,this.userConfig.config.prefix);
	this.mainPrefix = this.prefix;
}

/*******************  FUNCTION  *********************/
/**
 * Get a file from HomeLinux install prefix
 * @param fname path of the file to load, relatively to PREFIX/homelinux.
**/
HomeLinux.prototype.getHLFile = function(fname)
{
	return hlPrefix+"/homelinux/"+fname;
}

/*******************  FUNCTION  *********************/
/**
 * Get a file from the managed main prefix.
 * @param fname path of the file to load, relatively to PREFIX/homelinux.
**/
HomeLinux.prototype.getPrefixFile = function(fname)
{
	return this.mainPrefix.prefix+"/homelinux/"+fname;
}

/*******************  FUNCTION  *********************/
HomeLinux.prototype.fillEnv = function(envSetup)
{
	this.prefix.fillEnv(envSetup);
}

/*******************  FUNCTION  *********************/
HomeLinux.prototype.switchPrefix = function(targetPrefix)
{
	this.prefix = new Prefix(this.userConfig,targetPrefix);
	this.mainPrefix = this.prefix;
}

/*******************  FUNCTION  *********************/
HomeLinux.prototype.listInstalled = function(parentPrefix)
{
	if (parentPrefix == undefined)
	{
		console.log("================ "+this.prefix.prefix+" =================");
		this.prefix.listInstalled();
	} else {
		var inherit = this.prefix.getInheritPrefixes();
		for (var i in inherit)
		{
			console.log("================ "+inherit.prefix+" =================");
			inherit.listInstalled();
		}
	}
}

/*******************  FUNCTION  *********************/
HomeLinux.prototype.search = function(packName)
{
	this.mainPrefix.search(packName);
}

/*******************  FUNCTION  *********************/
HomeLinux.prototype.export = function()
{
	this.prefix.export(this.userConfig);
}

/********************  GLOBALS  *********************/
module.exports = HomeLinux;
