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
	for (auto prefix : prefixes)
		delete prefix;
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
	char tmp[] = "/tmp/homelinux-install-XXXXXX.sh";
	//mktemp(tmp);
	std::string path = tmp;
	std::ofstream out;
	out.open(path);
	loader.genScript(out,false);
	out.close();
	
	//run
	if (System::runCommand("bash "+path) != 0)
		HL_ERROR("Encounter errors !");
	
	//remove
	System::removeFile(path);
}

/*******************  FUNCTION  *********************/
void HomeLinux::loadPrefix(bool onlyMaster)
{
	//check
	assume(config->prefix.empty() == false,"You need to provide at least one prefix in user config file, check ~/.homelinux.json");
	
	//load
	bool isMaster(true);
	for (auto & path : config->prefix)
	{
		//build
		prefixes.push_back(new Prefix(config,path,isMaster));
		
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
