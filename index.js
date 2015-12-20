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

var command = process.argv[2];

if (command == "env")
{
	var envSetup = new EnvSetup(userConfig);
	prefix.fillEnv(envSetup);
	envSetup.print();
} else if (command == "install") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	//pack.printDebug();
	pack.install();
	//console.log(pack.genScript())
} else if (command == "gen-package") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	console.log(pack.origPack)
} else if (command == "gen-full-package") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	console.log(pack.pack)
} else if (command == "gen-install") {
	var packs = new depsLoader(prefix,userConfig,process.argv.slice(3,process.argv.length));
	console.log(packs.genScript())
} else if (command == "install-ls") {
	var packs = new depsLoader(prefix,userConfig,process.argv.slice(3,process.argv.length));
	packs.printList();
} else if (command == "update-db") {
	//gentoo db
	var dbManager = new DbManager(prefix);
	dbManager.fetchGentoo();
	//versions
	var fetcher = new VersionFetcher();
	fetcher.fetchAll(prefix,userConfig);
} else if (command == "fetch-versions") {
	var fetcher = new VersionFetcher();
	fetcher.fetchAll(prefix,userConfig);
} else if (command == "prefix-of") {
	console.log(userConfig.config.prefix);
} else if (command == "is-pack-installed") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	if (pack.isInstalled() == false)
	{
		console.log(process.argv[3]+" not installed");
		process.exit(1);
	} else {
		console.log(process.argv[3]+" installed");
	}
} else if (command == 'ls') {
	prefix.listInstalled();
} else if (command == 'search') {
	prefix.search(process.argv[3]);
} else {
	console.error("Invalid command : '"+userConfig,command+"', please check --help|-h");
}
