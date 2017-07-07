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
	
	//get node path
	std::string path = prefix->getFilePath("/share/homelinux/html-link-extractor/");
	nodeScript = path + "index.js";
	if (System::fileExist(nodeScript) == false)
	{
		path = EXTRACTOR_SOURCE_PATH;
		nodeScript = path + "/index.js";
		assumeArg(System::fileExist(nodeScript),"Fail to find html extracttor script : %1").arg(nodeScript).end();
	}

	//check node_modules
	if (System::fileExist(path+"/node_modules") == false)
		System::runCommand("cd "+path+" && npm install");
}

/*******************  FUNCTION  *********************/
void CrawlerHtml::internalRun(std::string url)
{
	//gen filename
	static int id = 0;
	char fname[128];
	sprintf(fname,"/tmp/hl-html-crawler-%d.html",++id);
	
	//debug
	HL_DEBUG_ARG("CrawlerHtml","Crawling %1").arg(url).end();

	//prepare regexp
	std::string reg = txtRegexp;
	Helper::replaceInPlace(reg,"\\","\\\\");
	Helper::replaceInPlace(reg,"\"","\\\"");
	Helper::replaceInPlace(reg,"$","\\$");
	reg = "^[ \n]*" + reg + "[ \n]*$";
	
	//tag
	std::string tag = options.get("tag","a").asString();

	//gen command
	std::stringstream cmd;
	char tmp[32];
	sprintf(tmp,"%d",prefix->getUserConfig().crawlerTimeout);
	cmd << "curl -m " << tmp << " -L " << url << " -o " << fname << " 2> /dev/null"
		<< " && node " << nodeScript << " " << fname << " " << tag << " " << '\"' << reg << '\"';

	//help debug
	HL_DEBUG_ARG("CrawlerHtmlCmd","Call : %1").arg(cmd.str()).end();

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

	//erase
	System::removeFile(fname);
}

}
