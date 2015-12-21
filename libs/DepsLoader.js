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
	for (var i in packageList)
	{
		var p = new PackageBuilder(prefix,userConfig,packageList[i]);
		this.packages[p.pack.name] = p;
		if (this.presentOnSystem(p))
			p.pack.present = 'override-system';
		if (p.isInstalled())
			p.pack.present = 'reinstall';
		this.loadDeps(p);
	}
	
	//sched
	this.buildSched();
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.needLoadDep = function(dep)
{
	if (dep.indexOf('?') == -1)
	{
		var regexp = new RegExp("([0-9a-zA-Z_/-]+)( .+)?");
		var ret = regexp.exec(dep);
		if (ret == null)
			throw "Invalid format";
		return {name:ret[1],use:null};
	} else {
		var regexp = new RegExp("([a-zA-Z0-9-_]+)\\? ([0-9a-zA-Z_/-]+)( .+)?");
		var ret = regexp.exec(dep);
		if (ret == null)
			throw "Invalid format";
		return {name:ret[2],use:ret[1]};
	}
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.loadDeps = function(pack)
{
	this.sched.push(pack.pack.name);
	if (pack.pack.deps != undefined)
	{
		for (var j in pack.pack.deps)
		{
			var dep = this.needLoadDep(pack.pack.deps[j])
			
			if (dep.use == null || pack.hasUseFlags(dep.use))
			{
				var p = new PackageBuilder(this.prefix,this.userConfig,dep.name);
				if (this.packages[p.pack.name] != undefined) {
					console.error(pack.pack.name + " already provided by host, not installed as deps");
				} else if (p.isInstalled()) {
					p.pack.present = 'already-installed';
					this.packages[p.pack.name] = p;
				} else if (this.presentOnSystem(p)) {
					p.pack.present = 'use-host';
					this.packages[p.pack.name] = p;
				} else if (this.packages[p.pack.name] == undefined) {
					this.packages[p.pack.name] = p;
					this.loadDeps(p);
				} else {
					console.error(pack.pack.name + " already provided by host, not installed as deps");
				}
			}
		}
	}
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.buildSched = function()
{
	var s = this.sched.reverse();
	var sched = [];
	for (var i in s)
		if (sched.indexOf(s[i]) == -1)
			sched.push(s[i]);
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
		if (p.pack.present != undefined)
			console.log(this.sched[i]+"-"+p.getVersion()+" ["+p.pack.present+"]");
		else
			console.log(this.sched[i]+"-"+p.getVersion());
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
	var h = p.pack.host['debian8'];

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
			var res = child_process.execSync('dpkg -s '+h[i]);
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

module.exports = DepsLoader;
