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

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
struct Helper
{
    static void stringSplit(const std::string & value,char separator,std::function<void(const std::string&)> callback);
};

}

#endif //HL_HELPER_HPP
