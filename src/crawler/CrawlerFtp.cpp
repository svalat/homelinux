/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <portability/System.hpp>
#include "CrawlerFtp.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerFtp::CrawlerFtp(void) 
		   :Crawler("ftp")
{
	
}

/*******************  FUNCTION  *********************/
void CrawlerFtp::internalRun(std::string url)
{
	//add / at end otherwise curl fail
	if (Helper::endBy(url,"/") == false)
		url += "/";

	//load
	std::string out;
	bool status = System::runAndRead(out,"LC_ALL=C curl "+url+" 2>/dev/null | awk '{print $9}'");
	if (!status)
		HL_ERROR_ARG("FTP failure for package %1 : %2").arg(packageName).arg(url).end();
	
	//loop over
	Helper::split(out,'\n',[this](const std::string fname){
		scanValue(fname);
	});
}

}
