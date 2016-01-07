/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var VersionHelper = require('../VersionHelper');
var httpreq = require('sync-request');

/*********************  CLASS  **********************/
function Github(prefix)
{
	this.prefix = prefix;
	this.quickPackage = prefix.quickPackage;
}

/*******************  FUNCTION  *********************/
Github.prototype.updateCache = function(callback)
{
	//TODO
	callback();
}


/*******************  FUNCTION  *********************/
Github.prototype.updateDb = function(callback)
{
	//TODO
	callback();
}

/*******************  FUNCTION  *********************/
Github.prototype.getPackage = function(packageName)
{
	//build names
	var shortName;
	if (packageName.indexOf('github/') == 0)
	{
		shortName = packageName.replace('github/','');
	} else {
		shortName = packageName;
		packageName = 'github/'+packageName;
	}
	
	console.error("Search "+shortName+" on github");
	var ret = httpreq('GET',"https://api.github.com/repos/"+shortName+"/releases/latest",
		{
			'headers': {
				'user-agent': 'homelinux-user-agent'
		}
	});

	var qp = {};
	if (ret.statusCode == 200)
	{
		console.log("==============================");
		ret = JSON.parse(ret.getBody('utf8'));
		if (ret.tag_name != undefined)
		{
			qp.version = [ ret.tag_name.replace('v','') ];
			qp.url = "https://github.com/"+shortName+"/archive/"+ret.tag_name+".tar.gz";
			qp.steps = { "download": [ "hl_github_download" ] };
			qp.name = packageName;
		} else {
			qp = undefined;
		}
	} else {
		var ret = httpreq('GET',"https://api.github.com/repos/"+shortName+"/tags",
			{
				'headers': {
					'user-agent': 'homelinux-user-agent'
			}
		});
		if (ret.statusCode == 200)
		{
			ret = JSON.parse(ret.getBody('utf8'));
			
			//gen
			qp.version = [ ret[0].name.replace('v','') ];
			qp.url = "https://github.com/"+shortName+"/archive/"+ret[0].name+".tar.gz";
			qp.steps = { "download": [ "hl_github_download" ] };
			qp.name = packageName;
		} else {
			qp = undefined;
		}
	}
	
	if (qp != undefined)
		return  this.quickPackage.genFullPackage(qp);
	else
		return undefined;
}

module.exports = Github;
