/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var fs = require('fs');
var jso = require('json-override');
var httpreq = require('sync-request');
var find = require('find');
var PackageBuilder = require('./PackageBuilder');
var VersionHelper = require('./VersionHelper');

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
		"versions": {
		},
		"useflags": {
		},
		"gentoo" : {
			"server":"gentoo.mirrors.ovh.net",
			"port": 21,
			"distfiles": "gentoo-distfiles/distfiles/"
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
	
	//load version
	this.getVersions();
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
Prefix.prototype.getVersions = function()
{
	if (this.versions == undefined)
	{
		var fname = this.getFile('share/homelinux/packages/db/versions.json');
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
}

/*******************  FUNCTION  *********************/
Prefix.prototype.loadQuickFile = function(part)
{
	var fname = this.getFile('share/homelinux/packages/quickpackages/'+part+'.txt');
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
}

/*******************  FUNCTION  *********************/
Prefix.prototype.getQuick = function(part,name)
{
	if (part == 'deps')
	{
		if (this.quickdeps == undefined)
			this.quickdeps = this.loadQuickFile('deps');
		
		if (this.quickdeps[name] == undefined)
			return [];
		else
			return this.quickdeps[name];
	} else if (part == 'config') {
		if (this.quickconfig == undefined)
			this.quickconfig = this.loadQuickFile('configure');
		
		if (this.quickconfig[name] == undefined)
			return [];
		else
			return this.quickconfig[name];
	} else if (part == 'version') {
		if (this.quickversion == undefined)
			this.quickversion = this.loadQuickFile('version');
		
		if (this.quickversion[name] == undefined)
			return undefined;
		else
			return this.quickversion[name][0];
	} else {
		throw "Invalid part : "+part;
	}
}

/*******************  FUNCTION  *********************/
Prefix.prototype.getCache = function()
{
	if (this.cache == undefined)
	{
		var fname = this.getFile('share/homelinux/packages/db/cache.json');
		var content = fs.readFileSync(fname);
		this.cache = JSON.parse(content);
	}
	
	return this.cache;
}

/*******************  FUNCTION  *********************/
Prefix.prototype.searchInCache = function(packageName)
{
	this.getCache();
	
	var regexp = new RegExp("/"+packageName+"$");
	
	var list = [];
	for (var i in this.cache)
		if (regexp.test(i))
			list.push(i);
	
	if (list.length == 0)
	{
		return packageName;
	} else if (list.length == 1) {
		//console.error("Ok, simple name converted to : "+list[0]);
		return list[0];
	} else {
		console.error("Failed to find your package, multiple match : "+list.concat(','));
		process.exit(1);
	}
}

/*******************  FUNCTION  *********************/
Prefix.prototype.loadPackage = function(packageName)
{
	//extract version
	var version = packageName.split('@')[1];
	var packageName = packageName.split('@')[0];
	
	//if has no / search in db first before fallback
	if (packageName.indexOf('/') == -1)
		packageName = this.searchInCache(packageName);
	
	//load path
	var fname = this.prefix + "/share/homelinux/packages/db/"+packageName+".json";
	//console.error(fname);
	var p;
	if (fs.existsSync(fname) == false && packageName.indexOf('models/') == 0)
		fname = this.prefix + "/share/homelinux/packages/"+packageName+".json";
	if (fs.existsSync(fname))
	{
		//console.error("loading "+fname);
		var content = fs.readFileSync(fname);
		var json = JSON.parse(content);
		p = json;
	} else {
		p = this.buildQuickPackage(packageName);
	}
	
	//setup version
	p.version = version;
	
	return p;
}

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
			throw "Fail to find last release of package on github";
		
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
			throw 'Failed to search version on github !';
		ret = JSON.parse(ret.getBody('utf8'));
		
		//gen
		qp.version = ret[0].name.replace('v','');
		qp.url = "https://github.com/"+qp.name+"/archive/"+ret[0].name+".tar.gz";
		qp.steps = { "download": [ "hl_github_download" ] };
		qp.name = "github/"+qp.name.split('/').pop();

		return qp;
	}
}

/*******************  FUNCTION  *********************/
Prefix.prototype.buildUrlsQuickPackage = function(qp)
{
	//build regexp
	var version = qp.version == undefined ? '[0-9]+.[0-9]+.?[0-9]*' : qp.version;
	var vregexp = new RegExp('^'+qp.name.replace(/[+]/g,"\\+")+"-("+version+").tar.(gz|bz2|bzip|xz|lz)$");
	
	//load gentoo db
	if (this.urlsDb == undefined)
		this.urlsDb = require(this.getFile('/share/homelinux/packages/urls.json'));
	
	//serach in gentoo list
	var finalv;
	var v = [];
	var ext
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
		['bz2','xz','gz','zip'].forEach(function(ext) {
			qp.url.push( "ftp://"+self.config.gentoo.server
				+ ":"+self.config.gentoo.port
				+ "/"+self.config.gentoo.distfiles
				+ "/"+qp.name+"-${VERSION}.tar."+ext);
		});
		qp.name = "urls/"+qp.name;
		
		return qp;
	}
}

/*******************  FUNCTION  *********************/
Prefix.prototype.buildGentooQuickPackage = function(qp)
{
	var vregexp = this.getQuick('version','gentoo/'+qp.name);
	if (vregexp == undefined)
		var vregexp = this.getQuick('version',qp.name);
	if (vregexp == undefined)
		vregexp = '[0-9]+.[0-9]+.?[0-9]*';
	
	console.error(qp.name + " => "+vregexp);
	
	//build regexp
	var version = qp.version == undefined ? vregexp : qp.version;
	var vregexp = new RegExp('^'+qp.name.replace(/[+]/g,"\\+")+"-("+version+").tar.(gz|bz2|bzip|xz|lz)$");
	
	//load gentoo db
	if (this.gentooDb == undefined)
		this.gentooDb = require(this.getFile('/share/homelinux/packages/gentoo.json'));
	
	//serach in gentoo list
	var finalv;
	var v = [];
	var ext
	for (var i in this.gentooDb)
	{
		if (vregexp.test(this.gentooDb[i]))
		{
			var ret = vregexp.exec(this.gentooDb[i]);
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
		['bz2','xz','gz','zip'].forEach(function(ext) {
			qp.url.push( "ftp://"+self.config.gentoo.server
				+ ":"+self.config.gentoo.port
				+ "/"+self.config.gentoo.distfiles
				+ "/"+qp.name+"-${VERSION}.tar."+ext);
		});
		qp.name = "gentoo/"+qp.name;
		
		return qp;
	}
}

/*******************  FUNCTION  *********************/
Prefix.prototype.buildQuickPackage = function(packageName)
{
	//load quickpackage DB
	if (this.quickdb == undefined)
	{
		var content = fs.readFileSync(this.getFile('/share/homelinux/packages/quickpackages.json'));
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
			name: packageName,
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
		qp = this.buildGentooQuickPackage(qp);
	else if (qp.source == 'github')
		qp = this.buildGithubPackage(qp);
	else if (qp.source == 'urls')
		qp = this.buildUrlsQuickPackage(qp);
	else
		throw "Unexpected qp.source !"

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
		"inherit": qp.type == undefined ? 'models/auto' : qp.type,
		"versions": Array.isArray(qp.version)? qp.version: [ qp.version ],
		"subdir" : qp.name.split('/').pop()+"-${VERSION}",
		"urls": Array.isArray(qp.url)? qp.url : [ qp.url ],
		"deps": qp.deps == undefined ? [] : qp.deps ,
		"host": qp.host,
		"configure": qp.configure == undefined ? [] : { "":qp.configure },
		"provide": qp.provide,
		"md5": {},
		"steps": qp.steps
	};
	
	return pack;
}

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
	this.gentooDb = require(this.getFile('/share/homelinux/packages/gentoo.json'));
	for (var i in this.gentooDb)
		if (this.gentooDb[i].indexOf(name) != -1)
			console.log("gentoo/"+this.gentooDb[i]);
	console.log("----------------------------URLS----------------------------");
	this.urlsDb = require(this.getFile('/share/homelinux/packages/urls.json'));
	for (var i in this.urlsDb)
		if (this.urlsDb[i].indexOf(name) != -1)
			console.log("urls/"+this.urlsDb[i].split('/').pop());
	console.log("------------------------------------------------------------");
}

/*******************  FUNCTION  *********************/
Prefix.prototype.listInstalled = function()
{
	find.file(this.getFile("share/homelinux/install-db/"), function(files) {
		for (var i in files)
		{
			var content = fs.readFileSync(files[i]);
			var p = JSON.parse(content);
			if (p.slot != '0' && p.slot != undefined)
				console.log(p.name+":"+p.slot+"-"+p.version);
			else
				console.log(p.name+"-"+p.version);
		}
	})
}

/*******************  FUNCTION  *********************/
Prefix.prototype.export = function(userConfig)
{
	var exp = {};
	
	//fill
	exp['prefixConfig'] = this.config;
	exp['userConfig'] = userConfig.config;
	
	//installed
	var installed = {}
	exp['installed'] = installed;
	find.file(this.getFile("share/homelinux/install-db/"), function(files) {
		for (var i in files)
		{
			var content = fs.readFileSync(files[i]);
			var p = JSON.parse(content);
			installed[p.name] = p.version;
		}
		
		console.log(JSON.stringify(exp,null,'\t'));
	})
}

/*******************  FUNCTION  *********************/
module.exports = Prefix;