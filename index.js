var UserConfig = require('./libs/UserConfig');
var Prefix = require('./libs/Prefix');
var EnvSetup = require('./libs/EnvSetup');

var userConfig = new UserConfig();
// userConfig.print();

var prefix = new Prefix("/home/sebv/usr");
// prefix.print();

if (process.argv[2] == "env")
{
	var envSetup = new EnvSetup(userConfig);
	prefix.fillEnv(envSetup);
	envSetup.print();
}