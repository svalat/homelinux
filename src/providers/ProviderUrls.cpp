/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <base/Colors.hpp>
#include <base/Helper.hpp>
#include <portability/System.hpp>
#include <portability/Regexp.hpp>
#include <core/Prefix.hpp>
#include "ProviderUrls.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/******************* FUNCTION *********************/
ProviderUrls::ProviderUrls(Prefix * prefix)
             :Provider("urls",prefix)
{
	
}

/******************* FUNCTION *********************/
ProviderUrls::~ProviderUrls(void)
{
}

/******************* FUNCTION *********************/
void ProviderUrls::loadDb(void)
{
	//get path & load
	std::string path = prefix->getFilePath("/homelinux/packages/urls.lst");
	loadDb(path);
	
	//check if home exist
	path = System::getHomeDir() + "/.homelinux/urls.lst";
	if (System::fileExist(path))
		loadDb(path);
}

/******************* FUNCTION *********************/
void ProviderUrls::loadDb(const std::string & path)
{
	//load
	std::string file = System::loadFile(path);
	
	//for each line
	Helper::split(file,'\n',[this](const std::string & line){
		this->db.push_back(line);
	});
}

/******************* FUNCTION *********************/
bool ProviderUrls::getPackage(PackageDef & out,const std::string & name)
{
	//load db
	this->loadDb();
	
	//build name
	std::string shortName;
	std::string packageName = name;
	if (Helper::startBy(packageName,"urls/"))
	{
		shortName = packageName.substr(5);
	} else {
		shortName = Helper::last(packageName,'/');
		packageName = "urls/" + packageName;
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
	for (auto & entry : db)
	{
		fname = Helper::last(entry,'/');
		if (regexp.match(fname,version))
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
		out.merge(quickPack);
		out.versions.clear();
		out.versions.push_back(versions.front());
		out.urls.push_back(url);
		out.name = packageName;
		return true;
	}
}

/******************* FUNCTION *********************/
void ProviderUrls::updateCache(void)
{
}

/******************* FUNCTION *********************/
void ProviderUrls::updateDb(void)
{
}

/******************* FUNCTION *********************/
std::string ProviderUrls::search(const std::string & name)
{
	//vars
	StringList lst;

	//load db
	loadDb();

	//loop on all
	for (auto & entry : db)
		if (Helper::contain(entry,name))
			lst.push_back(Colors::magenta("urls/"+Helper::last(entry,'/')));
	
	//gen
	return Helper::join(lst,'\n');
}

}
