/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/*********************  CONSTS  *********************/
var FtpClient = require('ftp');
var fs = require('fs');
var compareVersion = require('compare-version');
var cheerio = require('cheerio');
var request = require('request');
var Batch = require('batch')
var PackageBuilder = require('./PackageBuilder');

/*********************  CLASS  **********************/
function VersionFetcher()
{
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.saveAll = function(prefix,packs)
{
	var out = {};
	for (var i in packs)
		out[packs[i].pack.name] = packs[i].pack.versions;
	console.log("Writing share/homelinux/packages/db/versions.json...");
	fs.writeFileSync(prefix.getFile("/share/homelinux/packages/db/versions.json"),JSON.stringify(out,null,'\t'))
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchAll = function(prefix,userConfig)
{
	this.gentooDb = require(prefix.getFile('/share/homelinux/packages/db/gentoo.json'));;
	
	//load all
	var packs = [];
	var cache = prefix.getCache();
	console.log("Loading packages....");
	for (var p in cache)
	{
		if (p != '__ignore_to_skip_last_comma__')
		{
			console.log("  - "+p);
			packs.push(new PackageBuilder(prefix,userConfig,p,false));
		}
	}
	
	//fetch all
	var batch = new Batch();
	batch.concurrency(2);
	
	var self = this;
	packs.forEach(function(p) {
		batch.push(function(done) {
			self.fetchVersions(p,done);
		});
	});

	batch.on('progress', function(e) {
		console.log("Progress : "+e.complete+"/"+e.total+" ["+e.percent+"%]");
		if (e.complete == e.total - 1)
			self.saveAll(prefix,packs);
	});
	
	batch.end(function(err,datas){
		if (err == null)
		{
			console.log("Finished without errors");
			self.saveAll(prefix,packs);
		} else {
			console.log("Get error : "+err);
		}
	});
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchFromGnomeCache = function(pack,callback)
{
	var self = this;
	var lst = pack.pack.vfetcher.url;
	if (Array.isArray(lst) == false)
		lst = [ lst ];

	for (var i in lst)
	{
		//console.log(lst[i]);
		request({
			method: 'GET',
			url: lst[i]
		}, function(err, response, body) {
			if (err) return console.error(err);

			// Tell Cherrio to load the HTML
			var json = JSON.parse(body);
			var shortName = pack.getShortName();
			pack.pack.versions = [];
			if (json[0] == 4)
				for (var i in json[1][shortName])
					pack.pack.versions.push(i);
			else
				throw "Invaild format";
			
			//sort & uniq
			pack.pack.versions = sort_unique(pack.pack.versions);
			console.log(pack.pack.name);
// 			console.log(pack.pack.name+" : "+pack.pack.versions);
			
			if (callback != undefined)
				callback(null,pack);
		});
	}
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchVersions = function(pack,callback)
{
	try {
		console.log("Start to fetch "+pack.pack.name);
		var mode = pack.pack.vfetcher.mode;
		if (mode == 'ftp')
			this.fetchVersionsFromFtp(pack,callback);
		else if (mode == 'http-apache-list' || mode == 'http')
			this.fetchVersionsFromApacheHttpList(pack,callback);
		else if (mode == 'gentoo')
			this.fetchFromGentoo(pack,callback);
		else if (mode == 'http-gnome-cache')
			this.fetchFromGnomeCache(pack,callback);
		else if (mode == 'none')
			callback();
		else
			throw "Invalid vfetcher "+mode+", please use ftp, http or gentoo !";
	} catch (e) {
		console.error("Fail to fetch version of "+pack.pack.name+" e: "+e);
		callback();
	}
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchFromGentoo = function(pack,callback)
{
	for (var i in this.gentooDb)
	{
		this.checkFile(pack,this.gentooDb[i]);
	}
}

/*******************  FUNCTION  *********************/
//from http://stackoverflow.com/questions/4833651/javascript-array-sort-and-unique
function sort_unique(arr) {
	return arr.sort(compareVersion).filter(function(el,i,a) {
		return (i==a.indexOf(el));
	});
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchVersionsFromApacheHttpList = function(pack,callback)
{
	var self = this;
	var lst = pack.pack.vfetcher.url;
	if (Array.isArray(lst) == false)
		lst = [ lst ];

	for (var i in lst)
	{
		//console.log(lst[i]);
		request({
			method: 'GET',
			url: lst[i]
		}, function(err, response, body) {
			if (err) return console.error(err);

			// Tell Cherrio to load the HTML
			$ = cheerio.load(body);
			$('a').each(function() {
					var href = $(this).text();
					//console.log(href);
					self.checkFile(pack,href);
			});
			
			//sort & uniq
			pack.pack.versions = sort_unique(pack.pack.versions);
			console.log(pack.pack.name);
// 			console.log(pack.pack.name+" : "+pack.pack.versions);
			
			if (callback != undefined)
				callback(null,pack);
		});
	}
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchVersionsFromFtp = function(pack,callback)
{
	var ftp = new FtpClient()
	var self = this;
	
	var lst = pack.pack.vfetcher.url;
	if (Array.isArray(lst) == false)
		lst = [ lst ];
	
	for (var j in lst)
	{
		//console.log(lst[j]);
		var vregexp = new RegExp('^ftp://([a-z0-9-\.]+)/(.+)');
		var ret = vregexp.exec(lst[j]);
		
		//error
		if (ret == null)
			throw "Fail to match FTP regexp with address "+lst[j];
		
		var server = ret[1];
		var dir = ret[2];
		
		try{
		
		ftp.on('ready', function() {
			//console.log("connected to "+server+" mirror...");
			ftp.cwd("/"+dir,function(err, curr) {
				//console.log("move to "+ dir+", fetching list...");
				if (err) throw err;
				ftp.list(function(err, list) {
					if (err) throw err;
					for (var i in list)
					{
						//if (list[i].type == '-')
						self.checkFile(pack,list[i].name);
					}
					
					//sort & uniq
					pack.pack.versions = sort_unique(pack.pack.versions);
					console.log(pack.pack.name);
// 					console.log(pack.pack.name+" : "+pack.pack.versions);
					
					ftp.end();
					
					if (callback != undefined)
						callback(null,pack);
				});
			});
		});
		
		ftp.connect({host: server});
		} catch (e) {
			console.log("Get error on "+pack.pack.name);
			callback();
		}
	}
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.checkFile = function(pack,file)
{
	var regexp = new RegExp(pack.pack.vfetcher.regexp);
	var ret = regexp.exec(file);
	if (ret != null)
	{
		//console.log("Find version "+ret[1]);
		pack.pack.versions.push(ret[1]);
	}
}

module.exports = VersionFetcher;