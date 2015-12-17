/********************  GLOBALS  *********************/
var jso = require('json-override');
var spawn = require('child_process').spawn;

/*********************  CLASS  **********************/
function PackageBuilder(prefix,userConfig,packageName,inherit)
{
	this.prefix = prefix;
	this.userConfig = userConfig;
	this.load(packageName,inherit);
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.load = function(packageName,inherit)
{
	var pack = this.prefix.loadPackage(packageName);
	this.origPack = pack;
	if (inherit == true || inherit == undefined)
		this.pack = this.loadInherit(pack);
	else
		this.pack = pack;
}

/*******************  FUNCTION  *********************/
function mergeEntry(parent,child)
{
	if( child == undefined)
		return parent;
	else
		return child;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.loadInherit = function(pack)
{
	if (pack.inherit == undefined || pack.inherit == '')
	{
		return pack;
	} else {
		//load
		var parent = this.prefix.loadPackage(pack.inherit);
		
		//merge
		for (var i in pack)
		{
			if (i == 'deps')
			{
				parent.deps = pack.deps.concat(parent.deps);
			} else if (i == 'steps') {
				if (parent.steps != undefined && pack.steps != undefined)
					parent.steps = jso(parent.steps,pack.steps);
				else if (pack.steps != undefined)
					parent.steps = pack.steps;
			} else if ( i == 'inherit' ) {
			} else {
				parent[i] = mergeEntry(parent[i],pack[i]);
			}
		}
			
		return this.loadInherit(parent);
	}
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.hasUseFlags = function(value)
{
	if (value == '')
		return true;
	else
		return false;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.applyUseFlags = function(value)
{
	if (value == '' || value[0] != '+')
		return true;
	else
		return false;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.buildOptions = function()
{
	var opts = [];
	for (var i in this.pack.configure)
	{
		if (this.applyUseFlags(i))
		{
			for (var j in this.pack.configure[i])
			{
				opts.push(this.pack.configure[i][j].replace('$enable',this.hasUseFlags(i)?'enable':'disable'))
			}
		}
	}
	
	return opts.join(' ');
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.genScript = function()
{
	//header
	var script = [];
	script.push("#!/bin/bash")
	script.push("source " + this.prefix.getFile("share/homelinux/packages/models/default.sh"));
	
	//setup variables HL
	script.push("HL_TEMP=\""+this.userConfig.config.temp+"\"");
	script.push("HL_PACKAGE=\""+this.prefix.getFile("share/homelinux/packages/")+"\"");
	script.push("HL_MAKEOPTS=\""+this.prefix.config.compiler.MAKEOPTS+"\"");
	
	//setup package variables
	var version = this.getVersion();
	script.push("NAME=\""+this.pack.name+"\"");
	script.push("VERSION=\""+version+"\"");
	script.push("URLS=\""+this.pack.urls.join(' ')+"\"");
	script.push("MD5=\""+this.pack.md5[version] != undefined?this.pack.md5[version]:''+"\"");
	script.push("SUBDIR=\""+this.pack.subdir+"\"");
	script.push("PREFIX=\""+this.prefix.prefix+"\"");
	script.push("BUILD_OPTIONS=\""+this.buildOptions()+"\"");
	
	//gen functions
	for (var i in this.pack.steps)
	{
		script.push("function hl_pack_"+i+"()");
		script.push("{");
		script.push("	info '"+i+"'");
		if (this.pack.steps[i].length == 0)
		{
			script.push("	echo 'Nothing to do'");
		} else {
			for (var j in this.pack.steps[i])
			{
				if (this.pack.steps[i][j][0] == '@')
					script.push("	hl_pack_"+this.pack.steps[i][j].replace('@',''));
				else
					script.push("	"+this.pack.steps[i][j]);
			}
		}
		script.push("}");
	}
	
	//run
	script.push("set -e");
	script.push("start_stop Start $NAME-$VERSION");
	script.push("hl_setup_tmp_dir");
	script.push("hl_pack_main")
	script.push("start_stop Finish $NAME-$VERSION");
	
	return script.join('\n');
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getVersion = function()
{
	return this.pack.versions[this.pack.versions.length-1];
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.printDebug = function()
{
	console.log(this.pack);
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.install = function()
{
	child = spawn('bash');
	
	//child.stdin.setEncoding('utf-8');
	child.stdout.pipe(process.stdout);
	child.stderr.pipe(process.stderr);
	child.stdin.write(this.genScript()+"\n\n\n");
	child.stdin.end();
}

/*******************  FUNCTION  *********************/
module.exports = PackageBuilder;
