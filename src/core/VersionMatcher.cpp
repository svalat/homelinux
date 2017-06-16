/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <map>
#include <algorithm>
#include <cstring>
#include <base/Debug.hpp>
#include <base/Helper.hpp>
#include "VersionMatcher.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  GLOBALS  *********************/
const SlotDef gblSlotNone;

/*******************  FUNCTION  *********************/
/**
 * Constructor. It set the rules. Rules are composed of sub rule elements
 * separated by space(s). An example could be : `>=3.4 <3.5`.
 * @param rules Define the rules to setup.
**/
VersionMatcher::VersionMatcher(const std::string & rules)
{
    this->set(rules);
}

/*******************  FUNCTION  *********************/
bool VersionMatcher::match(const std::string & version,const SlotDef & slots)
{
    
}

/*******************  FUNCTION  *********************/
void VersionMatcher::filterList(VersionList & list,const SlotDef & slots)
{
    
}

/*******************  FUNCTION  *********************/
VersionList VersionMatcher::sortList(const VersionList & list)
{
    
}

/*******************  FUNCTION  *********************/
bool VersionMatcher::applyVersionOperator(const std::string & op,const std::string version,const SlotDef & slots)
{
    
}

/*******************  FUNCTION  *********************/
/**
 * Constructor. It set the rules. Rules are composed of sub rule elements
 * separated by space(s). An example could be : `>=3.4 <3.5`.
 * @param rules Define the rules to setup.
**/
void VersionMatcher::set(const std::string & rules)
{
    ruleList.clear();
    Helper::stringSplit(rules,' ',[this](const std::string & value){
        this->ruleList.push_back(value);
    });
}

/*******************  FUNCTION  *********************/
/**
 * Compare two version number and return 1 for larger, 0 for equal and -1 for smaller.
 * Vesion can be formed as 1.35.3a
**/
int VersionMatcher::compareVersion(const std::string & v1,const std::string & v2)
{
    //split
    std::vector<std::string> v1Details;
    Helper::stringSplit(v1,'.',[&v1Details](const std::string& value){v1Details.push_back(fillNumber(value,8));});
    std::vector<std::string> v2Details;
    Helper::stringSplit(v2,'.',[&v2Details](const std::string& value){v2Details.push_back(fillNumber(value,8));});
    
    //fill
    std::string zero(12,'0');
    int m = std::max(v1Details.size(),v2Details.size());
    for (int i = 0 ; i < m ; i++)
    {
        if (i >= v1Details.size())
            v1Details.push_back(zero);
        if (i >= v2Details.size())
            v2Details.push_back(zero);
    }
    
    //compare
    for (int i = 0 ; i < m ; i++)
    {
        int res = strcmp(v1Details[i].c_str(),v2Details[i].c_str());
        //HL_INFO_ARG("Compare %1 > %2 => %3").arg(v1Details[i]).arg(v2Details[i]).arg(res).end();
        if (res > 0)
            return 1;
        else if (res < 0)
            return -1;
    }
    
    //ok this is equal
    return 0;
}

/*******************  FUNCTION  *********************/
/**
 * Fill all the number component such a way they ues the same size.
 * Eg. 1.3.35a becore 0001.0003.0035a-beta
**/
std::string VersionMatcher::fillNumber(const std::string & value,int pad)
{
    //search first non number
    int endNum = 0;
    while (value[endNum] >= '0' && value[endNum] <= '9')
        endNum++;
    
    //build repeated 0 for padding
    std::string fillPre(pad - endNum,'0');
    std::string fillPost(pad - (value.size() - endNum),'0');
    
    //aggregate
    return fillPre + value + fillPost;
}

}
