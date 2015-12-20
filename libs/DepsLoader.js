var PackageBuilder = require('./PackageBuilder');
var child_process = require('child_process');

/*********************  CLASS  **********************/
function DepsLoader(prefix,userConfig,packageList)
{
	//load packages
	this.prefix = prefix;
	this.userConfig = userConfig;
	this.packages = {};
	this.sched = [];
	for (var i in packageList)
	{
		var p = new PackageBuilder(prefix,userConfig,packageList[i]);
		this.packages[p.pack.name] = p;
		console.log(p.pack.name +" -> " + this.presentOnSystem(p));
		this.loadDeps(p);
	}
	
	this.buildSched();
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype

/*******************  FUNCTION  *********************/
DepsLoader.prototype.loadDeps = function(p)
{
	this.sched.push(p.pack.name);
	if (p.pack.deps != undefined)
	{
		for (var j in p.pack.deps)
		{
			var p = new PackageBuilder(this.prefix,this.userConfig,p.pack.deps[j]);
			console.log(p.pack.name +" -> " + this.presentOnSystem(p));
			if (this.packages[p.pack.name] == undefined && this.presentOnSystem(p) == false)
			{
				this.packages[p.pack.name] = p;
				this.loadDeps(p);
			} else {
				console.error(p.pack.name + " already provided by host, not installed as deps");
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
	for (var i in this.sched)
		console.log(this.sched[i]);
}

/*******************  FUNCTION  *********************/
DepsLoader.prototype.presentOnSystem = function(p)
{
	if (this.userConfig.config.host == 'default')
	{
		if (p.pack.host['default'] == undefined)
			return false;
		else
			return p.pack.host['default'];
	} else if (this.userConfig.config.host == 'debian8') {
		if (p.pack.host['debian8'] == undefined)
		{
			return false;
		} else {
			try {
				console.error("Check with dpkg "+p.pack.host['debian8']);
				var res = child_process.spawnSync('dpkg -s '+p.pack.host['debian8']);
				return true;
			} catch (e) {
				console.log(e);
				return false;
			}
		}
	}
}

module.exports = DepsLoader;
