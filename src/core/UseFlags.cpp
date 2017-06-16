/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <map>
#include <base/Debug.hpp>
#include "UseFlags.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
/**
 * Exctract the flags name removine the enable/disable symbol.
 * So, `+flag` and `-flag` becomes `flag`.
 * @param flag string to convert
**/
std::string UseFlags::getFlagName(const std::string & flag)
{
    if (flag[0] == '+' || flag[0] == '-')
        return flag.substr(1);
    else
        return flag;
}

/*******************  FUNCTION  *********************/
/**
 * Merge two list of use flags by removing the double definition.
 * It also manage the status changing if `+flags` follow `-flag` or
 * invert.
 * @param values Define the initial list of flags
 * @param additional Define the list of flags to add.
 * @param force Force usage of `+` and `-`.
**/
std::string UseFlags::mergeString(const std::string & in1,const std::string & in2,bool force)
{
    //var
    FlagList lst;
    
    //merge
    merge(lst,in1,in2,force);
    
    //convert
    std::string res;
    res << lst;
    
    //return
    return res;
}

/*******************  FUNCTION  *********************/
/**
 * Merge two list of use flags by removing the double definition.
 * It also manage the status changing if `+flags` follow `-flag` or
 * invert.
 * @param values Define the initial list of flags
 * @param additional Define the list of flags to add.
 * @param force Force usage of `+` and `-`.
**/
void UseFlags::merge(FlagList & out,const std::string & in1,const std::string & in2,bool force)
{
    //var
    FlagList lst1;
    FlagList lst2;
    
    //convert
    lst1 << in1 << in2;
    
    //call
    merge(out,lst1,lst2,force);
}

/*******************  FUNCTION  *********************/
/**
 * Merge two list of use flags by removing the double definition.
 * It also manage the status changing if `+flags` follow `-flag` or
 * invert.
 * @param values Define the initial list of flags
 * @param additional Define the list of flags to add.
 * @param force Force usage of `+` and `-`.
**/
void UseFlags::merge(FlagList & out,const FlagList & in1,const FlagList & in2,bool force)
{
    //clear
    out.clear();
    
    //trivial
    if (in1.empty() && in2.empty())
        return;
    
    //concat
    FlagList flags;
    flags = in1;
    flags.insert(flags.end(),in2.begin(),in2.end());
    
    //loop and build status list
    std::map<std::string,const char*> status;
    for (auto flag : flags)
    {
        //get name
        std::string flagName = UseFlags::getFlagName(flag);
        
        //apply status
        if (flag[0] == '-')
            status[flagName] = "-";
        else if (flag[0] == '+')
            status[flagName] = "+";
        else if (force)
            status[flagName] = "+";
        else if (status.find(flagName) == status.end())
            status[flagName] = "";
    }

    //build list
    for (auto it : status)
        out.push_back(std::string(it.second)+it.first);
}

/*******************  FUNCTION  *********************/
/**
 * Convert a flag list to string, usefull for unit testing.
 * @param str String to setup
 * @parma flags List of flags to convert.
**/
std::string & operator<<(std::string & str,const FlagList & flags)
{
    //apply
    bool first = true;
    for(auto flag : flags) 
    {
        if (first)
            first = false;
        else
            str += ' ';
        str += flag;
    }
    
    //ret
    return str;
}

/*******************  FUNCTION  *********************/
/**
 * Stream a flag list in string format to flag list
**/
FlagList & operator<<(FlagList & flags,const std::string & value)
{
    //vars
    char buffer[1024];
    
    //read
    int cnt = 0;
    for (int i = 0 ; i < value.size() ; i++)
    {
        if (value[i] == ' ' && cnt > 0)
        {
            buffer[cnt] = '\0';
            cnt = 0;
            flags.push_back(buffer);
        } else if (value[i] != ' ') {
            buffer[cnt++] = value[i];
            assume(cnt < 1024,"Flag name is too large");
        }
    }
    
    //flush
    buffer[cnt] = '\0';
    if (cnt > 0)
        flags.push_back(buffer);
    
    //ret
    return flags;
}

}
