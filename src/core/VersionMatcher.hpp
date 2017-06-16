/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_VERSION_MATCHER_HPP
#define HL_VERSION_MATCHER_HPP

/********************  HEADERS  *********************/
//std
#include <string>
#include <map>
#include <list>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************* TYPES ************************/
typedef std::map<std::string, std::string> SlotDef;
typedef std::list<std::string> VersionList;
typedef std::list<std::string> VersionMatcherRuleList;

/********************  GLOBALS  *********************/
extern const SlotDef gblSlotNone;

/*********************  CLASS  **********************/
class VersionMatcher
{
    public: 
        VersionMatcher(const std::string & rules);
        bool match(const std::string & version,const SlotDef & slots = gblSlotNone);
        void filterList(VersionList & list,const SlotDef & slots = gblSlotNone);
        static VersionList sortList(const VersionList & list);
        static int compareVersion(const std::string & v1,const std::string & v2);
        static std::string fillNumber(const std::string & value,int pad = 16);
    private:
        bool applyVersionOperator(const std::string & op,const std::string version,const SlotDef & slots = gblSlotNone);
        void set(const std::string & rules);
        VersionMatcherRuleList ruleList;
};

}

#endif //HL_VERSION_MATCHER_HPP
