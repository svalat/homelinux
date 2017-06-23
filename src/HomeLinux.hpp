/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_HOME_LINUX_HPP
#define HL_HOME_LINUX_HPP

/********************  HEADERS  *********************/
//std
//internal
#include <base/Helper.hpp>
#include <base/Config.hpp>
#include <core/Prefix.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class HomeLinux
{
	public:
		HomeLinux(Config * config);
		~HomeLinux(void);
		void buildCache(void);
		void install(const StringList & packages);
		void pinstall(const StringList & packages);
		void printGenPackage(const std::string & package);
		void printGenPackageFull(const std::string & package);
		void printGenInstall(const std::string & package);
		void updateDb(void);
	private:
		void loadPrefix(bool onlyMaster);
		void askOk(void);
	private:
		Config * config;
		PrefixList prefixes;
		Prefix * master;
};

}

#endif //HL_HOME_LINUX_HPP
