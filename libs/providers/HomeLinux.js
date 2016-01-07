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

/*********************  CLASS  **********************/
function HomeLinuxProvider(prefix)
{
	this.prefix = prefix;
	
	//load cache
	var fname = this.prefix.getFile('homelinux/packages/db/cache.json');
	if (fs.existsSync(fname))
	{
		var content = fs.readFileSync(fname);
		this.cache = JSON.parse(content);
	} else {
		this.cache = {};
		console.error("No cache file available, consider to call 'hl update-cache' at least once".yellow);
	}
}

/*******************  FUNCTION  *********************/
HomeLinuxProvider.prototype.getCache = function()
{
	return this.cache;
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
		shortName = this.searchInCache(shortName);
	
	//load path
	var fname = this.prefix.getFile("/homelinux/packages/db/"+shortName+".json");
	console.error(fname);
	if (fs.existsSync(fname))
	{
		//console.error("Parsing "+fname);
		var content = fs.readFileSync(fname);
// 		try {
			var json = JSON.parse(content);
			json.name = packageName;
// 		} catch (e) {
// 			console.error("Failed to load "+fname);
// 			throw e;
// 		}
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
	console.log("Nothing to do");
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
	for (var i in this.cache)
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

module.exports = HomeLinuxProvider;