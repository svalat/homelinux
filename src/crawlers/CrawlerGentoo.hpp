/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_CRAWLER_GENTOO_HPP
#define HL_CRAWLER_GENTOO_HPP

/********************  HEADERS  *********************/
#include "Crawler.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class CrawlerGentoo : public Crawler
{
	public:
		CrawlerGentoo(Prefix * prefix);
	protected:
		void loadDb(void);
		void internalRun(std::string url);
	private:
		StringList db;
};

}

#endif //HL_CRAWLER_GENTOO_HPP
