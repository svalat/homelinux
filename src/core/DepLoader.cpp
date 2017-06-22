/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//std
#include <cassert>
//extern
#include <re2/re2.h>
//internal
#include "HostPkgChecker.hpp"
#include "DepLoader.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
DepLoader::DepLoader(Prefix * prefix)
{
	assert(prefix != NULL);
	this->prefix = prefix;
}

/*******************  FUNCTION  *********************/
DepLoader::~DepLoader(void)
{
	for (auto & it : packages)
		delete it.second;
	packages.clear();
	sched.clear();
}

/*******************  FUNCTION  *********************/
/**
 * Lad list of packages into workspace
 * @param packageList An array of string giving the list of packages
 * to install
**/
void DepLoader::loadRequest(const StringList & packageList)
{
	//clear
	for (auto package : packageList)
	{
		//prepare
		Helper::replaceInPlace(package,":"," :");
		Helper::replaceInPlace(package,"<"," <");
		Helper::replaceInPlace(package,">"," >");
		Helper::replaceInPlace(package,"="," =");
		Helper::replaceInPlace(package,"!"," !");
		Helper::replaceInPlace(package,"~"," ~");
		Helper::replaceInPlace(package," > ="," >=");
		Helper::replaceInPlace(package," < ="," <=");

		//we can load
		this->root.push_back(loadPackage(package,NULL,true));
	}

	//apply vspecific recursivly
	this->applyVSpecific();

	//check use flags
	//@TODO

	//check installed
	this->checkStatus();

	//sched
	this->buildSched();
}

/*******************  FUNCTION  *********************/
/**
 * Apply the vspecific entries from the packages depending on the 
 * vesion which is selected. This function apply on the root package
 * list requested by the user and recursivly call the other one
 * for the deps.
**/
void DepLoader::applyVSpecific(void)
{
	for (auto pack : root)
		applyVSpecific(pack);
}

/*******************  FUNCTION  *********************/
/**
 * Same than applyVSpecific() but to be apply recursivly on the
 * deps of given package.
 * @param pack Apply on package p and then do recursive call on
 * its dependencies.
**/
void DepLoader::applyVSpecific(DepPackage * pack)
{
	//if null
	if (pack == NULL)
		return;

	//loop detection
	pack->infos.loopChecker++;
	assumeArg (pack->infos.loopChecker < 100
		,"Seems to encounder infinit dep loop in package %1")
		.arg(pack->def.getSlotName())
		.end();
	
	//TODO take care of reapplying the wall tree if introduce new deps
	checkUseFlagHints(pack);
	applyVersionHints(pack);
	selectVSpecific(pack);

	//load deps
	loadPackageDeps(pack);

	//loop on deps
	for (auto & it : pack->infos.deps)
		applyVSpecific(it.second);
	
	//loop
	pack->infos.loopChecker--;
}

/*******************  FUNCTION  *********************/
/**
 * Load the requested package into the memory and load its deps.
 * This function use a cache and do not load again a package
 * already loaded. The cache is indexed by name and slot to ensure
 * possibility to install multiple instances of the package with 
 * different slots.
 * @param request THe quest string from user. It can be a simple
 * package name or up the the complete format : "dev-vcs/git[+debug] :3 =3.5"
 * @param parent The package of the package to attach of null if none (root package).
 * @param force Define if we need to force the install of this package if
 * it is already install of provided by the host system.
**/
DepPackage * DepLoader::loadPackage(const std::string & request,DepPackage * parent,bool force)
{
	//vars
	bool needLoadDeps = true;
	PackageRequest infos(request,parent);

	//check if need to load
	if (parent != NULL && infos.use.empty() && parent->def.use.getApplyStatusWithAnd(infos.use) != FLAG_ENABLED)
		return NULL;

	//load package
	DepPackage * p = new DepPackage;
	prefix->loadPackage(p->def,infos.name);

	//apply version rules
	applyVersionRules(p,parent,infos);

	//check if already loader (take in account selected slot)
	std::string slotName = p->def.getSlotName();
	if (packages.find(slotName) != packages.end())
	{
		//ok don't need to load deps
		needLoadDeps = false;

		//replace by current loaded one
		delete p;
		p = packages[slotName];

		//apply rules
		applyVersionRules(p,parent,infos);

		//if slot has changed to to version selection
		if (p->def.getSlotName() != slotName)
		{
			packages[p->def.getSlotName()] = p;
			packages.erase(slotName);
		}
	} else {
		packages[slotName] = p;
	}

	//force
	if (force)
		p->infos.force = true;

	//load deps
	if (needLoadDeps || force)
		this->loadPackageDeps(p);
	
	//ok return
	return p;
}

/*******************  FUNCTION  *********************/
/**
 * Apply the version requirement from parent on the given package.
 * @param pack Define the package on which to filter the version to select.
 * @param infos Rules to apply
**/
void DepLoader::applyVersionRules(DepPackage * pack,DepPackage * parent,PackageRequest & infos)
{
	//null
	if (pack == NULL)
		return;

	//replace #useflag with parent status
	if (infos.iuse.empty() == false && parent != NULL)
		infos.iuse = replaceParentUseFlags(infos.iuse,parent);

	//push and apply
	std::string parentName = "none";
	if (parent != NULL)
		parentName = parent->def.getSlotName();
	pack->hints[parentName] = infos;

	//apply
	checkUseFlagHints(pack);
	applyVersionHints(pack);
}

/*******************  FUNCTION  *********************/
/**
 * Looop on package dep list of the given package and then
 * load them into the list of loaded packages.
 * Also register them into the parent as child packages.
 * @param p the parent package to consider.
**/
void DepLoader::loadPackageDeps(DepPackage * pack)
{
	//trivial
	if (pack == NULL)
		return;

	//loop on all
	for (auto & dep : pack->def.deps)
	{
		if (pack->infos.deps.find(dep) == pack->infos.deps.end())
			pack->infos.deps[dep] = loadPackage(dep,pack,false);
	}
}

/*******************  FUNCTION  *********************/
void DepLoader::checkStatus(void)
{
	HostPkgChecker host(prefix->getUserConfig().host);

	//loop on all
	for (auto & it : packages)
	{
		//extract
		DepPackage & pack = *(it.second);

		//check status
		if(pack.infos.force)
		{
			if (host.presentOnSystem(pack.def.host))
				pack.infos.present = "override-system";
			else if (prefix->isInstalled(pack.def))
				pack.infos.present = "reinstall";
		} else if (pack.infos.present.empty()) {
			if (prefix->isInstalled(pack.def))
				pack.infos.present = "already-installed";
			else if (host.presentOnSystem(pack.def.host))
				pack.infos.present = "use-host";		
		}
	}
}

/*******************  FUNCTION  *********************/
/**
 * Build install scheduling by looping into the tree. The packages with
 * less deps will be installed first then the others.
 * This function only setup this.sched variable.
**/
void DepLoader::buildSched(void)
{
	//clear
	sched.clear();

	//build list
	for (auto & pack : root)
		buildSchedChild(pack);

	//reverse
	sched.reverse();

	//make uniq
	StringList endSched;
	for (auto pack : sched)
		if (Helper::contain(endSched,pack) == false)
			endSched.push_back(pack);

	//overrite	
	sched.clear();
	sched = endSched;
}

/*******************  FUNCTION  *********************/
/**
 * Same than buildSched but using reursive call to walk in the tree.
**/
void DepLoader::buildSchedChild(DepPackage * pack)
{
	//NULL case
	if (pack == NULL)
		return;
	
	//selec ontly empty, override and reinstall
	const std::string & present = pack->infos.present;
	if (present.empty() || present == "override-system" || present == "reinstall")
	{
		//push
		sched.push_back(pack->def.getSlotName());

		//apply deps
		for (auto &it : pack->infos.deps)
			buildSchedChild(it.second);
	}
}

/*******************  FUNCTION  *********************/
void DepLoader::checkUseFlagHints(DepPackage * pack)
{
	//vars
	std::string err;

	//loop on all parent packages hints
	for (auto & hint : pack->hints)
	{
		//look on all slides
		Helper::split(hint.second.iuse,',',[&err,this,pack](const std::string & flag) {
			std::string full = flag;
			if (full[0] != '+' && full[0] != '-')
				full = "+"+full;
			UseFlagState state = pack->def.use.getApplyStatusWithAnd(full);
			if (state != FLAG_ENABLED)
				err += full + " (" + pack->def.getSlotName() + ") ";
		});
	}

	if (err.empty() == false)
		HL_FATAL_ARG("Package %1 has some missing use flags tp match dependencies requirement : %2")
			.arg(err)
			.end();
}

/*******************  FUNCTION  *********************/
void DepLoader::applyVersionHints(DepPackage * pack)
{
	std::string before = Helper::join(pack->def.versions,' ');

	//loop on all hints
	for (auto & hint : pack->hints)
	{
		VersionMatcher match(hint.second.version);
		pack->def.versions = match.filterList(pack->def.versions,pack->def.slots);
	}

	//if empty
	if (pack->def.versions.empty())
	{
		HL_ERROR_ARG("Version filter is too strict for package %1").arg(pack->def.name).end();
		HL_ERROR_ARG("  - Previous version list is : %1").arg(before).end();
		for (auto & hint : pack->hints)
			HL_ERROR_ARG("  - Hint from %1 : %2").arg(hint.first).arg(hint.second.version).end();
		exit(1);
	}
}

/*******************  FUNCTION  *********************/
void DepLoader::selectVSpecific(DepPackage * pack)
{
	//get versions
	std::string version = pack->def.getVersion();

	//loop on all specific
	StringList toRemove;
	for (auto & it : pack->def.vspecific)
	{
		VersionMatcher match(it.first);
		if (match.match(version,pack->def.slots))
		{
			PackageDef def;
			def.loadJson(it.second);
			pack->def.merge(def);
			toRemove.push_back(it.first);
		}
	}

	//remove applied
	for (auto & it : toRemove)
		pack->def.vspecific.erase(it);
}

/*******************  FUNCTION  *********************/
std::string DepLoader::replaceParentUseFlags(const std::string uses,const DepPackage * parent)
{
	//vars
	StringList out;

	//loop on all
	Helper::split(uses,' ',[&out,parent](const std::string & use){
		if (use[0] == '#')
		{
			std::string name = use.substr(1);
			UseFlagState state = parent->def.use.getStatus(name);
			if (state == FLAG_ENABLED)
				out.push_back("+"+name);
			else if (state == FLAG_DISABLED)
				out.push_back("-"+name);
		} else {
			out.push_back(use);
		}
	});

	return Helper::join(out,' ');
}

/*******************  FUNCTION  *********************/
DepLoaderInfos::DepLoaderInfos(void)
{
	this->loopChecker = 0;
	this->force = false;
}

}
