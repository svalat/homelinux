/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_HELPER_HPP
#define HL_HELPER_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <functional>
#include <list>
#include <map>
#include "Debug.hpp"

/*******************  TYPES  ************************/
//to avoid to include json.hpp everywhere
namespace Json
{
    class Value;
}

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  TYPES  ************************/
typedef std::map<std::string,std::string> StringMap;
typedef std::map<std::string,std::list<std::string>> StringMapList;
typedef std::list<std::string> StringList;
typedef std::map<std::string,Json::Value> JsonMap;

/********************  STRUCT  **********************/
struct Helper
{
	static void stringSplit(const std::string & value,char separator,std::function<void(const std::string&)> callback);
	static bool contain(const std::string & value,const std::string what);
	static std::string join(const std::list<std::string> & lst,char sep);
	template <class T> static T getListEl(const std::list<T> & lst,int id);
	static void jsonToObj(StringList & out,const Json::Value & json);
	static void jsonToObj(StringMap & out,const Json::Value & json);
	static void jsonToObj(StringMapList & out, const Json::Value & json);
	static void jsonToObj(JsonMap & out,const Json::Value & json);
	static void toJson(Json::Value & out,const StringList & list);
	static void toJson(Json::Value & out,const StringMap & map);
	static void toJson(Json::Value & out,const StringMapList & mapList);
	static void toJson(Json::Value & out,const JsonMap & map);
	static void merge(StringMap & out,const StringMap & override);
	static void merge(StringList & out,const StringList & override);
	static void merge(StringMapList & out,const StringMapList & override,bool erase);
};

/*******************  FUNCTION  *********************/
template <class T> 
T Helper::getListEl(const std::list<T> & lst,int id)
{
    assumeArg(id < lst.size(),"Cannot get element %1, list size is %2").arg(id).arg(lst.size());
    auto it = lst.begin();
    for (int i = 0 ; i < id ; i++)
        ++it;
    return *it;
}

}

#endif //HL_HELPER_HPP
