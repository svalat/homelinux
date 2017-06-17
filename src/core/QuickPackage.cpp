/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cassert>
#include <portability/System.hpp>
#include <base/Helper.hpp>
#include <base/Debug.hpp>
#include "Prefix.hpp"
#include "QuickPackage.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  CONST  ***********************/
static const StringList gblEmpty;
static const char * gblProperties[] = {
	"deps",
	"patch",
	"config",
	"version",
	"module",
	"type",
	"subdir"
};
static const char* gblHosts[] = {
	"centos7",
	"debian8",
	"user",
	"gentoo"
};

/*******************  FUNCTION  *********************/
QuickPackage::QuickPackage(const Prefix * prefix)
{
	//check
	assert(prefix != NULL);
	
	//setup
	this->prefix = prefix;
	
	//load files from prefix & home
	for (auto prop : gblProperties)
	{
		loadQuickFile(prop,this->prefix->getFilePath("/homelinux/packages/quickpackages/"));
		loadQuickFile(prop,System::getHomeDir()+".homelinux/quickpackages/");
	}
	
	//load hosts
	for (auto host : gblHosts)
	{
		loadQuickFile(host,this->prefix->getFilePath("/homelinux/packages/hosts/"));
		loadQuickFile(host,System::getHomeDir()+".homelinux/hosts/");
	}
}

/*******************  FUNCTION  *********************/
void QuickPackage::loadQuickFile(const std::string & property,const std::string & dir)
{
	//setup
	db[property];
	
	//get filename
	std::string path = dir + "/"+property+".txt";
	
	//if file does not exit, skip
	if (System::fileExist(path) == false)
		return;
	
	//load
	std::string content = System::loadFile(path);
	
	//for each line
	StringMapList & prop = db[property];
	Helper::stringSplit(content,'\n',[&prop](const std::string & line){
		if (line[0] != '#' && line.empty() == false)
		{
			StringList lst;
			Helper::stringSplit(line,' ',[&lst](const std::string & value){
				lst.push_back(value);
			});
			std::string name = Helper::getListEl(lst,0);
			lst.erase(lst.begin());
			Helper::merge(prop[name],lst);
		}
	});
}

/*******************  FUNCTION  *********************/
const StringList & QuickPackage::getQuickInfo(const std::string & property,const std::string packageName) const
{
	//get entry
	auto it = db.find(property);
	
	//error
	assumeArg(it != db.end(),"Invalid property name : %1").arg(property).end();
	
	//check if has one
	const StringMapList & map = it->second;
	auto entryIt = map.find(packageName);
	if (entryIt == map.end())
		return gblEmpty;
	else
		return entryIt->second;
}

/*******************  FUNCTION  *********************/
std::string QuickPackage::getQuickInfoFirst(const std::string & property,const std::string packageName,const std::string & defaultValue) const
{
	const StringList & lst = getQuickInfo(property,packageName);
	if (lst.empty())
		return defaultValue;
	else
		return *lst.begin();
}

/*******************  FUNCTION  *********************/
void QuickPackage::genPackage(PackageDef & pack,const std::string & name) const
{
	std::string shortName;
	Helper::stringSplit(name,'/',[&shortName](const std::string & value){
		shortName = value;
	});
	
	pack.name = name;
	
	pack.inherit = this->getQuickInfoFirst("type",name,"models/auto");
	pack.subdir = this->getQuickInfoFirst("subdir",name,shortName+"-${VERSION}");
	pack.module = this->getQuickInfoFirst("module",name,"");
	
	pack.versions = this->getQuickInfo("version",name);
	pack.deps = this->getQuickInfo("deps",name);
	pack.patch = this->getQuickInfo("patch",name);
	pack.configure[""] = this->getQuickInfo("config",name);
	
	for (auto host : gblHosts)
	{
		const StringList & lst = this->getQuickInfo(host,name);
		if (lst.empty() == false)
		{
			if (host == "user")
					pack.host[host] = true;
			else
					Helper::toJson(pack.host[host],lst);
		}
	}
}

}
