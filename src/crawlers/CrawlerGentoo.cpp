/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include <core/Prefix.hpp>
#include "CrawlerGentoo.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerGentoo::CrawlerGentoo(Prefix * prefix) 
		   :Crawler("gentoo",prefix)
{
	loadDb();
}

/*******************  FUNCTION  *********************/
void CrawlerGentoo::loadDb(void)
{
	//vars
	std::string path = prefix->getFilePath("/homelinux/packages/gentoo.lst");

	//load
	std::string file = System::loadFile(path);

	//parse
	db = Helper::split(file,'\n');
}

/*******************  FUNCTION  *********************/
void CrawlerGentoo::internalRun(std::string url)
{
	//debug
	HL_DEBUG_ARG("CrawlerGentoo","Crawling %1").arg(url).end();
	
	//scan
	//for (auto & it : db)
	forEach(StringList,it,db)
		scanValue(*it);
}

}
