/*********************  CONSTS  *********************/
var FtpClient = require('ftp');
var fs = require('fs');
var compareVersion = require('compare-version');

/*********************  CLASS  **********************/
function VersionFetcher()
{
	
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchVersions = function(pack)
{
	console.log(pack.pack);
	if (pack.pack.vfetcher.mode == 'ftp')
		this.fetchVersionsFromFtp(pack);
}

/*******************  FUNCTION  *********************/
//from http://stackoverflow.com/questions/4833651/javascript-array-sort-and-unique
function sort_unique(arr) {
	return arr.sort(compareVersion).filter(function(el,i,a) {
		return (i==a.indexOf(el));
	});
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.fetchVersionsFromFtp = function(pack)
{
	var ftp = new FtpClient()
	var self = this;
	
	console.log(pack.pack.vfetcher.url);
	var vregexp = new RegExp('^ftp://([a-z0-9\.]+)/(.+)');
	var ret = vregexp.exec(pack.pack.vfetcher.url);
	
	var server = ret[1];
	var dir = ret[2];
	
	ftp.on('ready', function() {
		console.log("connected to "+server+" mirror...");
		ftp.cwd(dir,function(err, curr) {
			console.log("move to "+ dir+", fetching list...");
			if (err) throw err;
			ftp.list(function(err, list) {
				if (err) throw err;
				for (var i in list)
				{
					if (list[i].type == '-')
						self.checkFile(pack,list[i].name);
				}
				
				//sort & uniq
				pack.pack.versions = sort_unique(pack.pack.versions);
				console.log(pack.pack.versions);
				
				ftp.end();
			});
		});
	});
	
	ftp.connect({host: server});
}

/*******************  FUNCTION  *********************/
VersionFetcher.prototype.checkFile = function(pack,file)
{
	var regexp = new RegExp(pack.pack.vfetcher.regexp);
	var ret = regexp.exec(file);
	if (ret != null)
	{
		console.log("Find version "+ret[1]);
		pack.pack.versions.push(ret[1]);
	}
}

module.exports = VersionFetcher;