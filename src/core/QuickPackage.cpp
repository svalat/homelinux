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
	"subdir",
	NULL
};
static const char* gblHosts[] = {
	"centos7",
	"debian8",
	"default",
	"gentoo",
	NULL
};

/*******************  FUNCTION  *********************/
QuickPackage::QuickPackage(const Prefix * prefix)
{
	//check
	assert(prefix != NULL);
	
	//setup
	this->prefix = prefix;
	
	//load files from prefix & home
	int i = 0;
	while(gblProperties[i] != NULL)
	{
		loadQuickFile(gblProperties[i],this->prefix->getFilePath("/homelinux/packages/quickpackages/"));
		loadQuickFile(gblProperties[i],System::getHomeDir()+".homelinux/quickpackages/");
		i++;
	}
	
	//load hosts
	i = 0;
	while (gblHosts[i] != NULL)
	{
		loadQuickFile(gblHosts[i],this->prefix->getFilePath("/homelinux/packages/hosts/"));
		loadQuickFile(gblHosts[i],System::getHomeDir()+".homelinux/hosts/");
		i++;
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
	StringList lst = Helper::split(content,'\n');
	forEach(StringList,line,lst)
	{
		if ((*line)[0] != '#' && line->empty() == false)
		{
			StringList lst2 = Helper::split(*line,' ');
			std::string name = Helper::getListEl(lst2,0);
			lst2.erase(lst2.begin());
			Helper::merge(prop[name],lst2);
		}
	}
}

/*******************  FUNCTION  *********************/
const StringList & QuickPackage::getQuickInfo(const std::string & property,const std::string packageName) const
{
	//get entry
	std::map<std::string,StringMapList>::const_iterator it = db.find(property);
	
	//error
	assumeArg(it != db.end(),"Invalid property name : %1").arg(property).end();
	
	//check if has one
	const StringMapList & map = it->second;
	StringMapList::const_iterator entryIt = map.find(packageName);
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
	std::string shortName = Helper::last(name,'/');
	
	pack.name = name;
	pack.api = HL_JHSON_API;
	pack.inherit = this->getQuickInfoFirst("type",name,"models/auto");
	pack.subdir = this->getQuickInfoFirst("subdir",name,shortName+"-${VERSION}");
	pack.module = this->getQuickInfoFirst("module",name,"");
	
	const StringList & versionRefexp = this->getQuickInfo("version",name);
	pack.vfetcher["regexp"] = versionRefexp.empty()?"":versionRefexp.front();
	pack.vfetcher["url"] = "";
	pack.vfetcher["mode"] = "";
	pack.deps = this->getQuickInfo("deps",name);
	pack.patch = this->getQuickInfo("patch",name);
	pack.configure[""] = this->getQuickInfo("config",name);
	
	int i = 0;
	while (gblHosts[i] != NULL)
	{
		const StringList & lst = this->getQuickInfo(gblHosts[i],name);
		if (lst.empty() == false)
		{
			if (std::string(gblHosts[i]) == "default")
					pack.host[gblHosts[i]] = false;
			else
					Helper::toJson(pack.host[gblHosts[i]],lst);
		}
		i++;
	}
}

}
