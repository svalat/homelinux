/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * Aimed at providing access to the Debian package source repository.
 * Currently it does not support the depenency as we need to cut the dependency
 * tree to not finish with an infinite number of packages to install.
 * And it seems not trivial to be done.
**/

/********************  GLOBALS  *********************/
var VersionHelper = require('../VersionHelper');
var fs = require('fs');
var colors = require('colors');
var child_process = require('child_process');

/*********************  CONSTS  *********************/
/**
 * (Currently unused). List of packages to consider available to cut the
 * depenency try.
**/
var baseList = [
	"autoconf",
	"automake",
	"m4",
	"python-defaults",
	"python3-defaults",
	"gcc-defaults",
	"autotrash",
	"lsb",
	"binutils"
]

/*********************  CLASS  **********************/
/**
 * Constructor of the Devian provider.
 * @param prefix Reference to the prefix, used to access the DB file.
**/
function Debian(prefix)
{
	this.prefix = prefix;
	this.quickPackage = prefix.quickPackage;
	this.config = prefix.config.debian;
}

/*******************  FUNCTION  *********************/
/**
 * Lazy load of the Debian DB file.
**/
Debian.prototype.getDb = function()
{
	//already loaded
	if (this.gentooDb != undefined)
		return this.gentooDb;
	
	//load cache
	var fname = this.prefix.getFile('/homelinux/packages/debian.json');
	if (fs.existsSync(fname))
	{
		var content = fs.readFileSync(fname);
		this.gentooDb = JSON.parse(content);
	} else {
		this.cache = {};
		console.error("No debian DB available, consider to call 'hl update-db' at least once".yellow);
	}
	
	//return
	return this.gentooDb;
}

/*******************  FUNCTION  *********************/
/**
 * Search the list of packages maching the given name.
 * @return Return a string with the list of packages.
**/
Debian.prototype.search = function(name)
{
	var out = [];
	var db = this.getDb();
	name = name.toLowerCase();
	for (var i in db)
		if (i.toLowerCase().indexOf(name) != -1)
			out.push(colors.magenta("debian/"+i));
	return out.join('\n');
}

/*******************  FUNCTION  *********************/
/**
 * Return the name of the current provider.
**/
Debian.prototype.getName = function()
{
	return "Debian";
}

/*******************  FUNCTION  *********************/
/**
 * Sarch into the Debian DB and build a quickpakage for the given Name.
 * If not found, return undefined.
 * @param packageName Name of the package to search. Could be 'name', 
 * 'debian/name' of 'something/name'. The last one is used for fallback
 * of HomeLinux unknown packages.
**/
Debian.prototype.getPackage = function(packageName)
{
	//build names
	var shortName;
	if (packageName.indexOf('debian/') == 0)
	{
		shortName = packageName.replace('debian/','');
	} else {
		shortName = packageName.split('/').pop();
		packageName = 'debian/'+shortName;
	}

	//search
	var db = this.getDb();
	var pack;
	for (var i in db)
	{
		if (i == shortName)
			pack = db[i];
	}
	
	//search in sub packages
	if (pack == undefined)
	{
		for (var i in db)
		{
			var j = db[i].packages.indexOf(shortName);
			if (j != -1)
			{
				pack = db[i];
				break;
			}
		}
	}
	
	//search in sub packages
	//to fix eg libpng10-dev which is in real linpng-dev or linpng10
	/*if (pack == undefined && shortName.indexOf('-dev') != -1)
	{
		for (var i in db)
		{
			var j = db[i].packages.indexOf(shortName.replace('-dev',''));
			if (j != -1)
			{
				pack = db[i];
				break;
			}
		}
	}*/
	
	//not found
	if (pack == undefined)
		return undefined;
	
	//build quick pkg
	var qp = {
		version: pack.version,
		url: [ "http://http.debian.net/debian/"+pack.dir+"/"+pack.name+"_"+pack.version+".orig.tar.gz" ],
		subdir: '*',
		name: packageName,
		host: {
			"default":false,
			"debian8": pack.packages
		}
	};
	
	//filter deps
	var deps =pack.deps
				.replace(/[()]/g,"")
				.replace(/= /g,'=')
				.replace(/> /g,'>')
				.replace(/< /g,'<')
				.replace(/~/g,'')
				.split(', ')
				
	var finalDeps = [];
	for (var i in deps)
	{
		var name = deps[i].split(' ')[0];
		//if (name != 'debhelper' && name.indexOf('dh-') != 0 && name.indexOf('dpkg') != 0 && name.indexOf('autotools') != 0 && baseList.indexOf(name) == -1)
		//	finalDeps.push(name);
		for (var j in db)
		{
			if (db[j].packages.indexOf(name) != -1)
			{
				name = deps[i].replace(name,j).split(' ')[0];
				if (name != 'debhelper' && name.indexOf('dh-') != 0 && name.indexOf('dpkg') != 0 && name.indexOf('autotools') != 0 && baseList.indexOf(name) == -1)
					finalDeps.push(name);
			}
		}
	}

	//currently don't use deps
	//qp.deps = finalDeps;
	
	console.error("Install package "+qp.name);
	return this.quickPackage.genFullPackage(qp);
}

/*******************  FUNCTION  *********************/
Debian.prototype.updateCache = function(callback)
{
	//nothing to do
	callback();
}

/*******************  FUNCTION  *********************/
/**
 * Fetch the gentoo distfiles/ file list to get all available packages.
**/
Debian.prototype.updateDb = function(callback)
{
	var db =  {};

	for (var i in this.config.repos)
	{
		var repo = this.config.repos[i];
		console.log("Fetching DB... "+repo);
	
		try{
			var content = child_process.execSync("curl "+repo+"/source/Sources.xz | xz -d",{maxBuffer:(200*1024*1024)});
			content = content.toString().split('\n');
		} catch(e) {
			console.error("Fail to load debian DB : "+e);
			continue;
		}
		
		var state = {};
		var cur = '';
		
		console.log("Parsing DB...");
		for (var i in content)
		{
			var line = content[i];
			if (line.trim() == '')
			{
				//build
				db[state.Package] = {
					name: state.Package,
					version: state.Version.split('-')[0],
					deps: state['Build-Depends'],
					homepage: state.Homepage,
					packages: state.Binary.split(', '),
					dir: state.Directory,
					section: state.Section
				};
				
				//fill
				//for (var j in state['Package-List'])
				//	db[state.Package].packages.push(state['Package-List'][j][0]);
			} else if (line[0] == ' ') {
				if (Array.isArray(state[cur]))
					state[cur].push(line.split(' ').slice(1));
			} else {
				var infos = line.split(':');
				cur = infos[0];
				if (infos[1] == undefined || infos[1] == '' || infos[1] == ' ')
					state[cur] = [];
				else
					state[cur] = infos[1].trim();
			}
		}
	}

	this.saveDb(db);
	this.db = db;
	callback();
};

/*******************  FUNCTION  *********************/
/**
 * Save the gentoo DB into a local file.
**/
Debian.prototype.saveDb = function(db)
{
	console.log("Saving gentoo DB....");
	fs.writeFileSync(this.prefix.getFile('/homelinux/packages/debian.json'),JSON.stringify(db,null,"\t"));
	console.log("Debian DB saved");
};

module.exports = Debian;
