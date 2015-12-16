var UserConfig = require('./libs/UserConfig');
var Prefix = require('./libs/Prefix');
var EnvSetup = require('./libs/EnvSetup');
var PackageBuilder = require('./libs/PackageBuilder');

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
	console.log(pack.genScript())
}