/********************  GLOBALS  *********************/
var fs = require('fs'); 
var jso = require('json-override');

/*********************  CLASS  **********************/
function UserConfig()
{
	//default
	var defaultConfig = {
		prefix: process.env.HOME + "/usr",
		host: "default",
		python: "2.7",
		modules: [],
		temp: "/tmp/homelinux-"+process.env.USER
	};
	
	//load
	var filename = process.env.HOME + "/.homelinux.json"
	if (fs.existsSync(filename)) 
	{ 
		var config = require(filename);
		this.config = jso(defaultConfig,config);
	} else {
		console.error("No config file ~/.homelinux.json, using default values !");
		this.config = defaultConfig;
	}
}

/*******************  FUNCTION  *********************/
UserConfig.prototype.print = function()
{
	console.log(this.config);
}

/*******************  FUNCTION  *********************/
module.exports = UserConfig;