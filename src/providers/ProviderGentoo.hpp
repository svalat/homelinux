/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PREOVIDER_GENTOO_HPP
#define HL_PREOVIDER_GENTOO_HPP

/********************  HEADERS  *********************/
//std
//internal
#include <base/Helper.hpp>
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class ProviderGentoo : public Provider
{
	public:
		ProviderGentoo(Prefix * prefix, bool unitTest = false);
		virtual bool getPackage(PackageDef & out,const std::string & name);
		virtual void updateCache(void);
		virtual void updateDb(void);
		virtual std::string search(const std::string & name);
    private:
        void loadDb(void);
        std::string getGentooUrl(const std::string & filename);
    private:
        bool warn;
        StringList db;
        std::string server;
        std::string port;
        std::string distfiles;
        bool unitTest;
};

}

#endif //HL_PREOVIDER_GENTOO_HPP
