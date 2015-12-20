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

/*********************  CLASS  **********************/
function DbManager(prefix)
{
	this.prefix = prefix;
}

/*******************  FUNCTION  *********************/
DbManager.prototype.fetchGentoo = function(callback)
{
	var ftp = new FtpClient()
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
}

/*******************  FUNCTION  *********************/
DbManager.prototype.saveGentooDb = function(list)
{
	console.log("Saving gentoo DB....");
	var files = [];
	for (var i in list)
	{
		if (list[i].type == '-')
			files.push(list[i].name);
	}
	fs.writeFileSync(this.prefix.getFile('/share/homelinux/packages/db/gentoo.json'),JSON.stringify(files));
	console.log("Gentoo DB saved");
}

/*********************  CONSTS  *********************/
module.exports = DbManager;