/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//must be on top
#include "config.h"
//std
#include <iostream>
#include <cstdlib>
//internal
#include "HomeLinux.hpp"

/**********************  USING  *********************/
using namespace hl;
using namespace std;

/********************  CONSTS  **********************/
static const char gblHelp[] = "hl {OPTIONS} {COMMAND} [ARGS...]\n\
Options:\n\
    -p/--prefix PATH  : For usage of given prefix instead user config file.\n\
    -v COMPONENTS     : In debug mode, enable components verbosity as a list\n\
                        of names. Can be 'all'. Use 'help' to get list of\n\
                        components.\n\
    -h/--help         : Display this help message\n\
    --version         : Print version of Homelinux.\n\
\n\
Commandsl:\n\
    build-cache       : Rebuild the cache file to convert short\n\
                        package name to long names.\n\
    help              : Show this help message.\n\
    install           : Install the requested packages. Require package \n\
                        name as argument.\n\
    pinstall          : Same than install but building multiple packages at same time\n\
                        CAUITION, it is likely to fail if the dep-tree is not strict enougth.\n\
                        We recommend to enable the +gentoo flag to use this build mode.\n\
    uninstall         : Uninstall the given packages (only if you enable usage of GNU stow in \n\
                        prefix config).\n\
    gen-package       : Pint generated package.\n\
    gen-full-package  : Print fully generated package.\n\
    gen-install       : Generate install script to forward to bash.\n\
    update-db         : Sync the gentoo DB for unknown packages.\n\
    versions          : Print the list of available version for the given package.\n\
    fetch-versions    : Fetch versions of given package (work only\n\
                        for db packages.\n\
    env               : Setup the environement variable to load.\n\
    unenv             : Reset the environnement variables by removing the homelinux entries.\n\
    switch            : Switch the environnement variables to the given prefix.\n\
    is-pack-installed : Check if the given package is installed or not.\n\
    prefix-of         : Return the prefix where the package is installed or empty if not.\n\
    ls                : List installed packages.\n\
    search            : Search for matching packages in the available dbs.\n\
    export            : Export the current config and list of packages.\n\
    jump              : Start a shell configured for the hl prefix. You\n\
                        can optionally provide a command to run into this shell.\n\
";

/*******************  FUNCTION  *********************/
int main(int argc, char ** argv)
{
	//setup config
	Config config(true);
	config.parseArgs(argc,(const char **) argv);
	
	//show help
	if (config.showHelp || config.command == "help")
	{
		cout << gblHelp;
		return EXIT_SUCCESS;
	}
	
	//check command
	assume(config.command.empty() == false,"You need to provide a command to run !");
	
	
	//build homelinux
	HomeLinux homelinux(&config);
	
	//run command
	if (config.command == "build-cache")
	{
		homelinux.buildCache();
	} else if (config.command == "install") {
		homelinux.install(config.args);
	} else if (config.command == "pinstall") {
		homelinux.pinstall(config.args);
	} else if (config.command == "uninstall") {
		HL_FATAL("Uninstall is not supported yet !");
	} else if (config.command == "gen-package") {
		assume(config.args.size() == 1,"Command gen-package expect only one argument !");
		homelinux.printGenPackage(config.args.front());
	} else if (config.command == "gen-package-full") {
		assume(config.args.size() == 1,"Command gen-package-full expect only one argument !");
		homelinux.printGenPackageFull(config.args.front());
	} else if (config.command == "gen-install") {
		assume(config.args.size() == 1,"Command gen-install expect only one argument !");
		homelinux.printGenInstall(config.args.front());
	} else if (config.command == "update-db") {
		assume(config.args.size() == 0,"Command update-db expect no arguments !");
		homelinux.updateDb();
	} else if (config.command == "versions") {
		assume(config.args.size() == 1,"Command versions expect only one argument !");
		homelinux.printVersions(config.args.front());
	} else if (config.command == "fetch-versions") {
		assume(config.args.size() == 1,"Command versions expect only one argument !");
		homelinux.fetchVersions(config.args.front());
	} else if (config.command == "env") {
		assume(config.args.size() == 0,"Command env expect no arguments !");
		homelinux.env();
	} else if (config.command == "unenv") {
		assume(config.args.size() == 0,"Command unenv expect no arguments !");
		homelinux.unenv();
	} else {
		HL_FATAL_ARG("Invalid command : %1").arg(config.command).end();
	}
	
	return EXIT_SUCCESS;
}
