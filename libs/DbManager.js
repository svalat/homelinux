/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * The DbManager class is used to load the external databases
 * to search packages. Currently it only support the gentoo one
 * and only fetch its main file list;
**/

/*********************  CONSTS  *********************/
var FtpClient = require('ftp');
var fs = require('fs');

/*********************  CLASS  **********************/
function DbManager(prefix)
{
	this.prefix = prefix;
}

/*******************  FUNCTION  *********************/
/**
 * Fetch the gentoo distfiles/ file list to get all available packages.
**/
DbManager.prototype.fetchGentoo = function(callback)
{
	var ftp = new FtpClient();
	var self = this;
	
	ftp.on('ready', function() {
		console.log("connected to gentoo mirror...");
		ftp.cwd(self.prefix.config.gentoo.distfiles,function(err, curr) {
			console.log("move to "+ self.prefix.config.gentoo.distfiles+", fetching list...");
			if (err) throw err;
			ftp.list(function(err, list) {
				if (err) throw err;
				self.saveGentooDb(list);
				ftp.end();
				if (callback != undefined)
					callback();
			});
		});
	});
	
	ftp.connect({host: this.prefix.config.gentoo.server, port:this.prefix.config.gentoo.port});
};

/*******************  FUNCTION  *********************/
/**
 * Save the gentoo DB into a local file.
**/
DbManager.prototype.saveGentooDb = function(list)
{
	console.log("Saving gentoo DB....");
	var files = [];
	for (var i in list)
	{
		if (list[i].type == '-')
			files.push(list[i].name);
	}
	fs.writeFileSync(this.prefix.getFile('/homelinux/packages/gentoo.json'),JSON.stringify(files));
	console.log("Gentoo DB saved");
};

/*********************  CONSTS  *********************/
module.exports = DbManager;
