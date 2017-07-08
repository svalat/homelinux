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
#include <base/Colors.hpp>
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
void UseFlags::addOne(const std::string & flag,bool onlyIfExist)
{
	//get flag name (removing -/+)
	std::string flagName = UseFlags::getFlagName(flag);

	//only if exist
	if (onlyIfExist)
		if (stateMap.find(flagName) == stateMap.end())
			return;
	
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
void UseFlags::addOne(const std::string & flagName,UseFlagState state,bool onlyIfExist)
{
	//only if exist
	if (onlyIfExist)
		if (stateMap.find(flagName) == stateMap.end())
			return;

	//apply
	if (state == FLAG_DISABLED || state == FLAG_ENABLED)
		stateMap[flagName] = state;
	else if (stateMap.find(flagName) == stateMap.end())
		stateMap[flagName] = FLAG_AUTO;
}

/*******************  FUNCTION  *********************/
/**
* Get the status of the given flag
**/
UseFlagState UseFlags::getStatus(const std::string & flag) const
{
	//get name
	std::string flagName = getFlagName(flag);
	
	//search
	UseFlagStateMap::const_iterator it = stateMap.find(flagName);
	
	//select
	if (it == stateMap.end())
		HL_THROW_ARG("Undefined flag %1").arg(flagName).end();

	return it->second;
}

/*******************  FUNCTION  *********************/
/**
* Same as getApplyStatus but managing multiple flags
* with & operation.
**/
UseFlagState UseFlags::getApplyStatusWithAnd(const std::string & flag) const
{
	if (flag.find('&') == std::string::npos)
	{
		return getApplyStatus(flag);
	} else {
		UseFlagState state = FLAG_ENABLED;
		StringList flags = Helper::split(flag,'&');
		forEach(StringList,it,flags)
		{
			UseFlagState tmp = getApplyStatus(*it);
			if (tmp == FLAG_AUTO)
				state = FLAG_AUTO;
			else if (state == FLAG_ENABLED && tmp == FLAG_DISABLED)
				state = FLAG_DISABLED;
		}
		return state;
	}
}

/*******************  FUNCTION  *********************/
/**
* Check if the flag request need to be applied or not (for configure and deps).
* It als
* @param flag Flag string potentially with `+` or `-`. `-` will invert the return value.
**/
UseFlagState UseFlags::getApplyStatus(const std::string & flag) const
{
	//trivial
	if (flag == "" || flag == "+" || flag == "always" || flag == "+always")
		return FLAG_ENABLED;
	
	//select mode
	UseFlagState mode = FLAG_AUTO;
	if (flag[0] == '+')
		mode = FLAG_ENABLED;
	else if (flag[0] == '-')
		mode = FLAG_DISABLED;

	//apply
	UseFlagState status = this->getStatus(flag);
	if (mode == FLAG_AUTO)
		return status;
	else if (mode == FLAG_ENABLED && status == FLAG_ENABLED)
		return FLAG_ENABLED;
	else if (mode == FLAG_DISABLED && status == FLAG_DISABLED) 
		return FLAG_ENABLED;
	else
		return FLAG_AUTO;
}

/*******************  FUNCTION  *********************/
/**
* Replace all the AUTO flags to set them to the given state
* @param state Define the state to use to replace AUTO.
**/
void UseFlags::setAuto(UseFlagState state)
{
	//loop on all
	//for (auto & it : this->stateMap)
	forEach(UseFlagStateMap,it,stateMap)
		if (it->second == FLAG_AUTO)
			it->second = state;
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
	//for (auto & it : this->stateMap) 
	forEach(UseFlagStateMap,it,stateMap)
	{
		//filter
		if (it->second == state || (it->second == FLAG_AUTO && force && state == FLAG_ENABLED))
		{
			//space
			if (out.empty() == false)
				out += " ";
			
			//flag
			if (state == FLAG_ENABLED) {
				out += "+";
				out += it->first;
			} else if (state == FLAG_DISABLED) {
				out += "-";
				out += it->first;
			} else if (state == FLAG_AUTO && force) {
				out += "+";
				out += it->first;
			} else {
				out += it->first;
			}
		}
	}
}

/*******************  FUNCTION  *********************/
/**
* Convert the use flags to string only for the given state. This is used by toString
* to give the flags in order.
* @param out The string to complete (it will append)
* @param state State to filter
* @param force If force equal true, flas auto are set to enabled.
**/
void UseFlags::toStringByStateColored(std::string & out,UseFlagState state,bool force)
{
	//loop on all
	//for (auto & it : this->stateMap) 
	forEach(UseFlagStateMap,it,stateMap)
	{
		//filter
		if (it->second == state || (it->second == FLAG_AUTO && force && state == FLAG_ENABLED))
		{
			//space
			if (out.empty() == false)
				out += " ";
			
			//flag
			if (state == FLAG_ENABLED) {
				out += Colors::cyan("+" + it->first);
			} else if (state == FLAG_DISABLED) {
				out += Colors::red("-" + it->first);
			} else if (state == FLAG_AUTO && force) {
				out += Colors::cyan("+" + it->first);
			} else {
				out += Colors::blue(it->first);
			}
		}
	}
}

/*******************  FUNCTION  *********************/
std::string UseFlags::toString(bool force,bool colored)
{
	//var
	std::string res;
	
	if (colored)
	{
		toStringByStateColored(res,FLAG_ENABLED,force);
		toStringByStateColored(res,FLAG_DISABLED,force);
		if (!force)
			toStringByStateColored(res,FLAG_AUTO,false);
	} else {
		toStringByState(res,FLAG_ENABLED,force);
		toStringByState(res,FLAG_DISABLED,force);
		if (!force)
			toStringByState(res,FLAG_AUTO,false);
	}
	
	//ret
	return res;
}


/*******************  FUNCTION  *********************/
/**
 * Load from json.
**/
void UseFlags::fromJson(const Json::Value & json)
{
	if (json.isArray())
		for (Json::ArrayIndex i = 0 ; i < json.size() ; i++)
			addOne(json[i].asString());
}

/*******************  FUNCTION  *********************/
/**
 * Save flags to json
 * @param json Json node in which to dump.
 * @param force Replace AUTO by `+`.
**/
void UseFlags::toJson(Json::Value & json,bool force)
{
	toJsonByState(json,FLAG_ENABLED,force);
	toJsonByState(json,FLAG_DISABLED,force);
	if (!force)
		toJsonByState(json,FLAG_AUTO,force);
}

/*******************  FUNCTION  *********************/
/**
* Convert the use flags to string only for the given state. This is used by toString
* to give the flags in order.
* @param out The string to complete (it will append)
* @param state State to filter
* @param force If force equal true, flas auto are set to enabled.
**/
void UseFlags::toJsonByState(Json::Value & json,UseFlagState state,bool force)
{
	//loop on all
	//for (auto & it : this->stateMap)
	forEach(UseFlagStateMap,it,stateMap) 
	{
		//filter
		if (it->second == state || (it->second == FLAG_AUTO && force && state == FLAG_ENABLED))
		{
			//flag
			if (state == FLAG_ENABLED) {
				json.append("+" + it->first);
			} else if (state == FLAG_DISABLED) {
				json.append("-" + it->first);
			} else if (state == FLAG_AUTO && force) {
				json.append("+" + it->first);
			} else {
				json.append(it->first);
			}
		}
	}
}

/*******************  FUNCTION  *********************/
/**
* Merge the list of flags to the current state
**/
UseFlags & UseFlags::merge(const std::string & flags,bool onlyIfExist)
{
	StringList lst = Helper::split(flags,' ');
	forEach(StringList,it,lst)
		this->addOne(*it,onlyIfExist);
	return *this;
}

/*******************  FUNCTION  *********************/
UseFlags & UseFlags::merge(const UseFlags & flags,bool onlyIfExist)
{
	//loop
	//for (auto & it : flags.stateMap)
	forEachConst(UseFlagStateMap,it,flags.stateMap)
		addOne(it->first,it->second,onlyIfExist);
	return *this;
}

/*******************  FUNCTION  *********************/
bool UseFlags::hasFlag(const std::string & name) const
{
	return (stateMap.find(name) != stateMap.end());
}

}
