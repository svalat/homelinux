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
#include "ProviderDebian.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CONST ***********************/
/*static const char * gblCutDeps[] = {
	"autoconf",
	"automake",
	"m4",
	"python-defaults",
	"python3-defaults",
	"gcc-defaults",
	"autotrash",
	"lsb",
	"binutils"
};*/

/*******************  FUNCTION  *********************/
ProviderDebian::ProviderDebian(Prefix * prefix, bool unitTest)
               :Provider("debian",prefix)
{
	this->warn = true;
	this->unitTest = unitTest;
}

/*******************  FUNCTION  *********************/
bool ProviderDebian::getPackage(PackageDef & out,const std::string & name)
{
	//load db
	loadDb();

	//build name
	std::string shortName;
	std::string packageName = name;
	if (Helper::startBy(name,"debian/"))
	{
		shortName = packageName.substr(7);
	} else {
		shortName = Helper::last(packageName,'/');
		packageName = "debian/"+shortName;
	}

	//check
	DebianDbEntry * entry = NULL;
	if (db.find(shortName) != db.end())
		entry = &db[shortName];

	//search in sub packages
	if (entry == NULL)
	{
		//for (auto & it : db)
		forEach(DebianDb,it,db)
		{
			if (Helper::contain(it->second.packages,shortName))
				entry = &it->second;
		}
	}

	//not found
	if (entry == NULL)
		return false;

	//setup
	PackageDef def;
	prefix->getQuickPackage(def,shortName);

	//setup
	def.versions.push_back(entry->version);
	def.urls.push_back("http://http.debian.net/debian/"+entry->dir+"/"+entry->name+"_"+entry->version+".orig.tar.gz");
	def.subdir = '*';
	def.name = packageName;
	def.host["debian8"].append(shortName);

	//merge
	out.merge(def);
	
	//get quickpackage
	return true;
}

/*******************  FUNCTION  *********************/
void ProviderDebian::updateCache(void)
{

}

/*******************  FUNCTION  *********************/
void ProviderDebian::updateDb(void)
{
	//vars
	const Json::Value & json = prefix->getConfig().debian;

	//nothing
	if (json["repos"].isArray() == false)
		return;

	//loop
	for (Json::ArrayIndex i = 0 ; i < json["repos"].size() ; i++)
	{
		//get url
		std::string repo = json["repos"][i].asString();

		//load
		std::string content;
		StringMap state;
		std::string cur;
		if (System::runAndRead(content,"curl "+repo+"/source/Sources.xz | xz -d"))
		{
			StringList lst = Helper::split(content,'\n',true);
			forEach(StringList,line,lst)
			{
				if (line->empty())
				{
					DebianDbEntry entry;
					entry.name = state["Package"];
					entry.version = Helper::split(state["Version"],'-').front();
					entry.deps = state["Build-Depends"];
					entry.homepage = state["Homepage"];
					//todo split on ', '
					entry.packages = Helper::split(state["Binary"],',');
					entry.dir = state["Directory"];
					entry.section = state["Section"];

					db[entry.name] = entry;
				} else if ((*line)[0] == ' ') {
					state[cur] += " ";
					state[cur] += Helper::split(*line,' ').back();
				} else {
					StringList lst = Helper::split(*line, ':');
					cur = lst.front();
					if (lst.size() == 1 || lst.back() == "" || lst.back() == " ")
						state[cur] = "";
					else
						state[cur] = lst.back().substr(1);
				}
			}
		}
	}

	//save
	this->saveDb();
}

/*******************  FUNCTION  *********************/
std::string ProviderDebian::search(const std::string & name)
{
	//check if loaded
	this->loadDb();

	//low name
	std::string lname = Helper::toLower(name);

	//loop
	StringList out;
	//for (auto & entry : db)
	forEach(DebianDb,entry,db)
		if (Helper::contain(entry->first,lname))
			out.push_back(Colors::magenta("debian/"+entry->first));

	//ret
	return Helper::join(out,'\n');
}

/*******************  FUNCTION  *********************/
void ProviderDebian::loadDb(void)
{
	//already done
	if (db.empty() == false)
		return;

	//load file
	std::string path = prefix->getFilePath("/homelinux/packages/debian.json");

	//check if exist
	if (System::fileExist(path))
	{
		//load
		Json::Value json;
		System::loadJson(json,path);
				
		//convert
		if (json.isArray())
		{
			for (Json::ArrayIndex i = 0 ; i < json.size() ; i++)
			{
				DebianDbEntry entry;
				Json::Value & p = json[i];
				entry.name = p.get("name","").asString();
				entry.version = p.get("version","").asString();
				entry.deps = p["deps"].asString();
				entry.homepage = p.get("homepage","").asString();
				Helper::jsonToObj(entry.packages,p["packages"]);
				entry.dir = p.get("dir","").asString();
				entry.section = p.get("section","").asString();
				db[entry.name] = entry;
			}
		}
	} else {
		if (warn)
			HL_ERROR("No gen debian list file available, consider to call 'hl update-db' at least once");
		warn = false;
	}
}

/*******************  FUNCTION  *********************/
void ProviderDebian::saveDb(void)
{    
	//load file
	std::string path = prefix->getFilePath("/homelinux/packages/debian.json");

	//convert
	Json::Value json;
	forEach(DebianDb,it,db)
	{
		DebianDbEntry & entry = it->second;
		Json::Value e;
		e["name"] = entry.name;
		e["version"] = entry.version;
		e["deps"] = entry.deps;
		e["homepage"] = entry.homepage;
		Helper::toJson(e["pakages"],entry.packages);
		e["dir"] = entry.dir;
		e["section"] = entry.section;
		json.append(e);
	}

	//save
	System::writeJson(json,path);
}

}
