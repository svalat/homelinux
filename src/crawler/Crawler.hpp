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
		StringList run(const std::string & packagName,Json::Value & params,const StringList & origVersions);
		const std::string & getName(void) const;
	protected:
		virtual void internalRun(std::string url) = 0;
		void scanValue(const std::string & value);
		StringList makeUniq(StringList & lst);
	protected:
		StringList versions;
		RE2 * regexp;
		std::string packageName;
	private:
		std::string name;
};

}

#endif //HL_CRAWLER_HPP
