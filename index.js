/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

var UserConfig = require('./libs/UserConfig');
var Prefix = require('./libs/Prefix');
var EnvSetup = require('./libs/EnvSetup');
var PackageBuilder = require('./libs/PackageBuilder');
var DbManager = require('./libs/DbManager');
var VersionFetcher = require('./libs/VersionFetcher');
var depsLoader = require('./libs/DepsLoader');

var userConfig = new UserConfig();
// userConfig.print();

var prefix = new Prefix(userConfig.config.prefix);
// prefix.print();

if (process.argv[2] == "env")
{
	var envSetup = new EnvSetup(userConfig);
	prefix.fillEnv(envSetup);
	envSetup.print();
} else if (process.argv[2] == "install") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	//pack.printDebug();
	pack.install();
	//console.log(pack.genScript())
} else if (process.argv[2] == "gen-package") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	console.log(pack.origPack)
} else if (process.argv[2] == "gen-full-package") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	console.log(pack.pack)
} else if (process.argv[2] == "gen-install") {
	var packs = new depsLoader(prefix,userConfig,process.argv.slice(3,process.argv.length));
	console.log(packs.genScript())
} else if (process.argv[2] == "install-ls") {
	var packs = new depsLoader(prefix,userConfig,process.argv.slice(3,process.argv.length));
	packs.printList();
} else if (process.argv[2] == "update-db") {
	var dbManager = new DbManager(prefix);
	dbManager.fetchGentoo();
} else if (process.argv[2] == "fetch-versions") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3],false);
	var fetcher = new VersionFetcher();
	fetcher.fetchVersions(pack);
} else if (process.argv[2] == "prefix-of") {
	console.log(userConfig.config.prefix);
}
