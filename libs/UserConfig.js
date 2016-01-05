/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * This class implement the userconfig system, its only a way to predefined
 * the default values and overrite them with the config file.
**/

/********************  GLOBALS  *********************/
var fs = require('fs'); 
var jso = require('json-override');

/*********************  CLASS  **********************/
/**
 * Load the user config file and override the default properties.
**/
function UserConfig()
{
	//default
	var defaultConfig = {
		prefix: "HL_DEFAULT_PREFIX",
		host: "default",
		python: "2.7",
		modules: [],
		host: 'default',
		temp: "/tmp/homelinux-"+process.env.USER,
		homecache: false
	};
	
	//load
	var filename = process.env.HOME + "/.homelinux.json"
	if (fs.existsSync(filename)) 
	{ 
		var content = fs.readFileSync(filename);
		var config = JSON.parse(content);
		this.config = jso(defaultConfig,config);
	} else {
		console.error("No config file ~/.homelinux.json, using default values !");
		this.config = defaultConfig;
	}
}

/*******************  FUNCTION  *********************/
UserConfig.prototype.getModules = function ()
{
	return this.config.modules;
}

/*******************  FUNCTION  *********************/
UserConfig.prototype.print = function()
{
	console.log(this.config);
}

/*******************  FUNCTION  *********************/
module.exports = UserConfig;