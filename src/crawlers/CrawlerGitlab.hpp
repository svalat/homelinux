/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_CRAWLER_GITLAB_HPP
#define HL_CRAWLER_GITLAB_HPP

/********************  HEADERS  *********************/
#include "Crawler.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class CrawlerGitlab : public Crawler
{
	public:
		CrawlerGitlab(Prefix * prefix);
	protected:
		void internalRun(std::string url);
};

}

#endif //HL_CRAWLER_GITLAB_HPP
