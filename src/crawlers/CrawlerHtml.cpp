/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <sstream>
#include <base/Debug.hpp>
#include <portability/System.hpp>
#include <core/Prefix.hpp>
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
	sprintf(fname,"hl-html-crawler-%d.html",++id);

	//get node path
	std::string nodeScript = prefix->getFilePath("/share/homelinux/html-link-extractor/index.js");
	if (System::fileExist(nodeScript) == false)
	{
		nodeScript = EXTRACTOR_SOURCE_PATH "/index.js";
		assumeArg(System::fileExist(nodeScript),"Fail to find html extracttor script : %1").arg(nodeScript).end();
	}

	//prepare regexp
	std::string reg = txtRegexp;
	Helper::replaceInPlace(reg,"\\","\\\\");
	Helper::replaceInPlace(reg,"\"","\\\"");
	Helper::replaceInPlace(reg,"$","\\$");

	//gen command
	std::stringstream cmd;
	cmd << "curl " << url << " -o " << fname << " 2> /dev/null"
		<< " && node " << nodeScript << " " << fname << " a " << '\"' << reg << '\"';

	//help debug
	HL_DEBUG_ARG("CrawlerHtml","Call : %1").arg(cmd.str()).end();

	//run
	std::string out;
	if (System::runAndRead(out,cmd.str()))
	{
		Helper::split(out,'\n',[this](const std::string & line){
			versions.push_back(line);
		});
	} else {
		HL_ERROR_ARG("Fail to fetch or scan html page for package %1 : %2")
			.arg(packageName)
			.arg(url)
			.end();
	}
}

}
