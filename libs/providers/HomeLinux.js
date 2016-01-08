/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var fs = require('fs');
var find = require('find');
var colors = require('colors');
var PackageBuilder = require('../PackageBuilder');

/*********************  CLASS  **********************/
function HomeLinuxProvider(prefix)
{
	this.prefix = prefix;
}

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.getCache = function()
{
	if (this.cache != undefined)
		return this.cache;
	
	//load cache
	var fname = this.prefix.getFile('homelinux/packages/db/cache.json');
	if (fs.existsSync(fname))
	{
		var content = fs.readFileSync(fname);
		try {
			this.cache = JSON.parse(content);
		} catch(e) {
			throw new Error("Failed to parse "+fname+" : "+JSON.stringify(e));
		}
	} else {
		this.cache = {};
		console.error("No cache file available, consider to call 'hl update-cache' at least once".yellow);
	}
	
	//return
	return this.cache;
}

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.getName = function()
{
	return "HomeLinux";
}

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.getPackage = function(packageName)
{
	//build names
	var shortName;
	if (packageName.indexOf('hl/') == 0)
	{
		shortName = packageName.replace('hl/','');
	} else {
		shortName = packageName;
		packageName = 'hl/'+packageName;
	}
	
	//search in cache
	if (shortName.indexOf('/') == -1)
	{
		shortName = this.searchInCache(shortName);
		packageName = 'hl/'+shortName;
	}
	
	//load path
	var fname = this.prefix.getFile("/homelinux/packages/db/"+shortName+".json");
	//console.error(fname);
	if (fs.existsSync(fname))
	{
		//console.error("Parsing "+fname);
		var content = fs.readFileSync(fname);
// 		try {
			var json = JSON.parse(content);
			json.name = packageName;
// 		} catch (e) {
// 			var err = JSONLint(content);
// 			throw new Error(colors.red("Failed to load "+fname+" : "+err.error+" at "+err.line+":"+err.character));
// 		}
		
		//apply versions
		var versions = this.prefix.getVersions();
		if (versions[packageName] != undefined)
		{
			json.versions = versions[packageName];
		}
		
		return json;
	} else {
		return undefined;
	}
}

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.updateCache = function(callback)
{
	var regexp = new RegExp("([A-Za-z0-9_-]+)/[A-Za-z0-9+_-]+\\.json");
	var self = this;
	var path = this.prefix.getFile("homelinux/packages/db");
	find.file(path, function(files) {
		var final = {};
		
		for (var i in files)
		{
			var file = files[i].replace(path+'/','').replace('.json','');
			var res = regexp.exec(files[i]);
			if (res != null && res[1] != 'models')
				final[file] = true;
		}
		fs.writeFileSync(self.prefix.getFile("homelinux/packages/db/cache.json"),JSON.stringify(final,null,'\t'));
		callback();
	}).error(function(err) {
		console.error(err.red);
		callback(err);
	});
}

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.updateDb = function(callback)
{
	//TODO
	callback();
}

/*******************  FUNCTION  *********************/
/**
 * Search a given short package name (without directory) into the cache and return
 * the full name. If multiple packages match, then it exit with a message reuesting
 * the user to provide the extra directory.
 * @param packageName Name of the package to search in the cache.
**/
HomeLinuxProvider.prototype.searchInCache = function(packageName)
{
	var regexp = new RegExp("/"+packageName.replace('+','\\+')+"$");
	
	var list = [];
	var cache = this.getCache();
	for (var i in cache)
		if (regexp.test(i))
			list.push(i);
	
	if (list.length == 0)
	{
		return packageName;
	} else if (list.length == 1) {
		//console.error("Ok, simple name converted to : "+list[0]);
		return list[0];
	} else {
		console.error("Failed to find your package, multiple match : "+list.concat(','));
		process.exit(1);
	}
};

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.search = function(name)
{
	var out = [];
	var cache = this.getCache();
	for (var i in cache)
	{
		if (i.indexOf(name) != -1)
		{
			var p = new PackageBuilder(this.prefix,this.prefix.userConfig,i);
			out.push(p.getNameSlot().green+"-"+p.getVersion().cyan+" ["+p.getVersionList().reverse().slice(0,10).join(', ').blue+"]");
		}
	}
	
	return out.join('\n');
}

module.exports = HomeLinuxProvider;
