var PackageBuilder = require('./PackageBuilder');
var child_process = require('child_process');
var fs = require('fs');

/*********************  CLASS  **********************/
function DepsLoader(prefix,userConfig,packageList)
{
	//load packages
	this.prefix = prefix;
	this.userConfig = userConfig;
	this.packages = {};
	this.sched = [];
	
		
	//lost host
	var fname = prefix.getFile('share/homelinux/packages/hosts/'+userConfig.config.host+".json");
	var content = fs.readFileSync(fname);
	this.hostsRefs = JSON.parse(content);
	
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
DepsLoader.prototype.applyVSpecific = function()
{
	//console.log("Start specific");
	for (var i in this.root)
	{
		if (this.root[i] != null)
			this.applyVSpecificChild(this.root[i]);
	}
}

/*******************  FUNCTION  *********************/
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
}

/*******************  FUNCTION  *********************/
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
}

/*******************  FUNCTION  *********************/
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
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.loadPackage = function(request,parent,force)
{
	var needLoadDeps = true;
	var infos = this.parseRequestString(request);
	var name = infos.name;
	infos.parent = parent;
	
	//if useflag say to not load, skip it
	if (parent != null && infos.use != null && !parent.hasUseFlags(infos.use))
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
		if (this.presentOnSystem(p))
			p.pack.present = 'override-system';
		if (p.isInstalled())
			p.pack.present = 'reinstall';
	} else if (p.pack.present == undefined) {
		if (p.isInstalled())
			p.pack.present = 'already-installed';
		else if (this.presentOnSystem(p))
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
}

/*******************  FUNCTION  *********************/
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
}

/*******************  FUNCTION  *********************/
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
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.buildSchedChild = function(p)
{
	if (p.pack.present == undefined || p.pack.present == null || p.pack.present == 'override-system' || p.pack.present == 'reinstall')
	{
		this.sched.push(p.getNameSlot());
		for (var i in p.pdeps)
			if (p.pdeps[i] != null)
				this.buildSchedChild(p.pdeps[i]);
	}
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.genScript = function()
{
	var ret = "#!/bin/bash\n\n";
	ret += "HL_TOT_PACK="+this.sched.length+"\n";
	ret += "HL_CUR_PACK=0\n\n";
	for (var i in this.sched)
		ret += this.packages[this.sched[i]].genScript() + "\n\n####################################################\n\n";
	return ret;
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.printList = function()
{
	console.log("\n\n----------------------TO INSTALL--------------------------");
	for (var i in this.sched)
	{
		var p = this.packages[this.sched[i]];
		if (p.pack.present != undefined && p.pack.present != null)
			console.log(p.getNameSlot()+"-"+p.getVersion()+" ["+p.pack.present+"]");
		else
			console.log(p.getNameSlot()+"-"+p.getVersion());
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
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.presentOnSystemDefault = function(p)
{
	if (p.pack.host['default'] == undefined)
		return false;
	else
		return p.pack.host['default'];
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.presentOnSystemDebian8 = function(p)
{
	var h = [];
	
	//package one
	if (p.pack.host != undefined)
		h = p.pack.host['debian8'];

	//load from separate file
	if (this.hostsRefs[p.pack.name] != undefined)
		h = this.hostsRefs[p.pack.name];
	
	//not defined, consider not provided
	if (h == undefined)
		return false;
	
	//is not provided
	if (h === false)
		return false;
	
	//console.log("Check ncurses on host : "+h);
	
	//check in list
	for (var i in h)
	{
		try {
			//console.error("Check with dpkg "+h[i]);
			var res = child_process.execSync('dpkg -s '+h[i]+' 2> /dev/null');
		} catch (e) {
			return false;
		}
	}
	return true;
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.presentOnSystem = function(p)
{
	if (this.userConfig.config.host == 'default')
	{
		return this.presentOnSystemDefault(p);
	} else if (this.userConfig.config.host == 'debian8') {
		return this.presentOnSystemDebian8(p);
	} else {
		throw "Unsupposed host system to check deps, please use default in that case !";
	}
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.genParallelMalefile = function(tmpdir)
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
}

/*******************  FUNCTION  *********************/
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
}

module.exports = DepsLoader;
