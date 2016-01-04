/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
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
	envSetup.loadModules(true);
	envSetup.enableCCache();
	envSetup.enablePyEnv();
} else if (command == "unenv") {
	var envSetup = new EnvSetup(userConfig);
	envSetup.removeExisting();
	envSetup.loadModules(false);
	envSetup.print();
} else if (command == "switch") {
	var envSetup = new EnvSetup(userConfig);
	envSetup.removeExisting();
	//load new
	var prefix = new Prefix(process.argv[3]);
	prefix.fillEnv(envSetup);
	envSetup.enableCCache();
	envSetup.print();
} else if (command == "install") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	//pack.printDebug();
	pack.install();
	//console.log(pack.genScript())
} else if (command == "pinstall") {
	var packs = new depsLoader(prefix,userConfig,process.argv.slice(4,process.argv.length));
	packs.genParallelScripts(process.argv[3]);
	console.log(packs.genParallelMakefile(process.argv[3]));
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
} else if (command == "gen-uninstall") {
	var packs = new depsLoader(prefix,userConfig,process.argv.slice(3,process.argv.length));
	console.log(packs.genScript(true))
} else if (command == "update-db") {
	//gentoo db
	var dbManager = new DbManager(prefix);
	dbManager.fetchGentoo(function() {
		//versions
		var fetcher = new VersionFetcher();
		fetcher.fetchAll(prefix,userConfig);
	});
} else if (command == "fetch-versions") {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3],false);
	var fetcher = new VersionFetcher();
	fetcher.fetchVersions(pack,function(){
		console.log(pack.pack.versions);
	});
} else if (command == "prefix-of") {
	var ret = prefix.prefixOfPackage(userConfig,process.argv[3]);
	if (ret == null)
	{
		console.log("/usr");
		process.exit(1);
	} else {
		console.log(userConfig.config.prefix);
	}
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
} else if (command == 'export') {
	prefix.export(userConfig);
} else if (command == 'prefix-of') {
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	var pref = prefix.getPrefixOf(pack);
	if (pref != null)
	{
		console.log(pref.prefix);
	} else {
		console.error("Failed to find package "+pack.getNameSlot());
		process.exit(1);
	}
} else {
	console.error("Invalid command : '"+userConfig,command+"', please check --help|-h");
}
