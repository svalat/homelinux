/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0                          *
*           Date    : 06/2017                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * Define and manage an Homelinux prefix.
**/

/********************  GLOBALS  *********************/
var fs = require('fs');
var jso = require('json-override');
var find = require('find');
var PackageBuilder = require('./PackageBuilder');
var VersionHelper = require('./VersionHelper');
var colors = require('colors');
var QuickPackage = require('./QuickPackage');
var Batch = require('batch');
var colors = require('colors');
//providers
var Gentoo = require('./providers/Gentoo');
var HomeLinux = require('./providers/HomeLinux');
var Urls = require('./providers/Urls');
var Github = require('./providers/Github');
var Models = require('./providers/Models');
var Debian = require('./providers/Debian');

/*********************  CLASS  **********************/
/**
 * Prefix constructor. By default only load the given prefix configuration.
 * @param prefixPath Define the path of the prefix to load.
**/
function Prefix(userConfig,prefixPath) 
{
	this.userConfig = userConfig;
	this.load(prefixPath);
	this.quickPackage = new QuickPackage(this);
	this.provider = {
		debian: new Debian(this),
		models: new Models(this),
		homelinux: new HomeLinux(this),
		gentoo: new Gentoo(this),
		urls: new Urls(this),
		github: new Github(this)
	};
	if (this.config.providers != undefined)
		this.providerList = [ "models" ].concat(this.config.providers);
	else
		this.providerList = [ "models", "homelinux", "gentoo", "debian", "urls", "github" ];
	//order for getch
	this.fetchProviderList = [ "gentoo", "debian", "homelinux" ];
}

/*******************  FUNCTION  *********************/
Prefix.prototype.prefixOfPackage = function(userConfig,packageName)
{
	//load package to get full name if not already
	var pack = new PackageBuilder(this,userConfig,packageName);
	packageName = pack.pack.name;
	
	//search in install dir
	var files = fs.readdirSync(this.getFile('/homelinux/install-db/'));
	var fname = packageName.replace(/[/]/g,'_');
	
	//search
	for (var i in files)
	{
		if (files[i].indexOf(fname) == 0)
			return this;
	}
	
	//search in inherited childs
	for (var i in this.inherit)
	{
		var ret = this.inherit[i].prefixOfPackage(userConfig,packageName);
		if (ret != null)
			return ret;
	}
	
	return null;
};

/*******************  FUNCTION  *********************/
/**
 * Print the prefix configuration.
**/
Prefix.prototype.print = function()
{
	console.log(this.config);
};

/*******************  FUNCTION  *********************/
Prefix.prototype.updateCache = function(callback)
{
	//fetch all
	var batch = new Batch();
	batch.concurrency(2);
	
	var self = this;
	this.providerList.forEach(function(prov) {
		batch.push(function(done) {
			self.provider[prov].updateCache(done);
		});
	});

	batch.on('progress', function(e) {
		console.log("Progress : "+e.complete+"/"+e.total+" ["+e.percent+"%]");
	});
	
	batch.end(function(err,datas){
		if (err == null)
			console.log("Finished without errors");
		else
			console.log("Get error : "+err);
		callback(err);
	});
}

/*******************  FUNCTION  *********************/
Prefix.prototype.updateDb = function(callback)
{
	//fetch all
	var batch = new Batch();
	batch.concurrency(1);
	
	var self = this;
	this.fetchProviderList.forEach(function(prov) {
		batch.push(function(done) {
			self.provider[prov].updateDb(done);
		});
	});

	batch.on('progress', function(e) {
		console.log("Progress : "+e.complete+"/"+e.total+" ["+e.percent+"%]");
	});
	
	batch.end(function(err,datas){
		if (err == null)
			console.log("Finished without errors");
		else
			console.log("Get error : "+err);
		callback(err);
	});
}

/*******************  FUNCTION  *********************/
/**
 * Load the configuration of the given prefix.
 * @param prefixPath Define the path of the prefix to load.
**/
Prefix.prototype.load = function(prefixPath)
{
	//setup
	this.prefix = prefixPath;
	
	//default
	var defaultConfig = {
		"inherit": [],
		"compiler": {
			"MAKEOPTS":"-j8",
			"CFLAGS":"-O3 -march=native",
			"CXXFLAGS":"$CFLAGS"
		},
		"versions": {
		},
		"use": {
		},
		"gentoo" : {
			"server":"gentoo.mirrors.ovh.net",
			"port": 21,
			"distfiles": "gentoo-distfiles/distfiles/"
		}
	};
	
	//config path
	var configfile = prefixPath + "/homelinux.json";
	if (fs.existsSync(configfile)) 
	{ 
		var config = require(configfile);
		this.config = jso(defaultConfig,config);
	} else {
		console.error("No config file "+configfile+", using default values !");
		this.config = defaultConfig;
	}
	
	//override by user
	if (this.userConfig.config.prefixOverride != undefined && this.userConfig.config.prefixOverride[prefixPath] != undefined)
		this.config = jso(this.config,this.userConfig.config.prefixOverride[prefixPath]);
	
	//load inherited
	this.inherit = [];
	for (var i in this.config.inherit)
		this.inherit.push(new Prefix(this.userConfig,i));
	
	//load version
	this.getVersions();
};

/*******************  FUNCTION  *********************/
Prefix.prototype.getInheritPrefixes = function()
{
	return thus.inherit;
}

/*******************  FUNCTION  *********************/
/**
 * Loop under the inhertied prefix to fill the environnement variables.
 * @param env Define the environnement setting object.
**/
Prefix.prototype.fillEnv = function(env)
{
	for (var i in this.inherit)
		this.inherit[i].fillEnv(env);
	env.addPrefix(this.prefix);
};

/*******************  FUNCTION  *********************/
/**
 * Return the full path of a file from prefix.
**/
Prefix.prototype.getFile = function(path)
{
	return this.prefix + "/" + path;
};

/*******************  FUNCTION  *********************/
/**
 * Load the version file DB and return it. This funcion to caching to it is directly
 * return if already loaded.
**/
Prefix.prototype.getVersions = function()
{
	if (this.versions == undefined)
	{
		var fname = this.getFile('homelinux/packages/db/versions.json');
		if (fs.existsSync(fname))
		{
			var content = fs.readFileSync(fname);
			this.versions = JSON.parse(content);
		} else {
			console.error("Please run 'hl update-db' at lease once, using known package version !");
			this.versions = {};
		}
	}
	
	return this.versions;
};

/*******************  FUNCTION  *********************/
/**
 * Load a file from quickpackages directory. It also make the splitting to convert
 * it into JS objects and arrays.
 * @param prop Name of the property to load (will be converted to prop.txt)
**/
Prefix.prototype.loadQuickFile = function(prop)
{
	var fname = this.getFile('homelinux/packages/quickpackages/'+prop+'.txt');
	var content = fs.readFileSync(fname);
	var lines = content.toString().split('\n');
	
	var ret = {};
	for (var i in lines)
	{
		if (lines[i][0] != '#' && lines[i][0] != '')
		{
			var infos = lines[i].split(' ');
			ret[infos[0]] = infos.slice(1);
		}
	}
	
	return ret;
};

/*******************  FUNCTION  *********************/
/**
 * Get a property from quickpackages system.
 * @param prop Define the property to read
 * @param name Define the name of the package
 * @param defaultValue Optional default value to apply if not present. Let to undefined by default.
**/
Prefix.prototype.getQuick = function(prop,packageName,defaultValue)
{
	return this.quickPackage.getQuickInfo(prop,packageName,defaultValue);
};

/*******************  FUNCTION  *********************/
Prefix.prototype.loadPackage = function(packageName)
{
	//extract version
	var version = packageName.split('@')[1];
	packageName = packageName.split('@')[0];
	
	var provider = packageName.split('/')[0];
	var p;
	if (this.provider[provider] != undefined)
	{
		p = this.provider[provider].getPackage(packageName);
	} else {
		//apply providers
		for (var i in this.providerList)
		{
			p = this.provider[this.providerList[i]].getPackage(packageName);
			if (p != undefined)
				break;
		}
	}
	
	//otherwise fallback in default mode (TODO remove)
	if (p == undefined)
		throw new Error(colors.red("Fail to find package "+packageName));
	
	//setup version
	p.version = version;
	
	return p;
};

/*******************  FUNCTION  *********************/
Prefix.prototype.search = function(name)
{
	var reversed = this.providerList.slice().reverse();
	for (var i in reversed)
	{
		if (reversed[i] != 'models')
		{
			var provider = this.provider[reversed[i]];
			console.log("------------------------- "+provider.getName()+" ---------------------------");
			console.log(provider.search(name));
		}
	}
};

/*******************  FUNCTION  *********************/
Prefix.prototype.listInstalled = function(callback)
{
	find.file(this.getFile("homelinux/install-db/"), function(files) {
		var ret = [];
		for (var i in files)
		{
			var content = fs.readFileSync(files[i]);
			var p = JSON.parse(content);
			if (p.slot != '0' && p.slot != undefined)
				ret.push(colors.green(p.name+":"+p.slot)+" "+p.version.cyan);
			else
				ret.push(p.name.green+" "+p.version.cyan);
		}
		callback(ret.join('\n'));
	});
};

/*******************  FUNCTION  *********************/
Prefix.prototype.export = function(userConfig)
{
	var exp = {};
	
	//fill
	exp['prefixConfig'] = this.config;
	exp['userConfig'] = userConfig.config;
	
	//installed
	var installed = {};
	exp['installed'] = installed;
	find.file(this.getFile("homelinux/install-db/"), function(files) {
		for (var i in files)
		{
			var content = fs.readFileSync(files[i]);
			var p = JSON.parse(content);
			installed[p.name] = p.version;
		}
		
		console.log(JSON.stringify(exp,null,'\t'));
	});
};

/*******************  FUNCTION  *********************/
Prefix.prototype.hasPackageInstalled = function(p,prefix)
{
	if (prefix == undefined)
		prefix = this.prefix;
	
	if (p.isInstalled(this)) 
	{
		return true;
	} else {
		for (var i in this.inherit)
			if (p.isInstalled(this.inherit[i]))
				return true;
	}
	
	return false;
};

/*******************  FUNCTION  *********************/
Prefix.prototype.getPrefixOf = function(p, prefix)
{
	if (prefix == undefined)
		prefix = this.prefix;
	
	if (p.isInstalled(this.prefix)) 
	{
		return prefix;
	} else {
		for (var i in this.inherit)
			if (p.isInstalled(this.inherit[i]))
				return prefix;
	}
	
	return null;
};

/*******************  FUNCTION  *********************/
module.exports = Prefix;
