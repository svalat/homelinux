/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * This file provide the necessary tools to fetch the version list of each packaes by grabing the
 * defined websites.
**/

/*********************  CONSTS  *********************/
var FtpClient = require('ftp');
var fs = require('fs');
var compareVersion = require('compare-version');
var cheerio = require('cheerio');
var request = require('request');
var Batch = require('batch')
var PackageBuilder = require('./PackageBuilder');
var httpreq = require('sync-request');

/*********************  CLASS  **********************/
function VersionFetcher()
{
}

/*******************  FUNCTION  *********************/
/**
 * Save the list of package version into the given prefix.
 * @param prefix Prefix (object) in which to save the version list;
 * @param packs List of packages to save.
**/
VersionFetcher.prototype.saveAll = function(prefix,packs)
{
	var out = {};
	for (var i in packs)
		out[packs[i].pack.name] = packs[i].pack.versions;
	console.log("Writing homelinux/packages/db/versions.json...");
	fs.writeFileSync(prefix.getFile("/homelinux/packages/db/versions.json"),JSON.stringify(out,null,'\t'))
}

/*******************  FUNCTION  *********************/
/**
 * Loop on all the packages defined into the given prefix and fetch their vesrion list.
 * @param prefix Define the prefix object to use.
 * @param userConfig Define the userconfig to use.
**/
VersionFetcher.prototype.fetchAll = function(prefix,userConfig)
{
	this.gentooDb = require(prefix.getFile('/homelinux/packages/gentoo.json'));;
	
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
	batch.concurrency(16);
	
	var self = this;
	packs.forEach(function(p) {
		batch.push(function(done) {
			self.fetchVersions(p,done);
		});
	});

	batch.on('progress', function(e) {
		console.log("Progress : "+e.complete+"/"+e.total+" ["+e.percent+"%]");
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
/**
 * The gnome team provide a cache.json file into their archive folder, it is the simplest way
 * to extract version list from their respository as they use a level of directories to 
 * store the files and not provide them in a uniq one.
 * @param pack Define the package for which to fetch the version;
 * @param callback Callback to be called when the operation is finished. First parameter is err second is pack.
**/
VersionFetcher.prototype.fetchFromGnomeCache = function(pack,callback)
{
	var self = this;
	var lst = pack.pack.vfetcher.url;
	if (Array.isArray(lst) == false)
		lst = [ lst ];

	var cnt = 0;
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
				throw new Error("Invaild format");
			
			//sort & uniq
			pack.pack.versions = sort_unique(pack.pack.versions);
			console.log(pack.pack.name);
// 			console.log(pack.pack.name+" : "+pack.pack.versions);
			
			if (callback != undefined && ++cnt == lst.length)
				callback(null,pack);
		});
	}
}

/*******************  FUNCTION  *********************/
/**
 * Fetch the list of versions for the geven package.
 * @param pack Package to use
 * @param clalback Callback to be called on finish. First param is err, second is pack.
**/
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
		else if (mode == 'github')
			this.fetchVersionsFromGithub(pack,callback);
		else if (mode == 'none')
			callback(null);
		else
			throw new Error("Invalid vfetcher "+mode+", please use ftp, http or gentoo !");
	} catch (e) {
		console.error("Fail to fetch version of "+pack.pack.name+" e: "+e);
		callback(null);
	}
};

/*******************  FUNCTION  *********************/
/**
 * Fetch list of versions from gentoo distfile directory. As we already store it into
 * a local json it is a simple loop on its content.
 * @param pack Package to use
 * @param clalback Callback to be called on finish. First param is err, second is pack.
**/
VersionFetcher.prototype.fetchFromGentoo = function(pack,callback)
{
	for (var i in this.gentooDb)
	{
		this.checkFile(pack,this.gentooDb[i]);
	}
	callback(null);
};

/*******************  FUNCTION  *********************/
//from http://stackoverflow.com/questions/4833651/javascript-array-sort-and-unique
function sort_unique(arr) {
	return arr.sort(compareVersion).filter(function(el,i,a) {
		return (i==a.indexOf(el));
	});
}

/*******************  FUNCTION  *********************/
/**
 * Fetch an http page and search for all its internal links to check if their text
 * property match the version regexp of the package. It was originaly written for
 * apache directory list but finally also work fine on standard pages.
 * @param pack Package to use
 * @param clalback Callback to be called on finish. First param is err, second is pack.
**/
VersionFetcher.prototype.fetchVersionsFromGithub = function(pack,callback)
{
	var self = this;
	var url = pack.pack.vfetcher.url.replace('github://','');
	
	var ret = httpreq('GET',"https://api.github.com/repos/"+url+"/releases/latest",
		{
			'headers': {
				'user-agent': 'homelinux-user-agent'
		}
	});
	
	var key;
	if (ret.statusCode == 200)
	{
		ret = JSON.parse(ret.getBody('utf8'));
		if (ret.tag_name == undefined)
			throw new Error("Fail to find last release of package on github");
		key = 'tag_name';
	} else {
		ret = httpreq('GET',"https://api.github.com/repos/"+url+"/tags",
			{
				'headers': {
					'user-agent': 'homelinux-user-agent'
			}
		});
		if (ret.statusCode != 200)
			throw new Error('Failed to search version on github !');
		ret = JSON.parse(ret.getBody('utf8'));
		key = 'name';
	}
	
	//load
	for (var i in ret)
		this.checkFile(pack,ret[i][key]);
	
	//sort & uniq
	pack.pack.versions = sort_unique(pack.pack.versions);
	
	//callback
	if (callback != undefined)
		callback(null,pack);
};

/*******************  FUNCTION  *********************/
/**
 * Fetch an http page and search for all its internal links to check if their text
 * property match the version regexp of the package. It was originaly written for
 * apache directory list but finally also work fine on standard pages.
 * @param pack Package to use
 * @param clalback Callback to be called on finish. First param is err, second is pack.
**/
VersionFetcher.prototype.fetchVersionsFromApacheHttpList = function(pack,callback)
{
	var self = this;
	var lst = pack.pack.vfetcher.url;
	if (Array.isArray(lst) == false)
		lst = [ lst ];

	var cnt = 0;
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
			
			if (callback != undefined && ++cnt == lst.length)
				callback(null,pack);
		});
	}
};

/*******************  FUNCTION  *********************/
/**
 * Fetch the list of files from and FTP directory and apply the version regexp on
 * them to extract version list.
 * @param pack Package to use
 * @param clalback Callback to be called on finish. First param is err, second is pack.
**/
VersionFetcher.prototype.fetchVersionsFromFtp = function(pack,callback)
{
	var ftp = new FtpClient();
	var self = this;
	
	var lst = pack.pack.vfetcher.url;
	if (Array.isArray(lst) == false)
		lst = [ lst ];
	
	var cnt = 0;
	for (var j in lst)
	{
		//console.log(lst[j]);
		var vregexp = new RegExp('^ftp://([a-z0-9-\.]+)/(.+)');
		var ret = vregexp.exec(lst[j]);
		
		//error
		if (ret == null)
			throw new Error("Fail to match FTP regexp with address "+lst[j]);
		
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
						
						if (callback != undefined && ++cnt == lst.length)
							callback(null,pack);
					});
				});
			});
			
			ftp.connect({host: server,connTimeout:100000,pasvTimeout:100000});
		} catch (e) {
			console.log("Get error on "+pack.pack.name);
			callback();
		}
	}
};

/*******************  FUNCTION  *********************/
/**
 * Check if the given file match the regexp version from package, if it is true
 * the verison is added to the version list of package.
 * @param pack Define the package to use.
 * @param file Define the file name to test.
**/
VersionFetcher.prototype.checkFile = function(pack,file)
{
	var regexp = new RegExp(pack.pack.vfetcher.regexp);
	var ret = regexp.exec(file);
	if (ret != null)
	{
		//console.log("Find version "+ret[1]);
		pack.pack.versions.push(ret[1]);
	}
};

module.exports = VersionFetcher;
