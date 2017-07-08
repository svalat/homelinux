/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cstdlib>
#include <iostream>
#include <fstream>
//internal
#include <portability/System.hpp>
#include <core/DepLoader.hpp>
#include <providers/ProviderHomelinux.hpp>
#include <core/EnvSetup.hpp>
#include "HomeLinux.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
HomeLinux::HomeLinux(Config * config)
{
	this->config = config;
}

/*******************  FUNCTION  *********************/
HomeLinux::~HomeLinux(void)
{
	//for (auto prefix : prefixes)
	forEach(PrefixList,prefix,prefixes)
		delete *prefix;
}


/*******************  FUNCTION  *********************/
void HomeLinux::buildCache(void)
{
	//setup
	loadPrefix(true);
	
	//run
	master->updateCache();
}

/*******************  FUNCTION  *********************/
void HomeLinux::crawl(void)
{
	//setup
	loadPrefix(true);
	
	//run
	master->crawl();
}

/*******************  FUNCTION  *********************/
void HomeLinux::updateDb(void)
{
	//setup
	loadPrefix(true);
	
	//run
	master->updateDb();
}

/*******************  FUNCTION  *********************/
void HomeLinux::printGenPackage(const std::string & package)
{
	//setup
	loadPrefix(true);
	
	//load
	PackageDef pack;
	master->loadPackage(pack,package);
	
	//send to stdout
	pack.save(std::cout);
}

/*******************  FUNCTION  *********************/
void HomeLinux::printGenPackageFull(const std::string & package)
{
	//setup
	loadPrefix(true);
	
	//load
	DepLoader loader(master);
	StringList lst;
	lst.push_back(package);
	loader.loadRequest(lst);
	
	//extract
	DepPackage * pack = loader.loadPackage(package,NULL,true);
	
	//send to stdout
	pack->def.save(std::cout);
}

/*******************  FUNCTION  *********************/
void HomeLinux::printVersions(const std::string & package)
{
	//setup
	loadPrefix(true);
	
	//load
	DepLoader loader(master);
	StringList lst;
	lst.push_back(package);
	loader.loadRequest(lst);
	
	//extract
	DepPackage * pack = loader.loadPackage(package,NULL,true);
	
	//send to stdout
	std::cout << Helper::join(pack->def.versions,' ') << std::endl;
}

/*******************  FUNCTION  *********************/
void HomeLinux::fetchVersions(const std::string & package)
{
	//setup
	loadPrefix(true);

	//vars
	ProviderHomelinux provider(master);

	//laod package to get full name
	PackageDef pack;
	if (provider.getPackage(pack,package) == false)
		HL_FATAL_ARG("Fail to load package : %1").arg(package).end();

	//build file path
	std::string shortName = pack.name.substr(3);//remove 'hl/'
	std::string path = master->getFilePath("/homelinux/packages/db/"+shortName+".json");

	//crawl
	StringMapList out;
	provider.crawl(1, 1,0,out,path);

	//print
	std::cout << Helper::join(out[pack.name],'\n') << std::endl;
}

/*******************  FUNCTION  *********************/
void HomeLinux::printGenInstall(const std::string & package)
{
	//setup
	loadPrefix(true);
	
	//load
	DepLoader loader(master);
	StringList lst;
	lst.push_back(package);
	loader.loadRequest(lst);
	
	//extract
	DepPackage * pack = loader.loadPackage(package,NULL,true);
	
	//send to stdout
	pack->def.genScript(std::cout,*master,false);
}


/*******************  FUNCTION  *********************/
void HomeLinux::install(const StringList & packages)
{
	//setup
	loadPrefix(true);
	
	//build loader
	DepLoader loader(master);
	
	//load
	loader.loadRequest(packages);
	
	//print
	loader.printList(std::cout);
	
	//ask if ok
	askOk();
	
	//build script
	char tmp[128];
	sprintf(tmp,"/tmp/homelinux-install-%d.sh",System::getPid());
	//mktemp(tmp);
	std::string path = tmp;
	std::ofstream out;
	out.open(path.c_str());
	loader.genScript(out,false);
	out.close();
	
	//run
	if (System::runCommand("bash "+path) != 0)
		HL_ERROR("Encounter errors !");
	
	//remove
	System::removeFile(path);
}

/*******************  FUNCTION  *********************/
void HomeLinux::pinstall(const StringList & packages)
{
	//setup
	loadPrefix(true);
	
	//build loader
	DepLoader loader(master);
	
	//load
	loader.loadRequest(packages);
	
	//print
	loader.printList(std::cout);
	
	//warn
	HL_WARNING("Caution, parallel install is experimental !");
	
	//ask if ok
	askOk();
	
	//setup dir
	char tmp[] = "/tmp/homelinux-parallel-install-XXXXXX";
	std::string path = mkdtemp(tmp);
	
	//gen scripts & makefile
	loader.genParallelScripts(path);
	
	//gen makefile
	std::ofstream out;
	out.open((path+"/Makefile").c_str());
	loader.genParallelMakefile(out,path);
	out.close();
	
	//run
	std::string opts = Helper::join(master->getConfig().flags["MAKEOPTS"],' ');
	assume(System::runCommand("make -C "+path+" "+opts) == 0,"Installation encounted an issue, look before this message !");
	
	//rm
	System::runCommand("rm -rfd "+path);
}

/*******************  FUNCTION  *********************/
void HomeLinux::unenv(void)
{
	//setup
	loadPrefix(false);
	
	//build env
	EnvSetup env(config);
	
	//setup
	env.loadCurrent();

	//loop on all prefix
	//for (auto it : prefixes)
	forEach(PrefixList,it,prefixes)
		(*it)->fillEnv(env);

	//removed
	env.removeExisting();

	//special
	env.loadModules(false);

	//print
	env.print();

	/*if (config->ccache)
		env.disableCCache();
	if (config->pyEnv)
		env.disablePyEnv();*/
}

/*******************  FUNCTION  *********************/
void HomeLinux::env(void)
{
	//setup
	loadPrefix(false);

	//build env
	EnvSetup env(config);
	
	//setup
	env.loadCurrent();

	//loop on all prefix
	//for (auto it : prefixes)
	forEach(PrefixList,it,prefixes)
		(*it)->fillEnv(env);

	//print
	env.print();

	//special
	env.loadModules(true);
	if (config->ccache)
		env.enableCCache();
	if (config->pyEnv)
		env.enablePyEnv();
}

/*******************  FUNCTION  *********************/
void HomeLinux::switchEnv(const std::string & prefixPath)
{
	//unenv
	this->unenv();

	//replace prefix
	config->prefix.clear();
	config->prefix.push_back(prefixPath);

	//load env
	this->env();
}

/*******************  FUNCTION  *********************/
void HomeLinux::loadPrefix(bool onlyMaster)
{
	//check
	assume(config->prefix.empty() == false,"You need to provide at least one prefix in user config file, check ~/.homelinux.json");
	
	//load
	bool isMaster(true);
	//for (auto & path : config->prefix)
	forEach(StringList,path,config->prefix)
	{
		//build
		prefixes.push_back(new Prefix(config,*path,isMaster));
		
		//case
		if (onlyMaster)
			break;
		else
			isMaster = false;
	}
	
	//reorder
	prefixes.reverse();
	
	//take master
	master = prefixes.back();
}

/*******************  FUNCTION  *********************/
bool HomeLinux::isPackInstalled(const std::string & packageName)
{
	//load
	loadPrefix(false);

	//search on all prefix
	//for (auto pref : prefixes)
	forEach(PrefixList,pref,prefixes)
		if ((*pref)->isInstalled(packageName))
			return true;
	
	//not found
	return false;
}

/*******************  FUNCTION  *********************/
std::string HomeLinux::prefixOf(const std::string & packageName)
{
	//load
	loadPrefix(false);

	//search on all prefix
	//for (auto pref : prefixes)
	forEach(PrefixList,pref,prefixes)
	{
		std::string tmp = (*pref)->prefixOf(packageName);
		if (tmp.empty() == false)
			return tmp;
	}
	
	//not found
	return "/usr";
}

/*******************  FUNCTION  *********************/
void HomeLinux::ls(void)
{
	//load
	loadPrefix(false);

	//revers
	prefixes.reverse();

	//loop
	//for (auto prefix : prefixes)
	forEach(PrefixList,prefix,prefixes)
		(*prefix)->ls();
}

/*******************  FUNCTION  *********************/
void HomeLinux::search(const std::string & value)
{
	//load
	loadPrefix(true);

	//search in master
	master->search(std::cout,value);
}

/*******************  FUNCTION  *********************/
void HomeLinux::exportConfig(void)
{
	//load
	loadPrefix(true);

	//load all verisons
	master->exportConfig();
}

/*******************  FUNCTION  *********************/
int HomeLinux::jump(StringList command)
{
	//if command empty
	if (command.empty())
		command.push_back("$SHELL");
		
	//hl path
	loadPrefix(true);
	std::string path = master->getFilePath("/bin/hl");
	
	//launch
	return System::runCommand("eval \"$("+path+" env)\" && "+Helper::join(command,' '));
}

/*******************  FUNCTION  *********************/
void HomeLinux::protect(const std::string & value)
{
	//load
	loadPrefix(true);

	//print
	std::cout << "Protected command " << value << std::endl;

	//build
	System::symlink("hl-protected",master->getFilePath("/bin/protected/"+value));
}

/*******************  FUNCTION  *********************/
void HomeLinux::validate(void)
{
	//load
	loadPrefix(true);

	//apply
	master->validate();
}

/*******************  FUNCTION  *********************/
void HomeLinux::askOk(void)
{
	std::string buf = "?";
	while (buf != "y" && buf != "n" && buf.empty() == false)
	{
		std::cout << "Continue (Y/n) ? ";
		std::cin >> buf;
	}
	
	if (buf == "n")
		exit(0);
}

}
