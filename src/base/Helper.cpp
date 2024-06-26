/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
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
* Helper function to split strings with element containing at most 4096 characters.
**/
StringList Helper::split(const std::string & value,char separator,bool keepEmpty)
{
	//vars
	char buffer[8*4096];
	StringList res;
	
	//read
	int cnt = 0;
	for (size_t i = 0 ; i < value.size() ; i++)
	{
		if (value[i] == separator && (cnt > 0 || keepEmpty))
		{
			buffer[cnt] = '\0';
			cnt = 0;
			res.push_back(buffer);
		} else if (value[i] != separator) {
			buffer[cnt++] = value[i];
			assumeArg(cnt < sizeof(buffer),"Split element (on %2) is too large, should be shorter than %1 characters !").arg(sizeof(buffer)).arg(separator).end();
		}
	}
	
	//flush
	buffer[cnt] = '\0';
	if (cnt > 0)
		res.push_back(buffer);

	//ret
	return res;
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
* Check if a strin glist contain a given value.
* @param value String in which to search.
* @param what value to seach in string.
**/
bool Helper::contain(const StringList & value,const std::string what)
{
	//for (auto & it : value)
	forEachConst(StringList,it,value)
		if (*it == what)
			return true;
	return false;
}


/*******************  FUNCTION  *********************/
/**
* Concatenate
**/
std::string Helper::join(const std::list<std::string> & lst,char sep)
{
	std::string out;
	//for (auto & it : lst)
	forEachConst(StringList,it,lst)
	{
		if (out.empty() == false)
			out += sep;
		out+=*it;
	}
	return out;
}


/*******************  FUNCTION  *********************/
/**
 * Convert a JSON sub-tree to string list.
 * @param out Define the list to fill (will be cleared before)
 * @param json Define the JSON sub-tree to load.
**/
void Helper::jsonToObj(StringList & out,const Json::Value & json)
{
	out.clear();
	if (json.isArray())
	{
		int size = json.size();
		for (int i = 0 ; i < size ; i++)
			out.push_back(json[i].asString());
	} else if (json.isString()) {
		out.push_back(json.asString());
	}
}

/********************  STRUCT  **********************/
/**
 * Convert a JSON sub-tree to string map.
 * @param out Define the map to fill (will be cleared before)
 * @param json Define the JSON sub-tree to load.
**/
void Helper::jsonToObj(StringMap & out,const Json::Value & json)
{
	out.clear();
	for (Json::Value::const_iterator it = json.begin() ; it != json.end() ; ++it)
		out[it.key().asString()] = (*it).asString();
}

/********************  STRUCT  **********************/
/**
 * Convert a JSON sub-tree to map & list.
 * @param out Define the map to fill (will be cleared before)
 * @param json Define the JSON sub-tree to load.
**/
void Helper::jsonToObj(StringMapList & out,const Json::Value & json)
{
	out.clear();
	for (Json::Value::const_iterator it = json.begin() ; it != json.end() ; ++it)
		jsonToObj(out[it.key().asString()],*it);
}

/********************  STRUCT  **********************/
/**
 * Convert a JSON sub-tree to json map.
 * @param out Define the map to fill (will be cleared before)
 * @param json Define the JSON sub-tree to load.
**/
void Helper::jsonToObj(JsonMap & out,const Json::Value & json)
{
	out.clear();
	for (Json::Value::const_iterator it = json.begin() ; it != json.end() ; ++it)
		out[it.key().asString()] = (*it);
}

/********************  STRUCT  **********************/
/**
 * Convert the list to JSON tree
 * @param out Define the json node to fill
 * @param list Define the list to convert.
**/
void Helper::toJson(Json::Value & out,const StringList & list)
{
	out.isArray();
	//for (auto & value : list)
	forEachConst (StringList,it,list)
		out.append(*it);
}

/********************  STRUCT  **********************/
/**
 * Convert the map to JSON tree
 * @param out Define the json node to fill
 * @param list Define the map to convert.
**/
void Helper::toJson(Json::Value & out,const StringMap & map)
{
	//for (auto & it : map)
	forEachConst (StringMap,it,map)
		out[it->first] = it->second;
}

/********************  STRUCT  **********************/
/**
 * Convert the map list to JSON tree
 * @param out Define the json node to fill
 * @param list Define the map list to convert.
**/
void Helper::toJson(Json::Value & out,const StringMapList & map)
{
	//for (auto & it : map)
	forEachConst (StringMapList,it,map)
		toJson(out[it->first],it->second);
}

/********************  STRUCT  **********************/
/**
 * Convert the json map to JSON tree
 * @param out Define the json node to fill
 * @param list Define the map to convert.
**/
void Helper::toJson(Json::Value & out,const JsonMap & map)
{
	//for (auto & it : map)
	forEachConst (JsonMap,it,map)
		out[it->first] = it->second;
}

/********************  STRUCT  **********************/
/**
 * Merge two string map. It will copy all the element from override into
 * out. If some entries exist in out they will be overritten.
 * @param out Define the map to write in
 * @param override Define the map to merge on out.
**/
void Helper::merge(StringMap & out,const StringMap & override)
{
	//for (auto & it : override)
	forEachConst (StringMap,it,override)
		out[it->first] = it->second;
}

/********************  STRUCT  **********************/
/**
 * Merge two string list. It will append all the element from override into
 * out. It consider a particular case with entries in override starting with
 * the `!` character. In this case, it will remove all similar strings into
 * the list. Eg. if out contain `-O3 -ffast-math`, using `!-O3` will let only
 * `-ffast-math`.
 * @param out Define the list to write in
 * @param override Define the list to merge on out.
**/
void Helper::merge(StringList & out,const StringList & override)
{
	//for (auto & it : override)
	forEachConst (StringList,it,override)
	{
		if ((*it)[0] == '!')
		{
			std::string tmp = it->substr(1);
			for (StringList::iterator it2 = out.begin() ; it2 != out.end() ; ++it2)
				if (*it2 == tmp)
					it2 = out.erase(it2);
		} else {
			out.push_back(*it);
		}
	}
}

/********************  STRUCT  **********************/
/**
 * Merge two string list. It will append all the element from override into
 * out. It consider a particular case with entries in override starting with
 * the `!` character. In this case, it will remove all similar strings into
 * the list. Eg. if out contain `-O3 -ffast-math`, using `!-O3` will let only
 * `-ffast-math`.
 * @param out Define the list to write in
 * @param override Define the list to merge on out.
 * @param erase If defined to true it will replace the sub lists instead of
 * making merge.
**/
void Helper::merge(StringMapList & out,const StringMapList & override,bool erase)
{
	//for (auto & it : override)
	forEachConst (StringMapList,it,override)
	{
		if (out.find(it->first) == out.end() || erase)
			out[it->first] = it->second;
		else
			merge(out[it->first],it->second);
	}
}

/********************  STRUCT  **********************/
/**
 * Merge json nodes
**/
void Helper::merge(Json::Value & out,const Json::Value & override)
{
	//HL_DEBUG_ARG("Helper","Merge json : %1 on %2").arg(override).arg(out).end();
	if (override.isObject())
	{
		for (Json::Value::const_iterator it = override.begin() ; it != override.end() ; ++it)
				merge(out[it.key().asString()],*it);
	} else if (override.isArray()) {
		for (Json::ArrayIndex i = 0 ; i <override.size() ; i++)
			out.append(override[i]);
	} else if (override.isString() && override.asString().empty() == false) {
		out = override;
	} else if (override.isBool()) {
		out = override;
	}
}

/********************  STRUCT  **********************/
/**
 * Check if a string start by another one.
 * @param value String to check.
 * @param what Start string.
**/
bool Helper::startBy(const std::string & value,const std::string & what)
{
	//if too large
	if (what.size() > value.size())
		return false;
	
	//check
	return (strncmp(value.c_str(),what.c_str(),what.size()) == 0);
}

/********************  STRUCT  **********************/
/**
 * Check if a given string end by a reference.
 * @param value String to check
 * @param what Need to end by this.
**/
bool Helper::endBy(const std::string & value,const std::string & what)
{
	//if too large
	if (what.size() > value.size())
		return false;
	
	//check
	return (strncmp(value.c_str()+value.size()-what.size(),what.c_str(),what.size()) == 0);
}

/********************  STRUCT  **********************/
/**
 * Convert a string to lower case.
**/
std::string Helper::toLower(std::string value)
{
	//for (auto & el : value)
	for (int i = 0 ; i < value.size() ; i++)
		value[i] = std::tolower(value[i]);
	return value;
}

/*******************  FUNCTION  *********************/
/**
 * Return the last element of a string we split base on given character
**/
std::string Helper::last(const std::string& value, char sep)
{
	StringList buf = Helper::split(value,sep);
	return buf.back();
}

/******************* FUNCTION *********************/
/**
 * Escape the given character with backslash
 * @param value String to escape.
 * @param c Character to escape
**/
std::string Helper::escape(const std::string & value,char c)
{
	std::string out;
	//for (auto ch : value)
	for (int i = 0 ; i < value.size() ; i++)
	{
		if (value[i] == c)
			out += '\\';
		out += value[i];
	}	
	return out;
}

/******************* FUNCTION *********************/
/**
 * replace all occurences in the given string.
 * @param value The string in which to replace.
 * @param what The substring to replace
 * @param byWhat The replacement string.
**/
void Helper::replaceInPlace(std::string & value,const std::string what,const std::string & byWhat)
{
	size_t start_pos;
	size_t start = 0;
    while((start_pos = value.find(what,start)) != std::string::npos)
	{
	    value.replace(start_pos, what.size(), byWhat);
		start = start_pos+byWhat.size();
	}
}

}
