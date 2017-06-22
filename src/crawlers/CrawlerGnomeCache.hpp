/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_CRAWLER_GNOME_CACHE_HPP
#define HL_CRAWLER_GNOME_CACHE_HPP

/********************  HEADERS  *********************/
#include "Crawler.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class CrawlerGnomeCache : public Crawler
{
	public:
		CrawlerGnomeCache(Prefix * prefix);
	protected:
		void internalRun(std::string url);
};

}

#endif //HL_CRAWLER_GNOME_CACHE_HPP
