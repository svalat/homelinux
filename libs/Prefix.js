/********************  GLOBALS  *********************/
var fs = require('fs');
var jso = require('json-override');

/*********************  CLASS  **********************/
function Prefix(prefix) 
{
	this.load(prefix);
}

/*******************  FUNCTION  *********************/
Prefix.prototype.print = function()
{
	console.log(this.config);
}

/*******************  FUNCTION  *********************/
Prefix.prototype.load = function(prefix)
{
	//setup
	this.prefix = prefix;
	
	//default
	var defaultConfig = {
		"inherit": [],
		"compiler": {
			"MAKEOPTS":"-j8",
			"CFLAGS":"-O3 -march=native",
			"CXXFLAGS":"$CFLAGS"
		},
		"packages": {
		},
		"useflags": {
		}
	};
	
	//config path
	var configfile = prefix + "/homelinux.json"
	if (fs.existsSync(configfile)) 
	{ 
		var config = require(configfile);
		this.config = jso(defaultConfig,config);
	} else {
		console.error("No config file "+configfile+", using default values !");
		this.config = defaultConfig;
	}
	
	//load inherited
	this.inherits = [];
	for (var i in this.config.inherit)
		this.inherit.push(new Prefix(i));
}

/*******************  FUNCTION  *********************/
Prefix.prototype.fillEnv = function(env)
{
	for (var i in this.inherit)
		this.inherit[i].fillEnv(env);
	env.addPrefix(this.prefix);
}

/*******************  FUNCTION  *********************/
Prefix.prototype.getFile = function(path)
{
	return this.prefix + "/" + path;
}

/*******************  FUNCTION  *********************/
Prefix.prototype.loadPackage = function(packageName)
{
	var fname = this.prefix + "/share/homelinux/packages/db/"+packageName+".json";
	if (fs.existsSync(fname) == false)
		fname = this.prefix + "/share/homelinux/packages/models/"+packageName+".json";
// 	try {
	var content = fs.readFileSync(fname);
	var json = JSON.parse(content);
	return json;
// 	} catch (e) {
// 		console.error("Error while loading "+fname+":\n" + e);
// 		throw e;
// 	}
}

/*******************  FUNCTION  *********************/
module.exports = Prefix;