var PackageBuilder = require('./PackageBuilder');

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
			if (this.packages[p.pack.name] == undefined)
			{
				this.packages[p.pack.name] = p;
				this.loadDeps(p);
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

module.exports = DepsLoader;