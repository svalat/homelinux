/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_HOMELINUX_HPP
#define HL_HOMELINUX_HPP

/********************  HEADERS  *********************/
//std
//internal
#include <base/Helper.hpp>
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class ProviderHomelinux : public Provider
{
	public:
		ProviderHomelinux(Prefix * prefix);
		virtual ~ProviderHomelinux(void);
		virtual bool getPackage(PackageDef & out,const std::string & name);
		virtual void updateCache(void);
		virtual void updateDb(void);
		virtual std::string search(const std::string & name);
	private:
		void loadCache(void);
		void loadVersions(void);
		std::string searchInCache(const std::string & name);
	private:
		StringList cache;
		StringMapList versions;
		PackageDefMap packageCache;
		bool warn;
};

}

#endif //HL_HOMELINUX_HPP
