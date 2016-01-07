/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var fs = require('fs');
var VersionHelper = require('../VersionHelper');

/*********************  CLASS  **********************/
function Urls(prefix)
{
	this.prefix = prefix;
	this.quickPackage = prefix.quickPackage;
	
	//load db
	var content = fs.readFileSync(this.prefix.getFile('/homelinux/packages/urls.lst')).toString();
	this.db = content.split('\n');
}

/*******************  FUNCTION  *********************/
Urls.prototype.updateCache = function(callback)
{
	//TODO
	callback();
}


/*******************  FUNCTION  *********************/
Urls.prototype.updateDb = function(callback)
{
	//TODO
	callback();
}

/*******************  FUNCTION  *********************/
Urls.prototype.getPackage = function(packageName)
{
	//build names
	var shortName;
	if (packageName.indexOf('urls/') == 0)
	{
		shortName = packageName.replace('urls/','');
	} else {
		shortName = packageName;
		packageName = 'urls/'+packageName;
	}
	
	//get quick infos
	var quick = this.quickPackage.getInfos(packageName,{
		version:'[0-9]+.[0-9]+\\.?[0-9]*\\.?[0-9]*'
	});
	
	//build regexp
	vregexp = new RegExp('^'+shortName.replace(/[+]/g,"\\+")+"-("+quick.version+").(tar.gz|tar.bz2|tar.bzip|tar.xz|tar.lz|tgz)$");
	
	//search
	//serach in gentoo list
	var finalv;
	var v = [];
	var ext;
	var url;
	for (var i in this.db)
	{
		var fname = this.db[i].split('/').pop();
		if (vregexp.test(fname))
		{
			var ret = vregexp.exec(fname);
			v.push(ret[1]);
			finalv = ret[1];
			ext = ret[2];
			url = this.db[i].replace(ret[1],"${VERSION}");
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
		var qp = {
			version: v,
			url: [ url ],
			name: "urls/"+shortName
		};
		
// 		var self = this;
// 		['tar.bz2','tar.xz','tar.gz','zip','tgz'].forEach(function(ext) {
// 			qp.url.push( "ftp://"+self.config.gentoo.server
// 				+ ":"+self.config.gentoo.port
// 				+ "/"+self.config.gentoo.distfiles
// 				+ "/"+qp.name+"-${VERSION}."+ext);
// 		});
		
		return this.quickPackage.genFullPackage(qp);
	}
}

module.exports = Urls;
