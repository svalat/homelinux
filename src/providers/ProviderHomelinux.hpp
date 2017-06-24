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
#include <crawlers/Crawler.hpp>
#include "Provider.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  TYPES  **********************/
typedef std::map<int,std::map<std::string,Crawler *>> CrawlerMap;

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
		void crawl(int cur, int cnt,int threadId,StringMapList & out,const std::string & path);
	private:
		void loadCache(void);
		void loadVersions(void);
		Crawler * getCrawler(int threadId,const std::string & name,const std::string & packageName = "");
		std::string searchInCache(const std::string & name);
	private:
		StringList cache;
		StringMapList versions;
		PackageDefMap packageCache;
		bool warn;
		CrawlerMap crawlers;
};

}

#endif //HL_HOMELINUX_HPP
