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
#include "Debug.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
struct Helper
{
	static void stringSplit(const std::string & value,char separator,std::function<void(const std::string&)> callback);
	static bool contain(const std::string & value,const std::string what);
	static std::string join(const std::list<std::string> & lst,char sep);
	template <class T> static T getListEl(const std::list<T> & lst,int id);
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
