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
	if (fs.existsSync(fname))
	{
		var content = fs.readFileSync(fname);
		var json = JSON.parse(content);
		return json;
	} else {
		return this.buildQuickPackage(packageName);
	}
}

/*******************  FUNCTION  *********************/
Prefix.prototype.buildGentooQuickPackage = function(qp)
{
	//build regexp
	var version = qp.version == undefined ? '[0-9]+.[0-9]+.?[0-9]*' : qp.version;
	var vregexp = new RegExp('^'+qp.name+"-("+version+").tar.(gz|bz2|bzip)$");
	
	//load gentoo db
	if (this.gentooDb == undefined)
		this.gentooDb = require(this.getFile('/share/homelinux/packages/db/gentoo.json'));
	
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
	
	//error
	if (finalv == undefined)
		throw 'Fail to find version in gentoo DB for package ';
	
	//setup
	qp.version = v;
	qp.url = "ftp://"+this.config.gentoo.server
	       + ":"+this.config.gentoo.port
	       + "/"+this.config.gentoo.distfiles
	       + "/"+qp.name+"-${VERSION}.tar."+ext;
		   
	return qp;
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
	if (qp == undefined)
	{
		console.error("Can't find package "+packageName+" in DB not QuickDB, try to build from gentoo with defaults");
		qp = {
			name: packageName,
			source: 'gentoo',
			type: 'auto',
			host: { 'default': false }
		};
	}
	
	//if is gentoo source
	if (qp.source == 'gentoo' || qp.source == undefined)
		qp = this.buildGentooQuickPackage(qp);
	else
		throw "Unexpected qp.source !"
	
	//build package
	var pack = {
		"name": packageName,
		"inherit": qp.type == undefined ? 'auto' : qp.type,
		"versions": Array.isArray(qp.version)? qp.version: [ qp.version ],
		"subdir" : qp.name+"-${VERSION}",
		"urls": [ qp.url ],
		"deps": qp.deps == undefined ? [] : qp.deps ,
		"host": qp.host,
		"configure": qp.configure == undefined ? [] : { "":qp.configure },
		"md5": {}
	};
	
	return pack;
}

/*******************  FUNCTION  *********************/
module.exports = Prefix;