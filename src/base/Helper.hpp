/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
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
#include <vector>
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
typedef std::map<std::string,std::list<std::string> > StringMapList;
typedef std::list<std::string> StringList;
typedef std::map<std::string,Json::Value> JsonMap;
typedef std::vector<std::string> StringVector;

#define forEach(type,it,list) for(type::iterator it = list.begin() ; it != list.end() ; ++it)
#define forEachConst(type,it,list) for(type::const_iterator it = list.begin() ; it != list.end() ; ++it)

/********************  STRUCT  **********************/
/**
 * Helper class providing basic functions used everywhere in homelinux.
**/
struct Helper
{
	static StringList split(const std::string & value,char separator,bool keepEmpty = false);
	static std::string last(const std::string & value, char sep);
	static bool contain(const std::string & value,const std::string what);
	static bool contain(const StringList & value,const std::string what);
	static void replaceInPlace(std::string & value,const std::string what,const std::string & byWhat);
	static std::string join(const std::list<std::string> & lst,char sep);
	static std::string escape(const std::string & value,char c);
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
	static void merge(Json::Value & out,const Json::Value & override);
	static void merge(StringMapList & out,const StringMapList & override,bool erase);
	static bool startBy(const std::string & value,const std::string & what);
	static bool endBy(const std::string & value,const std::string & what);
	static std::string toLower(std::string value);
};

/*******************  FUNCTION  *********************/
/**
 * Return the N-th element from the list. If will fail if the ID is not in the list.
 * @param lst List to search in
 * @param id ID of the element to return.
**/
template <class T> 
T Helper::getListEl(const std::list<T> & lst,int id)
{
    assumeArg(id < lst.size(),"Cannot get element %1, list size is %2").arg(id).arg(lst.size());
    typename std::list<T>::const_iterator it = lst.begin();
    for (int i = 0 ; i < id ; i++)
        ++it;
    return *it;
}

}

#endif //HL_HELPER_HPP
