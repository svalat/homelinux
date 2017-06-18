/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_GITHUB_HPP
#define HL_GITHUB_HPP

/********************  HEADERS  *********************/
//std
//internal
#include <base/Helper.hpp>
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class ProviderGithub : public Provider
{
	public:
		ProviderGithub(Prefix * prefix);
		virtual bool getPackage(PackageDef & out,const std::string & name);
		virtual void updateCache(void);
		virtual void updateDb(void);
		virtual std::string search(const std::string & name);
};

}

#endif //HL_GITHUB_HPP
