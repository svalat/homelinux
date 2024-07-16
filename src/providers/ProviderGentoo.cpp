/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
//internal
#include <portability/System.hpp>
#include <portability/Regexp.hpp>
#include <core/Prefix.hpp>
#include <base/Helper.hpp>
#include <base/Colors.hpp>
#include "ProviderGentoo.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CONST ***********************/
static const char * gblExt[] = {"tar.bz2","tar.xz","tar.gz","zip","tgz",NULL};

/*******************  FUNCTION  *********************/
ProviderGentoo::ProviderGentoo(Prefix * prefix, bool unitTest)
			   :Provider("gentoo",prefix)
{
	this->warn = true;
	this->unitTest = unitTest;

	//load config
	const Json::Value & json = prefix->getConfig().gentoo;
	server = json.get("server","ftp.free.fr").asString();
	port = json.get("port",21).asString();
	distfiles = json.get("distfiles","mirrors/ftp.gentoo.org").asString();
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
		db = Helper::split(file,'\n');
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
		shortName = Helper::last(packageName,'/');
		packageName = "gentoo/"+shortName;
	}

	//load quickpage
	PackageDef quickPack;
	prefix->getQuickPackage(quickPack,packageName);
	
	//default regexp
	if (quickPack.vfetcher["regexp"].isString() == false || quickPack.vfetcher["regexp"].asString().empty())
		quickPack.vfetcher["regexp"] = "[0-9]+.[0-9]+.?[0-9]*.?[0-9]*";
	
	//build regexp
	std::string reg = Helper::escape(shortName,'+')+"-("+Helper::escape(quickPack.vfetcher["regexp"].asString()+").(tar.gz|tar.bz2|tar.bzip|tar.xz|tar.lz|tgz)",'.');
	
	//build regexp
	Regexp regexp(reg);
	
	//scan
	std::string fname;
	std::string version;
	std::string url;
	StringList versions;
	//for (auto & entry : db)
	forEach(StringList,entry,db)
	{
		fname = Helper::last(*entry,'/');
		if (regexp.match(fname,version))
		{
			versions.push_back(version);
			url = *entry;
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
		//for (auto ext :gblExt)
		for (int i = 0 ; gblExt[i] != NULL ; i++)
			out.urls.push_back(getGentooUrl(shortName+"-${VERSION}."+std::string(gblExt[i])));

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
	System::ftpListFiles(lst,getGentooUrl(""));

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
	//for (auto & entry : db)
	forEach(StringList,entry,db)
		if (Helper::contain(*entry,lname))
			out.push_back(Colors::magenta("gentoo/"+*entry));
	
	//ret
	return Helper::join(out,'\n');
}

}
