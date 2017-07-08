/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include <core/Prefix.hpp>
#include <cstdio>
#include "CrawlerFtp.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerFtp::CrawlerFtp(Prefix * prefix) 
		   :Crawler("ftp",prefix)
{
	
}

/*******************  FUNCTION  *********************/
void CrawlerFtp::internalRun(std::string url)
{
	//check
	if (Helper::startBy(url,"ftp://") == false && Helper::startBy(url,"sftp://") == false)
	{
		HL_ERROR_ARG("Invlid URL for FTP usage : %1").arg(url).end();
		return;
	}
	
	//add / at end otherwise curl fail
	if (Helper::endBy(url,"/") == false)
		url += "/";
		
	//debug
	HL_DEBUG_ARG("CrawlerFtp","Crawling %1").arg(url).end();

	//load
	std::string out;
	char tmp[2048];
	sprintf(tmp,"%d",prefix->getUserConfig().crawlerTimeout);
	bool status = System::runAndRead(out,"LC_ALL=C curl -m "+std::string(tmp)+" "+url+" 2>/dev/null");

	//check status
	if (!status)
	{
		HL_ERROR_ARG("FTP failure for package %1 : %2").arg(packageName).arg(url).end();
	} else {
		//scan lines
		StringList lst = Helper::split(out,'\n');
		forEach(StringList,line,lst)
			scanValue(Helper::last(*line,' '));
	}
}

}
