/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <fstream>
#include <base/Debug.hpp>
#include <base/Helper.hpp>
#include <portability/System.hpp>
#include "PackageDef.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
void PackageDef::load(const std::string & path)
{
	//load
	Json::Value tmpObj;
	System::loadJson(tmpObj,path);

	//apply
	this->loadJson(tmpObj);
}

/*******************  FUNCTION  *********************/
void PackageDef::loadJson(const Json::Value & json)
{
	this->name = json.get("name","").asString();
	this->homepage = json.get("homepage","").asString();
	this->inherit = json.get("inherit","").asString();
	Helper::jsonToObj(versions,json["versions"]);
	this->vfetcher.mode = json["vfetcher"].get("mode","").asString();
	this->vfetcher.url = json["vfetcher"].get("url","").asString();
	this->vfetcher.regexp = json["vfetcher"].get("regexp","").asString();
	Helper::jsonToObj(md5,json["md5"]);
	this->subdir = json.get("subdir","").asString();
	Helper::jsonToObj(deps,json["deps"]);
	this->host = json["host"];
	Helper::jsonToObj(configure,json["configure"]);
	Helper::jsonToObj(vspecific,json["vspecific"]);
	Helper::jsonToObj(steps,json["steps"]);
	Helper::jsonToObj(conflicts,json["conflicts"]);
	Helper::jsonToObj(use,json["use"]);
	Helper::jsonToObj(warn,json["warn"]);
	this->module = json.get("module","").asString();
	Helper::jsonToObj(scripts,json["scripts"]);
	Helper::jsonToObj(vars,json["vars"]);
	Helper::jsonToObj(flags,json["flags"]);
}

/*******************  FUNCTION  *********************/
void PackageDef::save(Json::Value & json)
{
	json["name"] = name;
	json["homepage"] = homepage;
	json["inherit"] = inherit;
	Helper::toJson(json["versions"],versions);
	json["vfetcher"]["mode"] = vfetcher.mode;
	json["vfetcher"]["url"] = vfetcher.url;
	json["vfetcher"]["regexp"] = vfetcher.regexp;
	Helper::toJson(json["md5"],md5);
	json["subdir"] = subdir;
	Helper::toJson(json["deps"],deps);
	json["host"] = host;
	Helper::toJson(json["configure"],configure);
	Helper::toJson(json["vspecific"],vspecific);
	Helper::toJson(json["steps"],steps);
	Helper::toJson(json["conflicts"],conflicts);
	Helper::toJson(json["use"],use);
	Helper::toJson(json["warn"],warn);
	json["module"] = module;
	Helper::toJson(json["scripts"],scripts);
	Helper::toJson(json["vars"],vars);
	Helper::toJson(json["flags"],flags);
}

/*******************  FUNCTION  *********************/
void PackageDef::merge(const PackageDef & def)
{
	if (def.name.empty() == false)
		name = def.name;
	if (def.homepage.empty() == false)
		homepage = def.homepage;
	if (def.inherit.empty() == false)
		inherit = def.inherit;
	if (def.versions.empty() == false)
		versions = def.versions;
	if (def.vfetcher.mode.empty() == false)
		vfetcher.mode = def.vfetcher.mode;
	if (def.vfetcher.url.empty() == false)
		vfetcher.url = def.vfetcher.url;
	if (def.vfetcher.regexp.empty() == false)
		vfetcher.regexp = def.vfetcher.regexp;
	Helper::merge(md5,def.md5);
	if (def.subdir.empty() == false)
		subdir = def.subdir;
	Helper::merge(deps,def.deps);
	//@TODO make better merge
	host = def.host;
	Helper::merge(configure,def.configure,false);
	for (auto & it : def.vspecific)
		vspecific[it.first] = it.second;
	Helper::merge(steps,def.steps,true);
	Helper::merge(conflicts,def.conflicts);
	Helper::merge(use,def.use);
	Helper::merge(warn,def.warn);
	if (def.module.empty() == false)
		module = def.module;
	Helper::merge(scripts,def.scripts);
	Helper::merge(vars,def.vars);
	Helper::merge(flags,def.flags,false);
}

/*******************  FUNCTION  *********************/
void PackageDef::save(const std::string & path)
{
	//open file
	std::ofstream out(path);
	assumeArg(out.fail() == false,"Fail to open file %1 to dump config : %2").arg(path).argStrErrno().end();

	//write	
	save(out);

	//close
	out.close();
}

/*******************  FUNCTION  *********************/
void PackageDef::save(std::ostream & out)
{
	//rebuild json
	Json::Value json;
	
	//convert
	save(json);
	
	//dump
	out << json;
}

}
