/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <base/Debug.hpp>
#include "Regexp.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  FUNCTION  *********************/
Regexp::Regexp(const std::string & pattern)
{
	std::string p = "^"+pattern+"$";
	int status = regcomp (&compiled, p.c_str(), REG_EXTENDED);
	if (status != 0)
	{
		char buf[1024];
		regerror(status,&compiled,buf,sizeof(buf));
		HL_THROW_ARG("Fail to compile regexp %1 : %2").arg(p).arg(buf).end();
	}
}

/********************  FUNCTION  *********************/
Regexp::~Regexp(void)
{
		regfree(&compiled);
}

/********************  FUNCTION  *********************/
bool Regexp::match(const std::string & value)
{
	int status = regexec(&compiled,value.c_str(),0,NULL,0);
	return (status == 0);
}

/********************  FUNCTION  *********************/
bool Regexp::match(const std::string & value,std::string & capture1)
{
	regmatch_t match[8];
	int status = regexec(&compiled,value.c_str(),8,match,0);
	if (status == 0)
		matchToString(capture1,value,match[1]);
	return (status == 0);
}

/********************  FUNCTION  *********************/
bool Regexp::match(const std::string & value,std::string & capture1,std::string & capture2)
{
	regmatch_t match[8];
	int status = regexec(&compiled,value.c_str(),8,match,0);
	if (status == 0)
	{
		matchToString(capture1,value,match[1]);
		matchToString(capture2,value,match[2]);
	}
	return (status == 0);
}

/********************  FUNCTION  *********************/
bool Regexp::match(const std::string & value,std::string & capture1,std::string & capture2,std::string & capture3)
{
	regmatch_t match[8];
	int status = regexec(&compiled,value.c_str(),8,match,0);
	if (status == 0)
	{
		matchToString(capture1,value,match[1]);
		matchToString(capture2,value,match[2]);
		matchToString(capture3,value,match[3]);
	}
	return (status == 0);
}

/********************  FUNCTION  *********************/
bool Regexp::match(const std::string & value,std::string & capture1,std::string & capture2,std::string & capture3, std::string & capture4)
{
	regmatch_t match[8];
	int status = regexec(&compiled,value.c_str(),8,match,0);
	if (status == 0)
	{
		matchToString(capture1,value,match[1]);
		matchToString(capture2,value,match[2]);
		matchToString(capture3,value,match[3]);
		matchToString(capture4,value,match[4]);
	}
	return (status == 0);
}

/********************  FUNCTION  *********************/
void Regexp::matchToString(std::string & out,const std::string & value,regmatch_t & match)
{
	out = "";
	for (int i = match.rm_so ; i < match.rm_eo ; i++)
		out += value[i];
}

}
