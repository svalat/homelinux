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
#include <regex>
#include <re2/re2.h>
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
/**
* Apply all the selection rules on the given verison to check if it match.
* @parma version Define the version to match.
* @param slots Give definition of slots to filter with slot operator.
**/
bool VersionMatcher::match(const std::string & version,const SlotDef & slots)
{
	for (auto & rule : ruleList)
		if (applyVersionOperator(rule,version,slots) == false)
			return false;
	return true;
}

/*******************  FUNCTION  *********************/
VersionList VersionMatcher::filterList(const VersionList & list,const SlotDef & slots)
{
	VersionList out;
	for (auto & version : list)
		if (match(version,slots))
			out.push_back(version);
	return out;
}

/*******************  FUNCTION  *********************/
static bool versionSortCompare(const std::string & a,const std::string & b)
{
	return VersionMatcher::compareVersion(a,b) == 1;
}

/*******************  FUNCTION  *********************/
VersionList VersionMatcher::sortList(VersionList list)
{
	list.sort(versionSortCompare);
	return list;
}

/*******************  FUNCTION  *********************/
bool VersionMatcher::applyVersionOperator(const std::string & op,const std::string version,const SlotDef & slots)
{
	//vars
	int tmp;
	int cnt = 1;
	bool ret;
	std::string oper;
	oper += op[0];
	
	//manage >= & <=
	if (op[1] == '=')
	{
		cnt++;
		oper+='=';
	}
	
	//extract operarant
	std::string operand = op.substr(cnt);
	
	//aplly operator
	if (oper == "<=")
	{
		tmp = compareVersion(version,operand);
		ret = (tmp == 0 || tmp == -1);
	} else if (oper == "<") {
		ret = (compareVersion(version,operand) == -1);
	} else if (oper == ">=") {
		tmp = compareVersion(version,operand);
		ret = (tmp == 0 || tmp == 1);
	} else if (oper == ">") {
		ret = (compareVersion(version,operand) == 1);
	} else if (oper == "=") {
		ret = (compareVersion(version,operand) == 0);
	} else if (oper == "!") {
		ret = (compareVersion(version,operand) != 0);
	} else if (oper == "~") {
		std::regex reg(regexpReplPoint(operand)+".*");
		ret = std::regex_match(version,reg);
	} else if (oper == ":") {
		std::string slot = getSlot(slots,version);
		ret = (slot == operand);
	} else {
		HL_FATAL_ARG("Invalid operator %1 in %2 !").arg(oper).arg(op).end();
	}

	//console.error("Apply "+operator +" on "+operand+" and "+version+" => "+ret);
	return ret;
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
	Helper::split(rules,' ',[this](const std::string & value){
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
	Helper::split(v1,'.',[&v1Details](const std::string& value){v1Details.push_back(fillNumber(value,8));});
	std::vector<std::string> v2Details;
	Helper::split(v2,'.',[&v2Details](const std::string& value){v2Details.push_back(fillNumber(value,8));});
	
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

/*******************  FUNCTION  *********************/
std::string VersionMatcher::getSlot(const SlotDef & slots,const std::string & version)
{
	//loop
	for (auto & it : slots)
	{
		if (it.first == "~")
		{
			//vars
			std::string extract;
			std::string prepared = regexpReplPoint(it.second)+".*";
			
			//match & extract
			if(RE2::FullMatch(version,prepared,&extract)) {
				return extract;
			} else {
				HL_FATAL_ARG("Fail to match versoin to search slot, regexp is %1 and verison is %2")
					.arg(prepared)
					.arg(version)
					.end();
			}
		} else {
			VersionMatcher matcher(it.second);
			if (matcher.match(version))
				return it.first;
		}
	}
	
	//not slot found
	return "0";
}

/*******************  FUNCTION  *********************/
std::string VersionMatcher::regexpReplPoint(const std::string & value)
{
	std::string res;
	bool lastIsProtect = false;
	for (auto c : value)
	{
		if (c == '.' && !lastIsProtect)
			res += "\\.";
		else
			res += c;
		if (c == '\\')
			lastIsProtect = true;
		else
			lastIsProtect = false;
	}
	return res;
}

}
