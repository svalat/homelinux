/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_CRAWLER_HTML_HPP
#define HL_CRAWLER_HTML_HPP

/********************  HEADERS  *********************/
#include "Crawler.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class CrawlerHtml : public Crawler
{
	public:
		CrawlerHtml(Prefix * prefix);
	protected:
		void internalRun(std::string url);
	private:
		std::string nodeScript;
};

}

#endif //HL_CRAWLER_HTML_HPP
