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

}
