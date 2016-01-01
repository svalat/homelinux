/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * The dep loader class is responsible to load all the packages
 * needed to build the requested list of root packages.
 * It build a tree which is cut by checking the available packages
 * on the host system, then build job list to build all the packages.
 * It also apply some version filtering depending on the requirement
 * of parent packages.
**/

/*********************** REQURE *********************/
var PackageBuilder = require('./PackageBuilder');
var HostPkgChecker = require('./HostPkgChecker');
var fs = require('fs');

/*********************  CLASS  **********************/
/**
 * Build a package dep workspace
 * @param prefix Prefix to use to build the packages (Prefix object).
 * @param userConfig The user config object to use.
 * @param packageList An array of string giving the list of packages
 * to install
**/
function DepsLoader(prefix,userConfig,packageList)
{
	//load packages
	this.prefix = prefix;
	this.userConfig = userConfig;
	this.packages = {};
	this.sched = [];
	
	//lost host
	var fname = prefix.getFile('homelinux/packages/hosts/'+userConfig.config.host+".json");
	var content = fs.readFileSync(fname);
	this.hostsRefs = JSON.parse(content);
	
	//host checker
	this.hostPkgChecker = new HostPkgChecker(userConfig);
	
	//load packages
	this.root = [];
	this.packages = {};
	for (var i in packageList)
	{
		var req = packageList[i]
			.replace(':',' :')
			.replace('<',' <')
			.replace('<=',' <=')
			.replace('>',' >')
			.replace('>=',' >=')
			.replace('=',' =')
			.replace('!',' !')
			.replace('~',' ~');
		this.root.push(this.loadPackage(req,null,true));
	}
	
	//apply vspecifiv recursivly
	this.applyVSpecific();

	//sched
	this.buildSched();
}

/*******************  FUNCTION  *********************/
/**
 * Apply the vspecific entries from the packages depending on the 
 * vesion which is selected. This function apply on the root package
 * list requested by the user and recursivly call the other one
 * for the deps.
**/
DepsLoader.prototype.applyVSpecific = function()
{
	//console.log("Start specific");
	for (var i in this.root)
	{
		if (this.root[i] != null)
			this.applyVSpecificChild(this.root[i]);
	}
};

/*******************  FUNCTION  *********************/
/**
 * Same than applyVSpecific() but to be apply recursivly on the
 * deps of given package.
 * @parap p Apply on package p and then do recursive call on
 * its dependencies.
**/
DepsLoader.prototype.applyVSpecificChild = function(p)
{
	if (p == null)
		return;
	
	//TODO take care of reapplying the wall tree if introduce new deps
	p.checkUseFlagHints();
	p.applyVersionHints();
	p.selectVSpecific();
	this.loadPackageDeps(p);
	if (p.pdeps != undefined)
		for (var i in p.pdeps)
		{
			this.applyVSpecificChild(p.pdeps[i]);
		}
};

/*******************  FUNCTION  *********************/
/**
 * Apply the version requirement from parent on the given package.
 * @param pack Define the package on which to filter the version to select.
 * @param parent The parent package to know which verion rules to apply.
 * @param infos Rules to apply
**/
DepsLoader.prototype.applyVersionRules = function(pack,parent,infos)
{
	//replace #useflag with parent status
	if (infos.iuse != undefined && parent != undefined)
		infos.iuse = parent.replaceParentUseFlags(infos.iuse);
	
	//add to list
	if (pack.hints == undefined)
		pack.hints = {};
	
	//push and apply
	if (parent == null)
		parent = 'none';
	else
		parent = parent.getNameSlot();
	pack.hints[parent] = infos;
	
	//apply
	pack.checkUseFlagHints();
	pack.applyVersionHints();
};

/*******************  FUNCTION  *********************/
/**
 * Looop on package dep list of the given package and then
 * load them into the list of loaded packages.
 * Also register them into the parent as child packages.
 * @param p the parent package to consider.
**/
DepsLoader.prototype.loadPackageDeps = function(p)
{
	var hasNewDeps = false;

	if (p.pdeps == undefined)
		p.pdeps = {};

	var deps = p.getProperty('deps');
	for (var i in deps)
	{
		if (p.pdeps[deps[i]] == undefined || p.pdeps[deps[i]] == null)
		{
			p.pdeps[deps[i]] = this.loadPackage(deps[i],p,false);
			if (p.pdeps[deps[i]] != null)
				hasNewDeps = true;
		}
	}
			
	return hasNewDeps;
};

/*******************  FUNCTION  *********************/
/**
 * Load the requested package into the memory and load its deps.
 * This function use a cache and do not load again a package
 * already loaded. The cache is indexed by name and slot to ensure
 * possibility to install multiple instances of the package with 
 * different slots.
 * @param request THe quest string from user. It can be a simple
 * package name or up the the complete format : "dev-vcs/git[+debug] :3 =3.5"
 * @parent The package of the package to attach of null if none (root package).
 * @param force Define if we need to force the install of this package if
 * it is already install of provided by the host system.
**/
DepsLoader.prototype.loadPackage = function(request,parent,force)
{
	var needLoadDeps = true;
	var infos = this.parseRequestString(request);
	var name = infos.name;
	infos.parent = parent;
	
	//if useflag say to not load, skip it
	if (parent != null && infos.use != null && !parent.hasUseFlags(infos.use,false))
		return null;
	
	//load package if need
	var p = new PackageBuilder(this.prefix,this.userConfig,name);
	
	//apply version rules
	this.applyVersionRules(p,parent,infos);
	
	//check if already loaded (take in account selected slot)
	if (this.packages[p.getNameSlot()] != undefined)
	{
		needLoadDeps = false;
		//take the already loaded one and reapply version rules on it
		p = this.packages[p.getNameSlot()];
		this.applyVersionRules(p,parent,infos);
	}
	
	//check status
	if (force == true)
	{
		if (this.hostPkgChecker.presentOnSystem(p))
			p.pack.present = 'override-system';
		if (p.isInstalled())
			p.pack.present = 'reinstall';
	} else if (p.pack.present == undefined) {
		if (this.prefix.hasPackageInstalled(p))
			p.pack.present = 'already-installed';
		else if (this.hostPkgChecker.presentOnSystem(p))
			p.pack.present = 'use-host';
		else
			p.pack.present = null;
	}
	
	//register into loaded DB
	this.packages[p.getNameSlot()] = p;
	
	//load deps
	if (needLoadDeps)
		this.loadPackageDeps(p);
	
	return p;
};

/*******************  FUNCTION  *********************/
/**
 * Parse the question string and then convert it into a more usable
 * struct for the rest of the app.
 * @param dep Define the string to parse. The format can be on of those
 * examples :
 *    - git : Only simple name
 *    - dev-vcs/git : Name with source (can be gentoo/, urls/....)
 *    - dev-vcs/git[+debug,-gzip] : Force some useflags on the package
 *    - dev-vcs/git <3.5 >2.5 : version hints
 *    - dev-vcs/git =3.5.6 : Force specific vesrion
 *    - dev-vcs/git :3 : Define the slot to be used.
 *    - dev-vcs/git ~3.[0-9]+ : with regexp
 *
 * You can see doc directory to get more infos on the format.
**/
DepsLoader.prototype.parseRequestString = function(dep)
{
	if (dep.indexOf('?') == -1)
	{
		var regexp = new RegExp("([0-9a-zA-Z+_/&-]+)(\\[[0-9A-Za-z#_+,-]+\\])?( .+)?");
		var ret = regexp.exec(dep);
		if (ret == null)
			throw "Invalid format "+dep;
		return {name:ret[1],use:null,iuse:ret[2],version:ret[3]};
	} else {
		var regexp = new RegExp("([a-zA-Z0-9-&_+]+)\\? ([0-9a-zA-Z_/-]+)(\\[[0-9A-Za-z#_+,-]+\\])?( .+)?");
		var ret = regexp.exec(dep);
		if (ret == null)
			throw "Invalid format "+dep;
		return {name:ret[2],use:ret[1],iuse:ret[3],version:ret[4]};
	}
};

/*******************  FUNCTION  *********************/
/**
 * Build install scheduling by looping into the tree. The packages with
 * less deps will be installed first then the others.
 * This function only setup this.sched variable.
**/
DepsLoader.prototype.buildSched = function()
{
	this.sched = [];
	for (var i in this.root)
		this.buildSchedChild(this.root[i]);
	
	//reverse for prio
	var sched = this.sched.reverse();
	
	//make uniq
	var endSched = [];
	for (var i in sched)
		if (endSched.indexOf(sched[i]) == -1)
			endSched.push(sched[i]);
	this.sched = endSched;
};

/*******************  FUNCTION  *********************/
/**
 * Same than buildSched but using reursive call to walk in the tree.
**/
DepsLoader.prototype.buildSchedChild = function(p)
{
	if (p.pack.present == undefined || p.pack.present == null || p.pack.present == 'override-system' || p.pack.present == 'reinstall')
	{
		this.sched.push(p.getNameSlot());
		for (var i in p.pdeps)
			if (p.pdeps[i] != null)
				this.buildSchedChild(p.pdeps[i]);
	}
};

/*******************  FUNCTION  *********************/
/**
 * Generate the inscall script my merging the install script of all
 * the packages from sched list;
 * This script must be forwarded to bash to be usd. This is done
 * by the homelinux command.
**/
DepsLoader.prototype.genScript = function()
{
	var ret = "#!/bin/bash\n\n";
	ret += "HL_TOT_PACK="+this.sched.length+"\n";
	ret += "HL_CUR_PACK=0\n\n";
	for (var i in this.sched)
		ret += this.packages[this.sched[i]].genScript() + "\n\n####################################################\n\n";
	return ret;
};

/*******************  FUNCTION  *********************/
/**
 * Print the list of packages to install and those who are not
 * installed because already installed or provided by the host system.
**/
DepsLoader.prototype.printList = function()
{
	console.log("\n\n----------------------TO INSTALL--------------------------");
	for (var i in this.sched)
	{
		var p = this.packages[this.sched[i]];
		if (p.pack.present != undefined && p.pack.present != null)
			console.log(p.getNameSlot()+"-"+p.getVersion()+" ["+p.pack.present+"] "+p.getUseFlagStatusString());
		else
			console.log(p.getNameSlot()+"-"+p.getVersion()+" "+p.getUseFlagStatusString());
	}
	console.log("-----------------------INSTALLED--------------------------");
	for (var i in this.packages)
		if (this.packages[i].pack.present == 'already-installed')
			console.log(this.packages[i].pack.name);
	console.log("----------------------REUSE HOST--------------------------");
	for (var i in this.packages)
		if (this.packages[i].pack.present == 'use-host')
			console.log(this.packages[i].pack.name);
	console.log("----------------------------------------------------------\n");
};

/*******************  FUNCTION  *********************/
/**
 * Generate the parallel makefile script to build all the requested packages
 * in parallel mode. it mostly call all the childs script function per function
 * and manage extraction of the Make subcommand to keep parallelism handling
 * into the root makefile.
 * @param tmpdir Temporary into which to write the Makefile.
**/
DepsLoader.prototype.genParallelMakefile = function(tmpdir)
{
	var mk = [];
	
	//all
	var all = [];
	for (var i in this.sched)
		all.push(this.sched[i].replace(/[/:]/g,'_'));
	mk.push("all: "+all.join(' '));
	mk.push('');
	
	for (var i in this.sched)
	{
		var step = this.sched[i].replace(/[/:]/g,'_');
		var makenotif = "$PWD/"+step+"hl-is-default-build.notify";
		mk.push(step+":");
		mk.push("	@bash "+step+".sh hl_start");
		mk.push("	@bash "+step+".sh hl_prebuid");
		mk.push("	@bash "+step+".sh hl_build "+makenotif);
		mk.push("	@if test -f "+makenotif+"; then make -C `cat "+makenotif+"` fi");
		mk.push("	@bash "+step+".sh hl_postbuild");
		mk.push("	@bash "+step+".sh hl_finish");
		mk.push("");
	}
	
	//deps
	for (var i in this.sched)
	{
		var step = this.sched[i].replace(/[/:]/g,'_');
		var deps = [];
		var pname = this.sched[i];
		var p = this.packages[pname];
		for (var j in p.pdeps)
			if (p.pdeps[j] != null && p.pdeps[j] != undefined && this.sched.indexOf(p.pdeps[j].getNameSlot()) != -1)
				deps.push(p.pdeps[j].getNameSlot().replace(/[/:]/g,'_'));
		mk.push(step+": "+deps.join(' '));
	}
	mk.push("");
	
	//PHONY
	var deps = [];
	for (var i in this.sched)
	{
		var step = this.sched[i].replace(/[/:]/g,'_');
		deps.push(step);
	}
	mk.push(".PHONY: "+deps.join(' '));
	
	//join
	mk = mk.join('\n');
	
	//if file
	if (tmpdir != undefined)
		fs.writeFileSync(tmpdir+"/Makefile",mk);
	
	return mk;
};

/*******************  FUNCTION  *********************/
/**
 * Gen the paralell script to build all the requested pacjages. Compared to the
 * genScript method we build a script in separate file per package.
 * The scritp permit to call a step by passing arguement from the shell call.
 * @param tmpdir Temporary directory to use to build the packages.
**/
DepsLoader.prototype.genParallelScripts = function(tmpdir)
{
	for (var i in this.sched)
	{
		var step = this.sched[i].replace(/[/:]/g,'_');
		var ret = "#!/bin/bash\n\n";
		ret += "HL_TOT_PACK="+this.sched.length+"\n";
		ret += "HL_CUR_PACK="+i+"\n\n";
		ret += this.packages[this.sched[i]].genScript() + "\n\n####################################################\n\n";
		fs.writeFileSync(tmpdir+"/"+step+".sh",ret);
	}
	return ret;
};

module.exports = DepsLoader;
