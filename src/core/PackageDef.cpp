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
	//compute file path
	assume(path.empty() == false,"Invalid empty path");
	
	//check if exist
	assumeArg(System::fileExist(path),"File %1 does not exist !").arg(path).end();

	//open file
	std::ifstream file(path.c_str(),std::ifstream::binary);
	assumeArg(file.is_open(),"Fail to open configuration file : %1 : %2").arg(path).argStrErrno().end();
	
	//load
	Json::Value tmpObj;
	
	//parse
	Json::Reader reader;
	bool status = reader.parse(file, tmpObj);
	assumeArg(status,"Fail to load configuration file '%1' : \n%2")
		.arg(path)
		.arg(reader.getFormattedErrorMessages())
		.end();

	//apply
	this->load(tmpObj);
}

/*******************  FUNCTION  *********************/
void PackageDef::load(Json::Value & json)
{
	this->name = json.get("name","").asString();
	this->homepage = json.get("home","").asString();
	this->inherit = json.get("inherit","").asString();
	this->jsonToObj(versions,json["versions"]);
	this->vfetcher.mode = json.get("mode","").asString();
	this->vfetcher.url = json.get("url","").asString();
	this->vfetcher.regexp = json.get("regexp","").asString();
	this->jsonToObj(md5,json["mod5"]);
	this->subdir = json.get("subdir","").asString();
	this->jsonToObj(deps,json["deps"]);
	this->host = json["host"];
	this->jsonToObj(configure,json["configure"]);
	this->jsonToObj(vspecific,json["vspecific"]);
	this->jsonToObj(steps,json["steps"]);
	this->jsonToObj(conflicts,json["conflicts"]);
	this->jsonToObj(use,json["use"]);
	this->jsonToObj(warn,json["warn"]);
	this->module = json.get("module","").asString();
	this->jsonToObj(scripts,json["scripts"]);
	this->jsonToObj(vars,json["vars"]);
	this->jsonToObj(flags,json["flags"]);
}

/*******************  FUNCTION  *********************/
void PackageDef::save(Json::Value & json)
{
	json["name"] = name;
	json["homepage"] = homepage;
	json["inherit"] = inherit;
	toJson(json["versions"],versions);
	json["vfetcher"]["mode"] = vfetcher.mode;
	json["vfetcher"]["url"] = vfetcher.url;
	json["vfetcher"]["regexp"] = vfetcher.regexp;
	toJson(json["md5"],md5);
	json["subdir"] = subdir;
	toJson(json["deps"],deps);
	json["host"] = host;
	toJson(json["configure"],configure);
	toJson(json["vspecific"],vspecific);
	toJson(json["steps"],steps);
	toJson(json["conflicts"],steps);
	toJson(json["use"],use);
	toJson(json["warn"],warn);
	json["module"] = module;
	toJson(json["scripts"],scripts);
	toJson(json["vars"],vars);
	toJson(json["flags"],flags);
}

/*******************  FUNCTION  *********************/
void PackageDef::apply(const PackageDef & def)
{
	
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

/*******************  FUNCTION  *********************/
void PackageDef::jsonToObj(StringList & out,const Json::Value & json)
{
	out.clear();
	if (json.isArray())
	{
		int size = json.size();
		for (int i = 0 ; i < size ; i++)
			out.push_back(json[i].asString());
	}
}

/********************  STRUCT  **********************/
void PackageDef::jsonToObj(StringMap & out,const Json::Value & json)
{
	out.clear();
	for (auto it = json.begin() ; it != json.end() ; ++it)
		out[it.key().asString()] = (*it).asString();
}

/********************  STRUCT  **********************/
void PackageDef::jsonToObj(StringMapList & out,const Json::Value & json)
{
	out.clear();
	for (auto it = json.begin() ; it != json.end() ; ++it)
		jsonToObj(out[it.key().asString()],*it);
}

/********************  STRUCT  **********************/
void PackageDef::jsonToObj(JsonMap & out,const Json::Value & json)
{
	out.clear();
	for (auto it = json.begin() ; it != json.end() ; ++it)
		out[it.key().asString()] = (*it);
}

/********************  STRUCT  **********************/
void PackageDef::toJson(Json::Value & out,const StringList & list)
{
	out.isArray();
	for (auto & value : list)
		out.append(value);
}

/********************  STRUCT  **********************/
void PackageDef::toJson(Json::Value & out,const StringMap & map)
{
	for (auto & it : map)
		out[it.first] = it.second;
}

/********************  STRUCT  **********************/
void PackageDef::toJson(Json::Value & out,const StringMapList & map)
{
	for (auto & it : map)
		toJson(out[it.first],it.second);
}

/********************  STRUCT  **********************/
void PackageDef::toJson(Json::Value & out,const JsonMap & map)
{
	for (auto & it : map)
		out[it.first] = it.second;
}

}
