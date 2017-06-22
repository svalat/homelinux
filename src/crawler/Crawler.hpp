/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_CRAWLER_HPP
#define HL_CRAWLER_HPP

/********************  HEADERS  *********************/
//std
#include <string>
//external
#include <json/json.h>
#include <re2/re2.h>
//internal
#include <base/Helper.hpp>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class Crawler
{
	public:
		Crawler(const std::string & name);
		virtual ~Crawler(void);
		StringList run(Json::Value & params,const StringList & origVersions);
		const std::string & getName(void) const;
	protected:
		virtual void internalRun(const std::string & url) = 0;
		void scanValue(const std::string & value);
	protected:
		StringList versions;
		RE2 * regexp;
	private:
		std::string name;
};

}

#endif //HL_CRAWLER_HPP
