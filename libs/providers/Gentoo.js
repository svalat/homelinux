/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var VersionHelper = require('../VersionHelper');
var FtpClient = require('ftp');
var fs = require('fs');
var colors = require('colors');

/*********************  CLASS  **********************/
function Gentoo(prefix)
{
	this.prefix = prefix;
	this.quickPackage = prefix.quickPackage;
	this.config = prefix.config.gentoo;
}

/*******************  FUNCTION  *********************/
Gentoo.prototype.getDb = function()
{
	//already loaded
	if (this.gentooDb != undefined)
		return this.gentooDb;
	
	//load cache
	var fname = this.prefix.getFile('/homelinux/packages/gentoo.json');
	if (fs.existsSync(fname))
	{
		var content = fs.readFileSync(fname);
		this.gentooDb = JSON.parse(content);
	} else {
		this.cache = {};
		console.error("No gentoo list file available, consider to call 'hl update-db' at least once".yellow);
	}
	
	//return
	return this.gentooDb;
}

/*******************  FUNCTION  *********************/
Gentoo.prototype.search = function(name)
{
	var out = [];
	var db = this.getDb();
	name = name.toLowerCase();
	for (var i in db)
		if (db[i].toLowerCase().indexOf(name) != -1)
			out.push(colors.magenta("gentoo/"+db[i]));
	return out.join('\n');
}

/*******************  FUNCTION  *********************/
Gentoo.prototype.getName = function()
{
	return "Gentoo";
}

/*******************  FUNCTION  *********************/
Gentoo.prototype.getPackage = function(packageName)
{
	//build names
	var shortName;
	if (packageName.indexOf('gentoo/') == 0)
	{
		shortName = packageName.replace('gentoo/','');
	} else {
		shortName = packageName.split('/').pop();
		packageName = 'gentoo/'+shortName;
	}
	
	//get quick infos
	var quick = this.quickPackage.getInfos(packageName,{
		version:'[0-9]+.[0-9]+\\.?[0-9]*\\.?[0-9]*'
	});
	
	//get quick infos
	console.error(packageName + " => "+quick.version);
	
	//build regexp
	vregexp = new RegExp('^'+shortName.replace(/[+]/g,"\\+")+"-("+quick.version+").(tar.gz|tar.bz2|tar.bzip|tar.xz|tar.lz|tgz)$");
	
	//serach in gentoo list
	var finalv;
	var v = [];
	var ext;
	var db = this.getDb();
	for (var i in db)
	{
		if (vregexp.test(db[i]))
		{
			var ret = vregexp.exec(db[i]);
			v.push(ret[1]);
			finalv = ret[1];
			ext = ret[2];
		}
	}
	
	//sort
	v = VersionHelper.sortUniqVersions(v);
	
	//error
	if (v.length == 0)
	{
		return undefined;
	} else {
		//setup
		var qp = {
			version: v,
			url: [],
			name: packageName,
			host: {
				default:false,
				debian8: [ shortName+"-dev" ]
			}
		};

		//fill versions
		var self = this;
		['tar.bz2','tar.xz','tar.gz','zip','tgz'].forEach(function(ext) {
			qp.url.push( "ftp://"+self.config.server
				+ ":"+self.config.port
				+ "/"+self.config.distfiles
				+ "/"+shortName+"-${VERSION}."+ext);
		});
		
		//finish
		return this.quickPackage.genFullPackage(qp);
	}
}

/*******************  FUNCTION  *********************/
Gentoo.prototype.updateCache = function(callback)
{
	//nothing to do
	callback();
}

/*******************  FUNCTION  *********************/
/**
 * Fetch the gentoo distfiles/ file list to get all available packages.
**/
Gentoo.prototype.updateDb = function(callback)
{
	var ftp = new FtpClient();
	var self = this;
	
	ftp.on('ready', function() {
		console.log("connected to gentoo mirror...");
		ftp.cwd(self.config.distfiles,function(err, curr) {
			console.log("move to "+ self.config.distfiles+", fetching list...");
			if (err)
				callback(err);
			ftp.list(function(err, list) {
				if (err) 
					callback(err);
				self.saveDb(list);
				ftp.end();
				if (callback != undefined)
					callback();
			});
		});
	});
	
	ftp.connect({host: this.config.server, port:this.config.port});
};

/*******************  FUNCTION  *********************/
/**
 * Save the gentoo DB into a local file.
**/
Gentoo.prototype.saveDb = function(list)
{
	console.log("Saving gentoo DB....");
	var files = [];
	for (var i in list)
	{
		if (list[i].type == '-')
			files.push(list[i].name);
	}
	fs.writeFileSync(this.prefix.getFile('/homelinux/packages/gentoo.json'),JSON.stringify(files,null,"\t"));
	console.log("Gentoo DB saved");
};

module.exports = Gentoo;
