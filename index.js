/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

var HomeLinuxManager = require('./libs/HomeLinuxManager');

//load homelinux configs
var hlManager = new HomeLinuxManager();
var homelinux = hlManager.homelinux;

//extract command
var command = process.argv[2];

//apply
switch(command)
{
	case 'env':
		hlManager.printEnvSetup();
		break;
	case 'unenv':
		hlManager.printUnsetEnv();
		break;
	case 'switch':
		hlManager.printSwitchPrefix(process.argv[3]);
		break;
	case 'install':
		hlManager.installOnPackage(process.argv[3]);
		break;
	case 'pinstall':
		hlManager.printParalelInstallMakefile(process.argv[3],process.argv.slice(4,process.argv.length));
		break;
	case 'gen-package':
		hlManager.printPackage(process.argv[3]);
		break;
	case 'gen-full-package':
		hlManager.printFullPackage(process.argv[3]);
		break;
	case 'gen-install':
		hlManager.printInstallScript(process.argv.slice(3,process.argv.length));
		break;
	case 'gen-install-p':
		hlManager.printInstallPScript(process.argv.slice(3,process.argv.length));
		break;
	case 'install-ls':
		hlManager.printInstallList(process.argv.slice(3,process.argv.length));
		break;
	case 'gen-uninstall':
		hlManager.printUninstallScript(process.argv.slice(3,process.argv.length));
		break;
	case 'update-db':
		hlManager.updateDb();
		//break; don't break, we also do update-cache
	case 'update-cache':
		hlManager.updateCache();
		break;
	case 'fetch-versions':
		hlManager.printFetchedVersions(process.argv[3]);
		break;
	case 'versions':
		hlManager.printVersions(process.argv[3]);
		break;
	case 'prefix-of':
		hlManager.printPrefixOf(process.argv[3]);
		break;
	case 'is-pack-installed':
		hlManager.testPackInstalled(process.argv[3]);
		break;
	case 'ls':
		hlManager.listInstalled();
		break;
	case 'search':
		hlManager.search(process.argv[3]);
		break;
	case 'export':
		hlManager.export();
		break;
	default:
		console.error("Invalid command : '"+homelinux.userConfig,command+"', please check --help|-h");
		break;
}
