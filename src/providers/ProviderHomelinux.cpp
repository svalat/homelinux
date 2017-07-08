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
#include <algorithm>
#include <base/Helper.hpp>
#include <base/Debug.hpp>
#include <core/Prefix.hpp>
#include <portability/System.hpp>
#include <portability/Mutex.hpp>
#include "ProviderHomelinux.hpp"
#include <portability/Regexp.hpp>
#include <base/Colors.hpp>
#include <crawlers/CrawlerDummy.hpp>
#include <crawlers/CrawlerFtp.hpp>
#include <crawlers/CrawlerHtml.hpp>
#include <crawlers/CrawlerGentoo.hpp>
#include <crawlers/CrawlerGithub.hpp>
#include <crawlers/CrawlerGnomeCache.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
ProviderHomelinux::ProviderHomelinux(Prefix * prefix)
               :Provider("homelinux",prefix)
{
	this->warn = false;
}

/*******************  FUNCTION  *********************/
ProviderHomelinux::~ProviderHomelinux(void)
{
	//free mem
	//for (auto it : crawlers)
	forEach(CrawlerMap,it,crawlers)
		//for (auto it2 : it.second)
		forEach(CrawlerStringMap,it2,it->second)
			delete it2->second;
	crawlers.clear();
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::loadCache(void)
{
	//already done
	if (cache.empty() == false)
		return;
	
	//load
	std::string path = prefix->getFilePath("/homelinux/packages/db/cache.json");
	if (System::fileExist(path))
	{
		//load
		Json::Value json;
		System::loadJson(json,path);
		Helper::jsonToObj(cache,json);
	} else {
		if (!warn)
	    	HL_ERROR("No cache file available, consider to call 'hl update-cache' at least once");
		warn = true;
	}
}

/*******************  FUNCTION  *********************/
bool ProviderHomelinux::getPackage(PackageDef & out,const std::string & name)
{
	//build name
	std::string shortName;
	std::string packageName = name;
	if (Helper::startBy(name,"hl/"))
	{
		shortName = packageName.substr(3);
	} else {
		shortName = packageName;
		packageName = "hl/"+shortName;
	}
	
	//search in cache
	if (Helper::contain(shortName,"/") == false)
	{
		//search
		shortName = this->searchInCache("/"+shortName);
		
		//not fount
		if (shortName.empty())
			return false;
		
		//build full name
		packageName = "hl/"+shortName;
	}
	
	//check in cache
	if (packageCache.find(packageName) != packageCache.end())
	{
		out.merge(packageCache[packageName]);
		return true;
	}
	
	//load from cache
	PackageDef & pack = packageCache[packageName];
	std::string path = prefix->getFilePath("/homelinux/packages/db/"+shortName+".json");
	HL_DEBUG_ARG("ProviderHomelinux","Try to load file : %1").arg(path).end();
	if (System::fileExist(path))
	{
		pack.load(path);
		
		//apply version
		this->loadVersions();
		if (versions.find(packageName) != versions.end())
			pack.versions = versions[packageName];

		//apply name
		pack.name = packageName;
		
		//merge
		out.merge(pack);
		
		return true;
	} else {
		return false;
	}
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::updateCache(void)
{
	//setup
	Regexp regexp("([A-Za-z0-9_-]+/[A-Za-z0-9+_-]+)\\.json");
	std::string path = prefix->getFilePath("/homelinux/packages/db");
	
	//check
	assumeArg(System::fileExist(path),"Fail to find homelinux db : %1 !").arg(path).end();
	
	//scan
	StringList lst = System::findFiles(path);
	forEach(StringList,file,lst)
	{
		std::string packageName;
		if (regexp.match(*file,packageName))
			cache.push_back(packageName);
	}
	
	//convert to json
	Json::Value json;
	Helper::toJson(json,cache);
	
	//write
	std::string jsonPath = prefix->getFilePath("/homelinux/packages/db/cache.json");
	System::writeJson(json,jsonPath);
}

/*******************  FUNCTION  *********************/
Crawler * ProviderHomelinux::getCrawler(int threadId,const std::string & name,const std::string & packageName)
{
	//check cache
	CrawlerStringMap::iterator it = crawlers[threadId].find(name);
	if (it != crawlers[threadId].end())
		return it->second;
	
	//load
	Crawler * ret = NULL;
	if (name == "dummy")
		ret = crawlers[threadId]["dummy"] = new CrawlerDummy(prefix);
	else if (name == "ftp")
		ret = crawlers[threadId]["ftp"] = new CrawlerFtp(prefix);
	else if (name == "html")
		ret = crawlers[threadId]["html"] = new CrawlerHtml(prefix);
	else if (name == "github")
		ret = crawlers[threadId]["github"] = new CrawlerGithub(prefix);
	else if (name == "gnome-cache")
		ret = crawlers[threadId]["gnome-cache"] = new CrawlerGnomeCache(prefix);
	else if (name == "gentoo")
		ret = crawlers[threadId]["gentoo"] = new CrawlerGentoo(prefix);
	else
		HL_ERROR_ARG("Invalid crawler in %1 : %2").arg(packageName).arg(name).end();
		
	//ok ret
	return ret;
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::crawl(int cur, int cnt,int threadId,StringMapList & out,const std::string & path)
{
	//load package
	Json::Value pack;
	System::loadJson(pack,path);
	if (pack.isObject() == false)
	{
		HL_ERROR_ARG("Fail to load package : %1").arg(path).end();
		return;
	}
	std::string name = pack.get("name","UNKNOWN").asString();

	//check
	if (Helper::endBy(path,name+".json") == false)
		HL_WARNING_ARG("Caution, filename do not match which package name : %1 != %2").arg(name).arg(path).end();
	
	//get default
	StringList versions;
	Helper::jsonToObj(versions,pack["versions"]);
	
	//get infos
	Json::Value vfetcher = pack["vfetcher"];
	
	//get mode
	std::string mode = vfetcher.get("mode","UNKNOWN").asString();
	
	//temporary convert
	if (mode == "http" || mode == "http-apache-list")
		mode = "html";
	if (mode == "http-gnome-cache")
		mode = "gnome-cache";
	if (mode == "none")
		mode = "dummy";
	vfetcher["mode"] = mode;
	
	//craw
	Crawler * crawler = getCrawler(threadId,mode,path);
	if (crawler == NULL)
	{
		out["hl/"+name] = versions;
		return;
	}

	//debug
	HL_MESSAGE_ARG("[%1/%2] Crawling %3 using %4 ...").arg(cur).arg(cnt).arg(Colors::green("hl/"+name)).arg(crawler->getName()).end();

	//run
	try{
		out["hl/"+name] = crawler->run(name,vfetcher,versions);
	} catch(std::runtime_error & err) {
		HL_ERROR_ARG("Get error on package hl/%1 : %2").arg(name).arg(err.what()).end();
	}
}

/******************  STRUCT  ************************/
struct WorkerArgs
{
	int * cur;
	int fileCnt;
	int threadId;
	ProviderHomelinux * provider;
	Mutex * mutex;
	StringMapList * global;
	StringList * files;
};

/*******************  FUNCTION  *********************/
void * worker(void * args)
{
	//convert
	WorkerArgs & wargs = *(WorkerArgs*)args;

	StringMapList local;
	bool loop = true;
	while (loop)
	{
		wargs.mutex->lock();
		if (wargs.files->empty())
		{
			Helper::merge(*wargs.global,local,false);
			loop = false;
			wargs.mutex->unlock();
		} else {
			std::string p = wargs.files->front();
			wargs.files->pop_front();
			(*wargs.cur)++;
			wargs.mutex->unlock();
			wargs.provider->crawl(*(wargs.cur),wargs.fileCnt,wargs.threadId,local,p);
		}
	}
	
	return NULL;
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::updateDb(void)
{
	//vars
	StringList files;
	StringMapList global;
	Mutex mutex;

	//find all packages
	std::string path = prefix->getFilePath("/homelinux/packages/db/");
	StringList tmp = System::findFiles(path,path);
	forEach(StringList,it,tmp)
		if (Helper::endBy(*it,".json") && *it != "cache.json" && *it != "versions.json")
			files.push_back(*it);

	//compute number of threads we want
	int cntThreads;
	if (prefix->getUserConfig().crawlerThreads == 0)
		cntThreads = System::getCoreCount();
	else
		cntThreads = prefix->getUserConfig().crawlerThreads;
	int fileCnt = files.size();
	int cur = 0;

	//create thread entries to not create in threads
	for (int i = 0 ; i < cntThreads ; i++)
		crawlers[i].empty();

	//load html provider to setup npm environnement befaore using threads
	getCrawler(0,"html","");

	//launcher worker
	pthread_t * threads = new pthread_t[cntThreads];
	for (int i = 0 ; i < cntThreads ; i++)
	{
		//setup args
		WorkerArgs * workerArgs = new WorkerArgs;
		workerArgs->cur = &cur;
		workerArgs->fileCnt = fileCnt;
		workerArgs->threadId = i;
		workerArgs->provider = this;
		workerArgs->mutex = & mutex;
		workerArgs->global = &global;
		workerArgs->files = &files;

		//launch
		pthread_create(&threads[i],NULL,worker,workerArgs);
	}

	//wait threads
	for (int i = 0 ; i < cntThreads ; i++)
		pthread_join(threads[i],NULL);
	delete [] threads;

	//convert
	Json::Value json;
	Helper::toJson(json,global);
	
	//save
	std::string outPath = prefix->getFilePath("/homelinux/packages/db/versions.json");
	System::writeJson(json,outPath);
}

/*******************  FUNCTION  *********************/
std::string ProviderHomelinux::search(const std::string & name)
{
	//vars
	StringList lst;
	
	//ensure load
	this->loadCache();
	
	//conver
	std::string lowerName = Helper::toLower(name);
	//for (auto & entry : cache)
	forEach(StringList,entry,cache)
	{
		//get name
		std::string pn;
		StringList tmp = Helper::split(*entry,'/');
		pn = tmp.back();
		
		//to lower
		pn = Helper::toLower(pn);
		if (Helper::contain(pn,name))
		{
			//@TODO replace by prefix.loadPackage
			PackageDef def;
			*entry = "hl/"+*entry;
			bool status = getPackage(def,*entry);
			assumeArg(status,"Fail to load package %1").arg(*entry).end();
			
			//sort versions
			def.sortVersions();
			
			//get vars
			std::string slot = def.getSlot();
			std::string version = def.getVersion();
			std::string versions = def.getNVersions(10);
			
			//build full
			std::string full = Colors::green(*entry+":"+slot)+"-"+Colors::cyan(version)+" ["+Colors::blue(versions)+"]";
			lst.push_back(full);
		}
	}
	
	return Helper::join(lst,'\n');
}

/*******************  FUNCTION  *********************/
std::string ProviderHomelinux::searchInCache(const std::string & name)
{
	//vars
	StringList lst;
	
	//get cache
	this->loadCache();
	
	//loop
	//for (auto entry : cache)
	forEach(StringList,entry,cache)
		if (Helper::endBy(*entry,name))
			lst.push_back(*entry);
			
	//check
	if (lst.empty())
	{
		return "";
	} else if (lst.size() == 1) {
		return lst.front();
	} else {
		HL_FATAL_ARG("Fail to find your package, multiple match : %1").arg(Helper::join(lst,',')).end();
		return "";
	}
}

/*******************  FUNCTION  *********************/
void ProviderHomelinux::loadVersions(void)
{
	//already done
	if (versions.empty() == false)
		return;
	
	//path
	std::string path = prefix->getFilePath("/homelinux/packages/db/versions.json");
	
	//check if exist
	Json::Value json;
	System::loadJson(json,path);
	
	//convert
	Helper::jsonToObj(versions,json);
}

}
