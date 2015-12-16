var UserConfig = require('./libs/UserConfig');
var Prefix = require('./libs/Prefix');
var EnvSetup = require('./libs/EnvSetup');
var PackageBuilder = require('./libs/PackageBuilder');
var DbManager = require('./libs/DbManager');

var userConfig = new UserConfig();
// userConfig.print();

var prefix = new Prefix("/home/sebv/usr");
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
	var pack = new PackageBuilder(prefix,userConfig,process.argv[3]);
	console.log(pack.genScript())
} else if (process.argv[2] == "update-db") {
	var dbManager = new DbManager(prefix);
	dbManager.fetchGentoo();
}