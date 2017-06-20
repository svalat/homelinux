/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <fstream>
#include <sstream>
#include <regex>
#include <base/Debug.hpp>
#include <base/Helper.hpp>
#include <portability/System.hpp>
#include "VersionMatcher.hpp"
#include "Prefix.hpp"
#include "PackageDef.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
/**
 * Constructor
**/
PackageDef::PackageDef(void)
{
	this->api = 0;
}

/*******************  FUNCTION  *********************/
/**
 * Load the package from given JSON file.
 * @param path Path of the JSON file to load.
**/
void PackageDef::load(const std::string & path)
{
	//load
	Json::Value tmpObj;
	System::loadJson(tmpObj,path);

	//apply
	this->loadJson(tmpObj);
	
	assumeArg (api == HL_JHSON_API,"Invalid API in %1, requires %2 but has %3")
		.arg(path)
		.arg(HL_JHSON_API)
		.arg(api)
		.end();
}

/*******************  FUNCTION  *********************/
/**
 * Load from a JSON tree.
 * @param json JSON tree to use.
**/
void PackageDef::loadJson(const Json::Value & json)
{
	this->api = json.get("api",1).asInt();
	this->name = json.get("name","").asString();
	this->homepage = json.get("homepage","").asString();
	this->inherit = json.get("inherit","").asString();
	Helper::jsonToObj(versions,json["versions"]);
	this->vfetcher.mode = json["vfetcher"].get("mode","").asString();
	this->vfetcher.url = json["vfetcher"].get("url","").asString();
	this->vfetcher.regexp = json["vfetcher"].get("regexp","").asString();
	Helper::jsonToObj(md5,json["md5"]);
	this->subdir = json.get("subdir","").asString();
	Helper::jsonToObj(deps,json["deps"]);
	this->host = json["host"];
	Helper::jsonToObj(configure,json["configure"]);
	Helper::jsonToObj(vspecific,json["vspecific"]);
	Helper::jsonToObj(steps,json["steps"]);
	Helper::jsonToObj(conflicts,json["conflicts"]);
	this->use.fromJson(json["use"]);
	Helper::jsonToObj(warn,json["warn"]);
	this->module = json.get("module","").asString();
	Helper::jsonToObj(scripts,json["scripts"]);
	Helper::jsonToObj(vars,json["vars"]);
	Helper::jsonToObj(flags,json["flags"]);
	Helper::jsonToObj(urls,json["urls"]);
	Helper::jsonToObj(patch,json["patch"]);
	Helper::jsonToObj(slots,json["slots"]);
}

/*******************  FUNCTION  *********************/
/**
 * Save the current package into JSON tree.
 * @param json JSON tree to fill.
**/
void PackageDef::save(Json::Value & json)
{
	json["api"] = api;
	json["name"] = name;
	json["homepage"] = homepage;
	json["inherit"] = inherit;
	Helper::toJson(json["versions"],versions);
	json["vfetcher"]["mode"] = vfetcher.mode;
	json["vfetcher"]["url"] = vfetcher.url;
	json["vfetcher"]["regexp"] = vfetcher.regexp;
	Helper::toJson(json["md5"],md5);
	json["subdir"] = subdir;
	Helper::toJson(json["deps"],deps);
	json["host"] = host;
	Helper::toJson(json["configure"],configure);
	Helper::toJson(json["vspecific"],vspecific);
	Helper::toJson(json["steps"],steps);
	Helper::toJson(json["conflicts"],conflicts);
	this->use.toJson(json["use"]);
	Helper::toJson(json["warn"],warn);
	json["module"] = module;
	Helper::toJson(json["scripts"],scripts);
	Helper::toJson(json["vars"],vars);
	Helper::toJson(json["flags"],flags);
	Helper::toJson(json["urls"],urls);
	Helper::toJson(json["patch"],patch);
	Helper::toJson(json["slots"],slots);
}

/*******************  FUNCTION  *********************/
/**
 * Merge given package onto current one.
 * @param def Package to merge.
**/
void PackageDef::merge(const PackageDef & def)
{
	api = def.api;
	if (def.name.empty() == false)
		name = def.name;
	if (def.homepage.empty() == false)
		homepage = def.homepage;
	if (def.inherit.empty() == false)
		inherit = def.inherit;
	if (def.versions.empty() == false)
		versions = def.versions;
	if (def.vfetcher.mode.empty() == false)
		vfetcher.mode = def.vfetcher.mode;
	if (def.vfetcher.url.empty() == false)
		vfetcher.url = def.vfetcher.url;
	if (def.vfetcher.regexp.empty() == false)
		vfetcher.regexp = def.vfetcher.regexp;
	Helper::merge(md5,def.md5);
	if (def.subdir.empty() == false)
		subdir = def.subdir;
	Helper::merge(deps,def.deps);
	//@TODO make better merge
	host = def.host;
	Helper::merge(configure,def.configure,false);
	for (auto & it : def.vspecific)
		vspecific[it.first] = it.second;
	Helper::merge(steps,def.steps,true);
	Helper::merge(conflicts,def.conflicts);
	this->use.merge(def.use);
	Helper::merge(warn,def.warn);
	if (def.module.empty() == false)
		module = def.module;
	Helper::merge(scripts,def.scripts);
	Helper::merge(vars,def.vars);
	Helper::merge(flags,def.flags,false);
	Helper::merge(urls,def.urls);
	Helper::merge(patch,def.patch);
	Helper::merge(slots,def.slots);
}

/*******************  FUNCTION  *********************/
/**
 * Save the package in given path using JSON format.
 * @param path Path to be used.
**/
void PackageDef::save(const std::string & path)
{
	//open file
	std::ofstream out(path);
	assumeArg(out.fail() == false,"Fail to open file %1 to dump config : %2").arg(path).argStrErrno().end();

	//write	
	save(out);

	//close
	out.close();
}

/*******************  FUNCTION  *********************/
/**
 * Dump package into JSON ofrmat in ostream. (for unit tests)
 * @param out Define the output stream to be used.
**/
void PackageDef::save(std::ostream & out)
{
	//rebuild json
	Json::Value json;
	
	//convert
	save(json);
	
	//dump
	out << json;
}

/*******************  FUNCTION  *********************/
/**
 * Select the newer version in the list (considered already sorted)
**/
std::string PackageDef::getVersion(void) const
{
	return versions.front();
}

/*******************  FUNCTION  *********************/
/**
 * Get the current slot considering the current best version available
 * it the list (constidered already sorted)
**/
std::string PackageDef::getSlot(void) const
{
	return VersionMatcher::getSlot(slots,getVersion());
}

/*******************  FUNCTION  *********************/
/** 
 * Sort versions in decresing order so last version is first.
**/
void PackageDef::sortVersions(void)
{
	VersionMatcher::sortList(versions);
}

/*******************  FUNCTION  *********************/
/**
 * Return the N first versions (condiser already ordered)
 * @param cnt Number of element to return
**/
std::string PackageDef::getNVersions(int cnt) const
{
	//vars
	StringList tmp;
	
	//loop
	for (auto & v : versions)
	{
		tmp.push_back(v);
		if (--cnt == 0)
			break;
	}
	
	//return
	return Helper::join(tmp,' ');
}

/*******************  FUNCTION  *********************/
/**
 * Return the name to be used for stow installation
 * This correspond to the package name (full) plus
 * the slot with `:` separator.
**/
std::string PackageDef::getSlotName(void) const
{
	return this->name + ":" + this->getSlot();
}

/*******************  FUNCTION  *********************/
/**
 * Return the short name of package. For example, `hl/app-shells/bash`
 * become `bash`.
**/
std::string PackageDef::getShortName(void) const
{
	return Helper::last(name,'/');
}

/*******************  FUNCTION  *********************/
/**
 * Used for stow directories, it is the slot name replacing some charts
 * by underscore. (mostly `/` and `:`).
**/
std::string PackageDef::getStowName(void) const
{
	//build filename
	std::string name = getSlotName();

	//replace
	for (auto & c : name)
		if (c == '/' || c == ':')
			c = '_';
	
	//ok ret
	return name;
}

/*******************  FUNCTION  *********************/
/**
 * Compute short vestion which keep only the first two diggit.
 * This is usefull into URLs because many packages use this shorten address
 * into subdirectories to store the achives.
**/
std::string PackageDef::getShortVersion(void) const
{
	//get
	std::string version = getVersion();

	//split
	StringList lst;
	int cnt = 0;
	Helper::split(version,'.',[&lst,&cnt](const std::string & value) {
		if (cnt++ < 2)
			lst.push_back(value);
	});

	return Helper::join(lst,'.');
}

/*******************  FUNCTION  *********************/
/**
 * Return the prefix in which to install the package.
 * Caution, this is not necerssarly the homelinux prefix
 * as it could be completed by module paths if module
 * is enable by this package.
**/
std::string PackageDef::getRealPrefix(const std::string & prefix,bool stow) const
{
	if (module.empty())
	{
		if (stow && getShortName() != "stow")
			return prefix+"/stow/"+getSlotName();
		else 
			return prefix;
	} else {
		return prefix+"MOdules/installed/"+module;
	}
}

/*******************  FUNCTION  *********************/
/**
 * Json script to write on when the package is installed
 * or to check if it is installed.
 * @param prefix Prefix to which we add the internal path.
**/
std::string PackageDef::getPackInstalled(const std::string & prefix) const
{
	//build filename
	std::string name = getStowName();

	//build path
	std::string path = prefix+"/homelinux/install-db/"+name+".json";

	//ok ret
	return path;
}

/*******************  FUNCTION  *********************/
/**
 * Generate build option based on `configure` entries of package
 * depending on use flags enabling
**/
std::string PackageDef::getBuildOptions(void) const
{
	//vars
	StringList lst;

	//loop on all entries
	for (auto & criteria : this->configure)
	{
		//get status
		UseFlagState state = use.getApplyStatusWithAnd(criteria.first);

		//we ignore AUTO
		if (state != FLAG_AUTO)
		{
			//status
			bool status = (state == FLAG_ENABLED);

			//loop on all childs
			for (auto child : criteria.second)
			{

				//replace $enable, $disable...
				Helper::replaceInPlace(child, "$enable", status ? "enable":"disable");
				Helper::replaceInPlace(child, "$disable", status ? "disable":"enable");
				Helper::replaceInPlace(child, "$with", status ? "with":"without");
				Helper::replaceInPlace(child, "$without", status ? "without":"with");
				Helper::replaceInPlace(child, "$yes", status ? "yes":"no");
				Helper::replaceInPlace(child, "$no", status ? "no":"yes");
				Helper::replaceInPlace(child, "$ON", status ? "ON":"OFF");
				Helper::replaceInPlace(child, "$OFF", status ? "OFF":"ON");
				Helper::replaceInPlace(child, "-$no-", status ? "-":"-no-");//for QT

				//we protect and push
				lst.push_back("\\\""+child+"\\\"");
			}
		}
	}

	//join
	return Helper::join(lst,' ');
}

/*******************  FUNCTION  *********************/
/**
 * Generate the install script.
 * @param parallelInstall Enable generation for parallel installation of
 * packages. This requires some minot tricks in the generated script.
 * @param prefix Prefix in which to install
**/
void PackageDef::genScript(std::ostream & out,const Prefix & prefix,bool parallelInstall)
{
	//checking
	assumeArg(this->urls.empty() == false,"Fail to get URLS for package %1").arg(getSlotName()).end();

	//vars
	const Config & userConfig = prefix.getUserConfig();

	//setup bash
	out << "#!/bin/bash" << std::endl;

	//setup variables
	out << "HL_TEMP=\"" << userConfig.temp << "\"" << std::endl;
	out << "HL_PACKAGE=\"" << prefix.getFilePath("/homelinux/packages/") << "\"" << std::endl;
	out << "HL_PREFIX=\"" << prefix.getPrefix() +"\"" << std::endl;
	out << "HL_HOMECACHE=\"" << (userConfig.homecache?"true":"false") << "\"" << std::endl;

	//compiler flags
	out << std::endl << "#Compiler flags" << std::endl;
	const char * cstCplFlags[] = { "MAKEOPTS", "CFLAGS", "CXXFLAGS", "FFLAGS", "LDFLAGS" };
	for (auto flag : cstCplFlags)
		out << "HL_" << flag << "=\"" << Helper::join(this->flags[flag],' ') << "\"" << std::endl;

	//pack infos
	out << std::endl << "#Pack infos" << std::endl;
	out << "NAME=\"" << this->name << "\"" << std::endl;
	out << "SHORT_NAME=\"" << this->getShortName() << "\"" << std::endl;
	out << "VERSION=\"" << this->getVersion() << "\"" << std::endl;
	out << "SVERSION=\"" << this->getShortVersion() << "\"" << std::endl;
	out << "URLS=\"" << Helper::join(this->urls,' ') << "\"" << std::endl;
	out << "URLS=\"" << md5[getVersion()] << "\"" << std::endl;
	out << "SUBDIR=\"" << subdir << "\"" << std::endl;
	out << "SLOT=\"" << getSlot() << "\"" << std::endl;
	out << "PREFIX=\"" << getRealPrefix(prefix.getPrefix(),prefix.getConfig().useGnuStow) << "\"" << std::endl;
	out << "BUILD_OPTIONS=\"" << getBuildOptions() << "\"" << std::endl;
	out << "PATCHES=\"" << Helper::join(patch,' ') << "\"" << std::endl;
	out << "USE=\"" << use.toString() << "\"" << std::endl;
	out << "MODULE=\"" << module << "\"" << std::endl;
	if (prefix.getConfig().useGnuStow)
		out << "STOW_NAME=\"" << getStowName() << "\"" << std::endl;
	else
		out << "STOW_NAME=\"\"" << std::endl;
	
	//to mark install and keep track
	out << std::endl << "#to mark install and keep track" << std::endl;
	out << "PACK_INSTALLED=\"" << getPackInstalled(prefix.getFilePath("")) << "\"" << std::endl;

	//dump package
	std::stringstream tmp;
	this->save(tmp);
	std::string buf = tmp.str();
	Helper::replaceInPlace(buf,"\\","\\\\");
	Helper::replaceInPlace(buf,"\"","\\\"");
	Helper::replaceInPlace(buf,"$","\\$");
	out << "PACK_JSON=\"" << buf << "\"" << std::endl;

	//load scripts
	out << std::endl << "#load scripts" << std::endl;
	for (auto & script : scripts)
		out << "source \"" << prefix.getFilePath("/homelinux/packages/"+script) << "\"" << std::endl;
	
	//extra vars
	out << std::endl << "#extra vars" << std::endl;
	out << "function hl_pack_extra_vars()" << std::endl;
	out << "{" << std::endl;
	for (auto & var : vars)
		out << "\t" << var.first << "=\"" << var.second << "\"" << std::endl;
	out << "}" << std::endl;

	//build steps
	out << std::endl << "#package steps functions" << std::endl;
	for (auto & step : steps)
	{
		out << "function hl_pack_" << step.first << "()" << std::endl;
		out << "{" << std::endl;
			out << "\tinfo '" << step.first << "'" << std::endl;
			if (step.second.empty())
			{
				out << "\techo 'Nothing to do'" << std::endl;
			} else {
				for (auto & it : step.second)
					if (it[0] == '@')
						out << "\thl_pack_" << it.substr(1) << std::endl;
					else
						out << "\t" << it << std::endl;
			}
		out << "}" << std::endl;
	}

	//parallel install or not
	out << std::endl << "#Main calls to run" << std::endl;
	if (parallelInstall)
	{
		//for parallel install, the caller say which step to call via args
		out << "set -e" << std::endl;
		out << "setup_vars" << std::endl;
		out << "eval \"$@\"" << std::endl;
	} else {
		out << "set -e" << std::endl;
		out << "setup_vars" << std::endl;
		out << "hl_start" << std::endl;
		out << "hl_pack_main" << std::endl;
		out << "hl_stop" << std::endl;
	}
}

}
