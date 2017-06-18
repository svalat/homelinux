/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PROVIDER_MODELS_HPP
#define HL_PROVIDER_MODELS_HPP

/********************  HEADERS  *********************/
//std
//internal
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class ProviderModels : public Provider
{
	public:
		ProviderModels(Prefix * prefix);
		virtual ~ProviderModels(void);
		virtual bool getPackage(PackageDef & out,const std::string & name);
		virtual void updateCache(void);
		virtual void updateDb(void);
		virtual std::string search(const std::string & name);
	private:
		PackageDefMap cache;
};

}

#endif //HL_PROVIDER_MODELS_HPP
