/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
//internal
#include <portability/System.hpp>
#include <core/Prefix.hpp>
#include <base/Helper.hpp>
#include <base/Colors.hpp>
#include <re2/re2.h>
#include "ProviderGentoo.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CONST ***********************/
static const char * gblExt[] = {"tar.bz2","tar.xz","tar.gz","zip","tgz"};

/*******************  FUNCTION  *********************/
ProviderGentoo::ProviderGentoo(Prefix * prefix, bool unitTest)
			   :Provider("gentoo",prefix)
{
	this->warn = true;
	this->unitTest = unitTest;

	//load config
	const Json::Value & json = prefix->getConfig().gentoo;
	server = json.get("server","gentoo.mirrors.ovh.net").asString();
	port = json.get("port",21).asString();
	distfiles = json.get("distfiles","gentoo-distfiles/distfiles").asString();
}

/*******************  FUNCTION  *********************/
void ProviderGentoo::loadDb(void)
{
	//already done
	if (db.empty() == false)
		return;
	
	//load file
	std::string path = prefix->getFilePath("/homelinux/packages/gentoo.lst");
	
	//check if exist
	if (System::fileExist(path))
	{
		//load
		std::string file = System::loadFile(path);
		
		//for each line
		Helper::split(file,'\n',[this](const std::string & line){
			this->db.push_back(line);
		});
	} else {
		if (warn)
			HL_ERROR("No gentoo list file available, consider to call 'hl update-db' at least once");
		warn = false;
	}
}

/*******************  FUNCTION  *********************/
bool ProviderGentoo::getPackage(PackageDef & out,const std::string & name)
{
	//load db
	loadDb();

	//build name
	std::string shortName;
	std::string packageName = name;
	if (Helper::startBy(name,"gentoo/"))
	{
		shortName = packageName.substr(7);
	} else {
		shortName = packageName;
		packageName = "gentoo/"+shortName;
	}

	//load quickpage
	PackageDef quickPack;
	prefix->getQuickPackage(quickPack,packageName);
	
	//default regexp
	if (quickPack.vfetcher.regexp.empty())
		quickPack.vfetcher.regexp = "[0-9]+.[0-9]+.?[0-9]*.?[0-9]*";
	
	//build regexp
	std::string reg = Helper::escape(shortName,'+')+"-("+Helper::escape(quickPack.vfetcher.regexp+").(tar.gz|tar.bz2|tar.bzip|tar.xz|tar.lz|tgz)",'.');
	
	//build regexp
	RE2 regexp(reg);
	
	//scan
	std::string fname;
	std::string version;
	std::string url;
	StringList versions;
	for (auto & entry : db)
	{
		fname = Helper::last(entry,'/');
		if (RE2::FullMatch(fname,regexp,&version))
		{
			versions.push_back(version);
			url = entry;
			url.replace(url.find(version), version.length(), "${VERSION}");
		}
	}
	
	//sort version
	VersionMatcher::sortList(versions);
	
	//return
	if (versions.empty())
	{
		return false;
	} else {
		//basics
		out.merge(quickPack);
		out.versions.clear();
		out.versions.push_back(versions.front());
		out.name = packageName;

		//urls
		for (auto ext :gblExt)
			out.urls.push_back(getGentooUrl(shortName+"-${VERSION}"+std::string(ext)));

		//ok
		return true;
	}
}

/*******************  FUNCTION  *********************/
std::string ProviderGentoo::getGentooUrl(const std::string & filename)
{
	return "ftp://"+server+":"+port+"/"+distfiles+"/"+filename;
}

/*******************  FUNCTION  *********************/
void ProviderGentoo::updateCache(void)
{
	//nothing to do
}

/*******************  FUNCTION  *********************/
void ProviderGentoo::updateDb(void)
{
	//Vars
	StringList lst;

	//scan
	std::string url = "/";
	//this is a trick to make unit test faster
	if (unitTest)
		url = "../snapshots/";
	
	//get
	System::ftpListFiles(getGentooUrl(""),[&lst](const std::string & value){
		lst.push_back(value);
	});

	//save
	std::string content = Helper::join(lst,'\n');
	std::string path = prefix->getFilePath("/homelinux/packages/gentoo.lst");

	//don't override unit test file
	if (unitTest == false)
		System::writeFile(content,path);
}

/*******************  FUNCTION  *********************/
std::string ProviderGentoo::search(const std::string & name)
{
	//check if loaded
	this->loadDb();

	//low name
	std::string lname = Helper::toLower(name);

	//loop
	StringList out;
	for (auto & entry : db)
		if (Helper::contain(entry,lname))
			out.push_back(Colors::magenta("gentoo/"+entry));
	
	//ret
	return Helper::join(out,'\n');
}

}
