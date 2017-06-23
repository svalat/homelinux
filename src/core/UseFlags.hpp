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
#include <map>
#include <json/json.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  ENUM  ************************/
enum UseFlagState
{
	FLAG_DISABLED,
	FLAG_ENABLED,
	FLAG_AUTO
};

/********************* TYPES ************************/
typedef std::map<std::string,UseFlagState> UseFlagStateMap;

/*********************  CLASS  **********************/
class UseFlags
{
	public:
		UseFlags(void);
		UseFlags(const std::string & flags);
		UseFlags & merge(const std::string & flags,bool onlyIfExist = false);
		UseFlags & merge(const UseFlags & flags,bool onlyIfExist = false);
		UseFlagState getStatus(const std::string & flag) const;
		UseFlagState getApplyStatus(const std::string & flag) const;
		UseFlagState getApplyStatusWithAnd(const std::string & flag) const;
		bool hasFlag(const std::string & name) const;
		void fromJson(const Json::Value & json);
		void toJson(Json::Value & json,bool force = false);
		void setAuto(UseFlagState state = FLAG_ENABLED);
		std::string toString(bool force = false,bool colred = false);
		//statics
		static std::string getFlagName(const std::string & flag);
	private:
		void addOne(const std::string & flag,bool onlyIfExist = false);
		void addOne(const std::string & flagName,UseFlagState state,bool onlyIfExist = false);
		void toJsonByState(Json::Value & json,UseFlagState state,bool force = false);
		void toStringByState(std::string & out,UseFlagState state,bool force = false);
		void toStringByStateColored(std::string & out,UseFlagState state,bool force = false);
	private:
		UseFlagStateMap stateMap;
};

}

#endif //HL_USE_FLAGS_HPP
