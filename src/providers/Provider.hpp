/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_PROVIDER_HPP
#define HL_PROVIDER_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <map>
//internal
#include <core/PackageDef.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************* TYPES ************************/
//to voind infinit loop include
class Prefix;
typedef std::map<std::string,PackageDef> PackageDefMap;

/*********************  CLASS  **********************/
class Provider
{
	public:
		Provider(const std::string & name,Prefix * prefix);
		virtual ~Provider(void);
		const std::string getName(void);
		virtual bool getPackage(PackageDef & out,const std::string & name) = 0;
		virtual void updateCache(void) = 0;
		virtual void updateDb(void) = 0;
		virtual std::string search(const std::string & name) = 0;
	protected:
		Prefix * prefix;
	private:
		std::string name;
};

}

#endif //HL_PROVIDER_HPP
