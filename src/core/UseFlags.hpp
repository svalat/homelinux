/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_USE_FLAGS_HPP
#define HL_USE_FLAGS_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <list>

/*******************  NAMESPACE  ********************/
namespace hl
{
    
/********************* TYPES ************************/
typedef std::list<std::string> FlagList;

/********************  STRUCT  **********************/
/**
 * Structure to store the configuration of HLPIPE.
**/
struct UseFlags
{
    static std::string getFlagName(const std::string & flag);
    static void merge(FlagList & out,const FlagList & in1,const FlagList & in2,bool force = false);
    static void merge(FlagList & out,const std::string & in1,const std::string & in2,bool force = false);
    static std::string mergeString(const std::string & in1,const std::string & in2,bool force = false);
    static FlagList toFlagList(const std::string & value);
};

/*******************  FUNCTION  *********************/
std::string & operator<<(std::string & str,const FlagList & flags);
FlagList & operator<<(FlagList & flags,const std::string & value);

}

#endif //HL_USE_FLAGS_HPP
