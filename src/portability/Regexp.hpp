/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_REGEXP_HPP
#define HL_REGEXP_HPP

/********************  HEADERS  *********************/
#include <string>
#include <sys/types.h>
#include <regex.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class Regexp
{
	public:
		Regexp(const std::string & pattern);
		~Regexp(void);
		bool match(const std::string & value);
		bool match(const std::string & value,std::string & capture1);
		bool match(const std::string & value,std::string & capture1,std::string & capture2);
		bool match(const std::string & value,std::string & capture1,std::string & capture2,std::string & capture3);
		bool match(const std::string & value,std::string & capture1,std::string & capture2,std::string & capture3, std::string & capture4);
	private:
		void matchToString(std::string & out,const std::string & value,regmatch_t & match);
	private:
		regex_t compiled;
};


}

#endif //HL_REGEXP_HPP
