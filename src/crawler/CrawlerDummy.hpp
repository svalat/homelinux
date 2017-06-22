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
		CrawlerDummy(void) : Crawler("dummy"){};
	protected:
		void internalRun(const std::string & url) {};
};

}

#endif //HL_CRAWLER_DUMMY_HPP
