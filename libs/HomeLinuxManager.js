/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var HomeLinux = require('./HomeLinux');
var EnvSetup = require('./EnvSetup');
var DepsLoader = require('./DepsLoader');
var DbManager = require('./DbManager');
var PackageBuilder = require('./PackageBuilder');
var VersionFetcher = require('./VersionFetcher');

/***********************  DOC  **********************/
/**
 * This object handle all the action HomeLinux can provide
 * It is used directly by the main script to start the avaialble actions.
**/

/*********************  CLASS  **********************/
/**
 * Basic constructor which only load the HomeLinux setup
**/
function HomeLinuxManager()
{
	this.homelinux = new HomeLinux();
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printEnvSetup = function(envSetup)
{
	//env object
	if (envSetup == undefined)
		envSetup = new EnvSetup(this.homelinux.userConfig);
	
	//fill with prefixes
	this.homelinux.fillEnv(envSetup);
	
	//print
	envSetup.print();
	
	//some extra features
	envSetup.loadModules(true);
	envSetup.enableCCache();
	envSetup.enablePyEnv();
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printUnsetEnv = function(envSetup)
{
	//env object
	if (envSetup == undefined)
		envSetup = new EnvSetup(this.homelinux.userConfig);
	
	//build removal
	envSetup.removeExisting();
	
	//unload modules
	envSetup.loadModules(false);
	
	//print
	envSetup.print();
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printSwitchPrefix = function(targetPrefix)
{
	//env object
	var envSetup = new EnvSetup(this.homelinux.userConfig);
	
	//unset
	this.printUnsetEnv(envSetup);
	
	//load new
	this.homelinux.switchPrefix(targetPrefix);
	
	//print setup
	this.printEnvSetup(envSetup);
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.installOnPackage = function(packName)
{
	var pack = new PackageBuilder(this.homelinux.prefix,this.homelinux.userConfig,packName);
	pack.install();
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printParalelInstallMakefile = function(tmpDir,packageList)
{
	//build deps
	var packs = new DepsLoader(this.homelinux.prefix,this.homelinux.userConfig,packageList);
	
	//gen parallel scripts
	packs.genParallelScripts(tmpDir);
	
	//gen makefile and print it
	console.log(packs.genParallelMakefile(tmpDir));
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printPackage = function(packName)
{
	var pack = new PackageBuilder(this.homelinux.prefix,this.homelinux.userConfig,packName);
	console.log(pack.origPack);
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printFullPackage = function(packName)
{
	var pack = new PackageBuilder(this.homelinux.prefix,this.homelinux.userConfig,packName);
	console.log(pack.pack)
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printInstallList = function(packageList)
{
	var packs = new DepsLoader(this.homelinux.prefix,this.homelinux.userConfig,packageList);
	packs.printList();
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printInstallScript = function(packageList)
{
	var packs = new DepsLoader(this.homelinux.prefix,this.homelinux.userConfig,packageList);
	console.log(packs.genScript())
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printUninstallScript = function(packageList)
{
	var packs = new DepsLoader(this.homelinux.prefix,this.homelinux.userConfig,packageList);
	console.log(packs.genScript(true))
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.updateCache = function()
{
	var self = this;
	this.homelinux.prefix.updateCache(function(err) {});
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.updateDb = function()
{
	var self = this;
	//gentoo db
	this.homelinux.prefix.updateDb(function(err) {
		//versions
		var fetcher = new VersionFetcher();
		fetcher.fetchAll(self.homelinux.mainPrefix,self.homelinux.userConfig);
	});
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printFetchedVersions = function(packName)
{
	var pack = new PackageBuilder(this.homelinux.prefix,this.homelinux.userConfig,packName,false);
	var fetcher = new VersionFetcher();
	fetcher.fetchVersions(pack,function(){
		console.log(pack.pack.versions);
	});
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.printPrefixOf = function(packName)
{
	var ret = this.homelinux.prefix.prefixOfPackage(this.homelinux.userConfig,packName);
	if (ret == null)
	{
		console.log("/usr");
		process.exit(1);
	} else {
		console.log(this.homelinux.userConfig.config.prefix);
	}
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.testPackInstalled = function(packName)
{
	var pack = new PackageBuilder(this.homelinux.prefix,this.homelinux.userConfig,packName);
	if (pack.isInstalled() == false)
	{
		console.log(packName+" not installed");
		process.exit(1);
	} else {
		console.log(packName+" installed");
	}
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.listInstalled = function()
{
	this.homelinux.listInstalled();
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.search = function(packName)
{
	this.homelinux.search(packName);
}

/*******************  FUNCTION  *********************/
HomeLinuxManager.prototype.export = function()
{
	this.homelinux.export();
}

/********************  GLOBALS  *********************/
module.exports = HomeLinuxManager;
