/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
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
var httpreq = require('sync-request');
var find = require('find');
var PackageBuilder = require('./PackageBuilder');
var VersionHelper = require('./VersionHelper');
var colors = require('colors');
var QuickPackage = require('./QuickPackage');
var Batch = require('batch');
//providers
var Gentoo = require('./providers/Gentoo');
var HomeLinux = require('./providers/HomeLinux');

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
		homelinux: new HomeLinux(this),
		gentoo: new Gentoo(this)
	};
	this.providerList = [ "homelinux", "gentoo" ];
}

/*******************  FUNCTION  *********************/
Prefix.prototype.prefixOfPackage = function(userConfig,packageName)
{
	//load package to get full name if not already
	if (packageName.indexOf('/') == -1)
	{
		var pack = new PackageBuilder(this,userConfig,packageName);
		packageName = pack.pack.name;
	}
	
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
Prefix.prototype.updateDb = function(callback)
{
	//fetch all
	var batch = new Batch();
	batch.concurrency(2);
	
	var self = this;
	this.providerList.forEach(function(prov) {
		batch.push(function(done) {
			console.log(prov);
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
	
	//apply providers
	for (var i in this.providerList)
	{
		p = this.provider[this.providerList[i]].getPackage(packageName);
		if (p != undefined)
			break;
	}
	
	//otherwise fallback in default mode (TODO remove)
	if (p == undefined)
		p = this.buildQuickPackage(packageName);
	
	//setup version
	p.version = version;
	
	return p;
};

/*******************  FUNCTION  *********************/
Prefix.prototype.buildGithubPackage = function(qp)
{
	console.error("Search "+qp.name+" on github");
	var ret = httpreq('GET',"https://api.github.com/repos/"+qp.name+"/releases/latest",
		{
			'headers': {
				'user-agent': 'homelinux-user-agent'
		}
	});

	if (ret.statusCode == 200)
	{
		ret = JSON.parse(ret.getBody('utf8'));
		if (ret.tag_name == undefined)
			throw new Error("Fail to find last release of package on github");
		
		//gen
		qp.version = ret.tag_name.replace('v','');
		qp.url = "https://github.com/"+qp.name+"/archive/"+ret.tag_name+".tar.gz";
		qp.steps = { "download": [ "hl_github_download" ] };
		qp.name = "github/"+qp.name.split('/').pop();

		return qp;
	} else {
		var ret = httpreq('GET',"https://api.github.com/repos/"+qp.name+"/tags",
			{
				'headers': {
					'user-agent': 'homelinux-user-agent'
			}
		});
		if (ret.statusCode != 200)
			throw new Error('Failed to search version on github !');
		ret = JSON.parse(ret.getBody('utf8'));
		
		//gen
		qp.version = ret[0].name.replace('v','');
		qp.url = "https://github.com/"+qp.name+"/archive/"+ret[0].name+".tar.gz";
		qp.steps = { "download": [ "hl_github_download" ] };
		qp.name = "github/"+qp.name.split('/').pop();

		return qp;
	}
};

/*******************  FUNCTION  *********************/
Prefix.prototype.buildUrlsQuickPackage = function(qp)
{
	//build regexp
	var version = qp.version == undefined ? '[0-9]+.[0-9]+.?[0-9]*' : qp.version;
	var vregexp = new RegExp('^'+qp.name.replace(/[+]/g,"\\+")+"-("+version+").(tar.gz|tar.bz2|tar.bzip|tar.xz|tar.lz|tgz)$");
	
	//load gentoo db
	if (this.urlsDb == undefined)
	{
		var content = fs.readFileSync(this.getFile('/homelinux/packages/urls.lst'));
		this.urlsDb = content.split('\n');
	}
	
	//serach in gentoo list
	var finalv;
	var v = [];
	var ext;
	for (var i in this.urlsDb)
	{
		var fname = this.urlsDb[i].split('/').pop();
		if (vregexp.test(fname))
		{
			var ret = vregexp.exec(fname);
			v.push(ret[1]);
			finalv = ret[1];
			ext = ret[2];
		}
	}
	
	//sort
	v = VersionHelper.sortUniqVersions(v);
	
	//error
	if (finalv == undefined)
	{
		return undefined;
	} else {
		//setup
		qp.version = v;
		qp.url = [];
		var self = this;
		['tar.bz2','tar.xz','tar.gz','zip','tgz'].forEach(function(ext) {
			qp.url.push( "ftp://"+self.config.gentoo.server
				+ ":"+self.config.gentoo.port
				+ "/"+self.config.gentoo.distfiles
				+ "/"+qp.name+"-${VERSION}."+ext);
		});
		qp.name = "urls/"+qp.name;
		
		return qp;
	}
};

/*******************  FUNCTION  *********************/
Prefix.prototype.buildQuickPackage = function(packageName)
{
	//load quickpackage DB
	if (this.quickdb == undefined)
	{
		var content = fs.readFileSync(this.getFile('/homelinux/packages/quickpackages.json'));
		this.quickdb = JSON.parse(content);
	}
	
	//if not has entry
	var qp = this.quickdb[packageName];
	var auto = false;
	if (qp == undefined)
	{
		auto = true;
		console.error("Can't find package "+packageName+" in DB not QuickDB, try to build from gentoo with defaults");
		qp = {
			name: packageName.split('/').pop(),
			source: 'gentoo',
			type: 'models/auto',
			host: { 'default': false }
		};
		
		//manage gentoo/YYY
		if (packageName.split('/')[0] == 'gentoo')
		{
			console.error("Fallback automatically to gentoo based on package name");
			qp.name = packageName.replace('gentoo/','');
			qp.source = 'gentoo';
		}
		
		//manage github/XXX/YYY
		if (packageName.split('/')[0] == 'github')
		{
			console.error("Fallback automatically to github based on package name");
			qp.name = packageName.replace('github/','');
			qp.source = 'github';
		}
		
		//manage urls/YYYY
		if (packageName.split('/')[0] == 'urls')
		{
			console.error("Fallback automatically to url based on package name");
			qp.name = packageName.replace('urls/','');
			qp.source = 'urls';
		}
	}
	
	//if is gentoo source
	if (qp.source == 'gentoo' || qp.source == undefined)
		throw "Must not append here now";
	else if (qp.source == 'github')
		qp = this.buildGithubPackage(qp);
	else if (qp.source == 'urls')
		qp = this.buildUrlsQuickPackage(qp);
	else
		throw new Error("Unexpected qp.source !")

	//if auto & undef try urls
	if (qp == undefined && auto ==true)
	{
		console.error("Can't find package "+packageName+" in gentoo, try urls");
		qp = {
			name: packageName,
			source: 'urls',
			type: 'models/auto',
			host: { 'default': false }
		};
		qp = this.buildGithubPackage(qp);
	}

	//if auto & undef try github
	if (qp == undefined && auto ==true)
	{
		console.error("Can't find package "+packageName+" in urls, try github");
		qp = {
			name: packageName,
			source: 'github',
			type: 'models/auto',
			host: { 'default': false }
		};
		qp = this.buildGithubPackage(qp);
	}
	
	//build package
	var pack = {
		"name": qp.name,
		"inherit": this.getQuick('type',qp.name,qp.type == undefined ? 'models/auto' : qp.type),
		"versions": Array.isArray(qp.version)? qp.version: [ qp.version ],
		"subdir" : this.getQuick('subdir',qp.name,qp.name.split('/').pop()+"-${VERSION}"),
		"urls": Array.isArray(qp.url)? qp.url : [ qp.url ],
		"deps": qp.deps == undefined ? this.getQuick('deps',qp.name,[]) : this.getQuick('deps',qp.name,[]).concat(qp.deps) ,
		"patch": this.getQuick('patch',qp.name,[]), 
		"host": qp.host,
		"configure": qp.configure == undefined ? [] : { "":qp.configure },
		"provide": qp.provide,
		"md5": {},
		"steps": qp.steps,
		"module": this.getQuick('module',qp.name)
	};
	
	return pack;
};

/*******************  FUNCTION  *********************/
Prefix.prototype.search = function(name)
{
	var cache = this.getCache();
	console.log("-------------------------PACKAGES---------------------------");
	for (var i in cache)
		if (i.indexOf(name) != -1)
		{
			var p = new PackageBuilder(this,this.userConfig,i);
			console.log(p.getNameSlot()+"-"+p.getVersion()+" ["+p.getVersionList().join(', ')+"]");
		}
	console.log("--------------------------GENTOO----------------------------");
	this.gentooDb = require(this.getFile('/homelinux/packages/gentoo.json'));
	for (var i in this.gentooDb)
		if (this.gentooDb[i].indexOf(name) != -1)
			console.log("gentoo/"+this.gentooDb[i]);
	console.log("----------------------------URLS----------------------------");
	var content = fs.readFileSync(this.getFile('/homelinux/packages/urls.lst'));
	this.urlsDb = content.toString().split('\n');
	for (var i in this.urlsDb)
		if (this.urlsDb[i].indexOf(name) != -1)
			console.log("urls/"+this.urlsDb[i].split('/').pop());
	console.log("------------------------------------------------------------");
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
