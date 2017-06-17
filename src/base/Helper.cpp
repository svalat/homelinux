/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include "Debug.hpp"
#include "Helper.hpp"
#include <cstring>
#include <json/json.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
/**
* Helper function to split strings with element containing at most 1024 characters.
**/
void Helper::stringSplit(const std::string & value,char separator,std::function<void(const std::string&)> callback)
{
	//vars
	char buffer[1024];
	
	//read
	int cnt = 0;
	for (int i = 0 ; i < value.size() ; i++)
	{
		if (value[i] == separator && cnt > 0)
		{
			buffer[cnt] = '\0';
			cnt = 0;
			callback(buffer);
		} else if (value[i] != separator) {
			buffer[cnt++] = value[i];
			assume(cnt < 1024,"Split element is too large, should be shorter than 1024 characters !");
		}
	}
	
	//flush
	buffer[cnt] = '\0';
	if (cnt > 0)
		callback(buffer);
}

/*******************  FUNCTION  *********************/
/**
* Check if a string contain a given value.
* @param value String in which to search.
* @param what value to seach in string.
**/
bool Helper::contain(const std::string & value,const std::string what)
{
	return value.find(what) != std::string::npos;
}

/*******************  FUNCTION  *********************/
/**
* Concatenate
**/
std::string Helper::join(const std::list<std::string> & lst,char sep)
{
	std::string out;
	for (auto & value : lst)
	{
		if (out.empty() == false)
			out += sep;
		out+=value;
	}
	return out;
}


/*******************  FUNCTION  *********************/
void Helper::jsonToObj(StringList & out,const Json::Value & json)
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
void Helper::jsonToObj(StringMap & out,const Json::Value & json)
{
	out.clear();
	for (auto it = json.begin() ; it != json.end() ; ++it)
		out[it.key().asString()] = (*it).asString();
}

/********************  STRUCT  **********************/
void Helper::jsonToObj(StringMapList & out,const Json::Value & json)
{
	out.clear();
	for (auto it = json.begin() ; it != json.end() ; ++it)
		jsonToObj(out[it.key().asString()],*it);
}

/********************  STRUCT  **********************/
void Helper::jsonToObj(JsonMap & out,const Json::Value & json)
{
	out.clear();
	for (auto it = json.begin() ; it != json.end() ; ++it)
		out[it.key().asString()] = (*it);
}

/********************  STRUCT  **********************/
void Helper::toJson(Json::Value & out,const StringList & list)
{
	out.isArray();
	for (auto & value : list)
		out.append(value);
}

/********************  STRUCT  **********************/
void Helper::toJson(Json::Value & out,const StringMap & map)
{
	for (auto & it : map)
		out[it.first] = it.second;
}

/********************  STRUCT  **********************/
void Helper::toJson(Json::Value & out,const StringMapList & map)
{
	for (auto & it : map)
		toJson(out[it.first],it.second);
}

/********************  STRUCT  **********************/
void Helper::toJson(Json::Value & out,const JsonMap & map)
{
	for (auto & it : map)
		out[it.first] = it.second;
}

/********************  STRUCT  **********************/
void Helper::merge(StringMap & out,const StringMap & override)
{
	for (auto & it : override)
		out[it.first] = it.second;
}

/********************  STRUCT  **********************/
void Helper::merge(StringList & out,const StringList & override)
{
	for (auto & it : override)
	{
		if (it[0] == '!')
		{
			std::string tmp = it.substr(1);
			for (auto it = out.begin() ; it != out.end() ; ++it)
				if (*it == tmp)
					it = out.erase(it);
		} else {
			out.push_back(it);
		}
	}
}

/********************  STRUCT  **********************/
void Helper::merge(StringMapList & out,const StringMapList & override,bool erase)
{
	for (auto & it : override)
	{
		if (out.find(it.first) == out.end() || erase)
			out[it.first] = it.second;
		else
			merge(out[it.first],it.second);
	}
}

/********************  STRUCT  **********************/
bool Helper::startBy(const std::string & v1,const std::string & v2)
{
	//if too large
	if (v2.size() > v1.size())
		return false;
	
	//check
	return (strncmp(v1.c_str(),v2.c_str(),v2.size()) == 0);
}

}
