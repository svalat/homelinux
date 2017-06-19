/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PROVIDER_DEBIAN_HPP
#define HL_PROVIDER_DEBIAN_HPP

/********************  HEADERS  *********************/
//std
//internal
#include <base/Helper.hpp>
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  STRUCT  *********************/
struct DebianDbEntry
{
	std::string name;
	std::string version;
	std::string deps;
	std::string homepage;
	StringList packages;
	std::string dir;
	std::string section;
};

/*********************  TYPES  **********************/
typedef std::map<std::string,DebianDbEntry> DebianDb;

/*********************  CLASS  **********************/
class ProviderDebian : public Provider
{
	public:
		ProviderDebian(Prefix * prefix, bool unitTest = false);
		virtual bool getPackage(PackageDef & out,const std::string & name);
		virtual void updateCache(void);
		virtual void updateDb(void);
		virtual std::string search(const std::string & name);
	private:
		void loadDb(void);
		void saveDb(void);
	private:
		DebianDb db;
		bool unitTest;
		bool warn;
};

}

#endif //HL_PROVIDER_DEBIAN_HPP
