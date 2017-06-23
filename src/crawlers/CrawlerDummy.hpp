/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_CRAWLER_DUMMY_HPP
#define HL_CRAWLER_DUMMY_HPP

/********************  HEADERS  *********************/
#include "Crawler.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class CrawlerDummy : public Crawler
{
	public:
		CrawlerDummy(Prefix * prefix) : Crawler("dummy",prefix){};
	protected:
		void internalRun(std::string url) {HL_DEBUG_ARG("CrawlerDumy","Crawling dummy...");};
};

}

#endif //HL_CRAWLER_DUMMY_HPP
