/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PROVIDER_URLS_HPP
#define HL_PROVIDER_URLS_HPP

/********************  HEADERS  *********************/
//std
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class ProviderUrls : public Provider
{
	public:
		ProviderUrls(Prefix * prefix);
		virtual ~ProviderUrls(void);
		virtual bool getPackage(PackageDef & out,const std::string & name);
		virtual void updateCache(void);
		virtual void updateDb(void);
		virtual std::string search(const std::string & name);
	private:
		void loadDb(void);
		void loadDb(const std::string & path);
	private:
		StringList db;
};

}

#endif //HL_PROVIDER_URLS_HPP
