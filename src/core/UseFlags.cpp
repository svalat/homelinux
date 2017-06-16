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
#include <base/Helper.hpp>
#include "UseFlags.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
UseFlags::UseFlags(void)
{
    //nothing to do
}

/*******************  FUNCTION  *********************/
/**
 * Constrcutor extring a list of flags to load
 * @param flags List of flags to load, they can conatin +/- to set state.
**/
UseFlags::UseFlags(const std::string & flags)
{
    this->merge(flags);
}

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
 * Add one flag
 * @param flag flag to add, could contain + or - to enable or disable.
**/
void UseFlags::addOne(const std::string & flag)
{
    //get flag name (removing -/+)
    std::string flagName = UseFlags::getFlagName(flag);
    
    //apply
    if (flag[0] == '-')
        stateMap[flagName] = FLAG_DISABLED;
    else if (flag[0] == '+')
        stateMap[flagName] = FLAG_ENABLED;
    else if (stateMap.find(flagName) == stateMap.end())
        stateMap[flagName] = FLAG_AUTO;
}

/*******************  FUNCTION  *********************/
/**
 * Add one flag
 * @param flagName Name of flag to add (not starting by +/-)
 * @param state State of flag to apply
**/
void UseFlags::addOne(const std::string & flagName,UseFlagState state)
{
    //apply
    if (state == FLAG_DISABLED || state == FLAG_ENABLED)
        stateMap[flagName] = state;
    else if (stateMap.find(flagName) == stateMap.end())
        stateMap[flagName] = FLAG_AUTO;
}

/*******************  FUNCTION  *********************/
/**
 * Replace all the AUTO flags to set them to the given state
 * @param state Define the state to use to replace AUTO.
**/
void UseFlags::setAuto(UseFlagState state)
{
    //loop on all
    for (auto & it : this->stateMap)
        if (it.second == FLAG_AUTO)
            it.second = state;
}

/*******************  FUNCTION  *********************/
/**
 * Convert the use flags to string only for the given state. This is used by toString
 * to give the flags in order.
 * @param out The string to complete (it will append)
 * @param state State to filter
 * @param force If force equal true, flas auto are set to enabled.
**/
void UseFlags::toStringByState(std::string & out,UseFlagState state,bool force)
{
    //loop on all
    for (auto & it : this->stateMap) 
    {
        //filter
        if (it.second == state || (it.second == FLAG_AUTO && force && state == FLAG_ENABLED))
        {
            //space
            if (out.empty() == false)
                out += " ";
            
            //flag
            if (state == FLAG_ENABLED) {
                out += "+";
                out += it.first;
            } else if (state == FLAG_DISABLED) {
                out += "-";
                out += it.first;
            } else if (state == FLAG_AUTO && force) {
                out += "+";
                out += it.first;
            } else {
                out += it.first;
            }
        }
    }
}

/*******************  FUNCTION  *********************/
std::string UseFlags::toString(bool force)
{
    //var
    std::string res;
    
    //enabled
    toStringByState(res,FLAG_ENABLED,force);
    toStringByState(res,FLAG_DISABLED,force);
    if (!force)
        toStringByState(res,FLAG_AUTO,false);
    
    //ret
    return res;
}

/*******************  FUNCTION  *********************/
/**
 * Merge the list of flags to the current state
**/
UseFlags & UseFlags::merge(const std::string & flags)
{
    Helper::stringSplit(flags,' ',[this](const std::string & value){
        this->addOne(value);
    });
}

/*******************  FUNCTION  *********************/
UseFlags & UseFlags::merge(const UseFlags & flags)
{
    //loop
    for (auto & it : stateMap)
        addOne(it.first,it.second);
}

}
