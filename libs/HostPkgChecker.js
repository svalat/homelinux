/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var child_process = require('child_process');

/*******************  FUNCTION  *********************/
function HostPkgChecker(userConfig)
{
    this.userConfig = userConfig;
}

/*******************  FUNCTION  *********************/
/**
 * Check if present by using the 'default' system, so only
 * considering the host.default entry from the package.
 * @param p Package to check.
**/
HostPkgChecker.prototype.presentOnSystemDefault = function(p)
{
	if (p.pack.host['default'] == undefined)
		return false;
	else
		return p.pack.host['default'];
}

/*******************  FUNCTION  *********************/
/**
 * Check if the package is installed into a Debian 8 host system.
 * This funcction made the check by using the dpkg command.
 * CAUTION, it require child_process.execSync from nodejs which
 * is not provided on old versions.
 * @param p package to check.
**/
HostPkgChecker.prototype.presentOnSystemDebian8 = function(p)
{
	var h = [];
	
	//package one
	if (p.pack.host != undefined)
		h = p.pack.host['debian8'];

	//load from separate file
	if (this.hostsRefs[p.pack.name] != undefined)
		h = this.hostsRefs[p.pack.name];
	
	//not defined, consider not provided
	if (h == undefined)
		return false;
	
	//is not provided
	if (h === false)
		return false;
	
	//console.log("Check ncurses on host : "+h);
	
	//check in list
	for (var i in h)
	{
		try {
			//console.error("Check with dpkg "+h[i]);
			var res = child_process.execSync('dpkg -s '+h[i]+' 2> /dev/null');
		} catch (e) {
			return false;
		}
	}
	return true;
};

/*******************  FUNCTION  *********************/
/**
 * Check if the given package is installed onto the system.
 * This function is mostly a switch on the system type.
**/
HostPkgChecker.prototype.presentOnSystem = function(p)
{
	if (this.userConfig.config.host == 'default')
	{
		return this.presentOnSystemDefault(p);
	} else if (this.userConfig.config.host == 'debian8') {
		return this.presentOnSystemDebian8(p);
	} else {
		throw "Unsupposed host system to check deps, please use default in that case !";
	}
};

module.exports = HostPkgChecker;