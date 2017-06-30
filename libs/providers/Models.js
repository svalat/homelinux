/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0                          *
*           Date    : 06/2017                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var fs = require('fs');
var PackageBuilder = require('../PackageBuilder');
var clone = require('clone');

/*********************  CLASS  **********************/
function Models(prefix)
{
	this.prefix = prefix;
	this.cache = {};
}

/*******************  FUNCTION  *********************/
Models.prototype.getName = function()
{
	return "Models";
}

/*******************  FUNCTION  *********************/
Models.prototype.getPackage = function(packageName)
{
	//build names
	if (packageName.indexOf('models/') == -1)
		return undefined;
	
	//load path
	var fname = this.prefix.getFile("/homelinux/packages/"+packageName+".json");
	
	//check in cache
	if (this.cache[packageName] != undefined)
		return clone(this.cache[packageName]);
	
	//console.error(fname);
	if (fs.existsSync(fname))
	{
		//console.error("Parsing "+fname);
		var content = fs.readFileSync(fname);
// 		try {
			var json = JSON.parse(content);
// 		} catch (e) {
// 			console.error("Failed to load "+fname);
// 			throw e;
// 		}
		
		this.cache[packageName] = clone(json);
		return json;
	} else {
		return undefined;
	}
}

/*******************  FUNCTION  *********************/
Models.prototype.updateCache = function(callback)
{
	callback();
}

/*******************  FUNCTION  *********************/
Models.prototype.updateDb = function(callback)
{
	callback();
}

/*******************  FUNCTION  *********************/
Models.prototype.search = function(name)
{
	return "";
}

module.exports = Models;
