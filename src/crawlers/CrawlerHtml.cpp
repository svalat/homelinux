/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <portability/System.hpp>
#include "CrawlerHtml.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
CrawlerHtml::CrawlerHtml(Prefix * prefix) 
		   :Crawler("html",prefix)
{
	//check if have node
	assume(System::hasCommand("node -v"),"You need to install nodejs to crawl html pages !");
}

/*******************  FUNCTION  *********************/
void CrawlerHtml::internalRun(std::string url)
{
	//gen filename
	static int id = 0;
	char fname[64];
	sprintf(fname,"hl-html-crawler-%1.html",++id);

	//gen command and run
	std::string out;
	if (System::runAndRead(out,"curl "+url+" -o "+std::string(fname)+" 2> /dev/null && node TODO"))
	{
		Helper::split(out,'\n',[this](const std::string & line){
			versions.push_back(line);
		});
	} else {
		HL_ERROR_AGR("Fail to fetch or scan html page for package %1 : %2")
			.arg(packageName)
			.arg(url)
			.end();
	}
}

}
