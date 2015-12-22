/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var jso = require('json-override');
var spawn = require('child_process').spawn;
var fs = require('fs');
var VersionHelper = require('./VersionHelper');

/*********************  CLASS  **********************/
function PackageBuilder(prefix,userConfig,packageName,inherit)
{
	this.prefix = prefix;
	this.userConfig = userConfig;
	this.load(packageName,inherit);
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getShortName = function()
{
	return this.pack.name.split('/').pop();
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
			} else if ( i == 'configure' ) {
				parent[i] = this.mergeSubArrays(parent[i],pack[i]);
			} else if ( i == 'deps' ) {
				parent[i] = this.mergeSubArrays(parent[i],pack[i]);
			} else if ( i == 'useflags' ) {
				if (parent[i] != undefined)
					parent[i] = parent[i].concat(pack[i]);
				else
					parent[i] = pack[i];
				//TODO do proper marge
				parent[i] = parent[i].filter(function(value,index,self) {return self.indexOf(value) === index;});
			} else {
				parent[i] = mergeEntry(parent[i],pack[i]);
			}
		}
			
		return this.loadInherit(parent);
	}
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.selectVSpecific = function()
{
	var version = this.getVersion();
	this.vspecific = [];
	if (this.pack.vspecific != undefined)
	{
		for (var i in this.pack.vspecific)
		{
			if (VersionHelper.filterVersion(this.pack,i,version))
			{
				this.vspecific.push(this.pack.vspecific[i]);
			}
		}
	}
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.mergeSubArrays = function(orig,addition)
{
	if (addition == undefined)
		return orig;

	//copy
	var final = {};
	for (var i in orig)
		final[i] = orig[i].slice(0);//to clone
	
	//merge
	for (var i in addition)
	{
		if (final[i] == undefined)
			final[i] = addition[i];
		else
			final[i] = final[i].concat(addition[i]);
	}
	
	return final;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getProperty = function(name)
{
	var ref = this.pack[name];
	var mode = 'replace';
	
	//select mode
	switch(name)
	{
		case 'deps':
		case 'patch':
		case 'useflags':
			mode = 'merge-arrays';
			break;
		case 'configure':
			mode = 'merge-sub-arrays';
			break;
		default:
			mode = 'replace';
			break;
	}
	
	//apply
	var final = ref;
	for (var i in this.vspecific)
	{
		var vspecific = this.vspecific[i];
		if (vspecific[name])
		{
			switch(mode)
			{
				case 'replace':
					final = vspecific[name];
					break;
				case 'merge-arrays':
					if (final == undefined)
						final = [];
					final = final.concat(vspecific[name]);
					break;
				case 'merge-sub-arrays':
					final = this.mergeSubArrays(final,vspecific[name]);
					break;
				default:
					throw 'Invalid mode '+mode;
			}
		}
	}
	
	return final;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.hasUseFlags = function(value)
{
	//if not use flags defined, default is enable
	if (value == '')
		return true;
	
	//if value contain &
	if (value.indexOf('&') != -1)
	{
		var lst = value.split("&");
		for (var i in lst)
			if (!this.hasUseFlags(lst[i].trim()))
				return false;
		return true;
	}
	
	//check +-
	var invert = false;
	if (value[0] == '-')
		invert = true;
	value = value.replace(/[-+]/g,'');

	//get global use flag
	var local = this.getProperty('useflags');
	var global = this.prefix.config.useflags[""];
	var pack = this.prefix.config.useflags[this.pack.name];
	
	//apply local
	var status = false;
	if (local != undefined && (local.indexOf(value) != -1 || local.indexOf('+'+value) != -1))
		status = true;
	
	//apply global
	if (global != undefined && (global.indexOf(value) != -1 || global.indexOf('+'+value) != -1))
		status = true;
	else if (global != undefined && global.indexOf('-'+value) != -1)
		status = false;
	
	//apply package
	if (pack != undefined && (pack.indexOf(value) != -1 || pack.indexOf('+'+value) != -1))
		status = true;
	else if (pack != undefined && pack.indexOf('-'+value) != -1)
		status = false;

	if (invert)
		return !status;
	else
		return status;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.applyUseFlags = function(value)
{
	if (value == '')
	{
		return true;
	} else if (value[0] == '+') {
		return this.hasUseFlags(value.substring(1,value.length));
	} else if (value[0] == '-') {
		return !this.hasUseFlags(value.substring(1,value.length));
	} else {
		 return true;
	}
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.buildOptions = function()
{
	var opts = [];
	var configure = this.getProperty('configure');
	for (var i in configure)
		if (this.applyUseFlags(i))
			for (var j in configure[i])
			{
				var config = configure[i][j]
					.replace('$enable',this.hasUseFlags(i)?'enable':'disable')
					.replace('$disable',this.hasUseFlags(i)?'disable':'enable')
					.replace('$without',this.hasUseFlags(i)?'without':'with')
					.replace('$with',this.hasUseFlags(i)?'with':'without')
					.replace('$ON',this.hasUseFlags(i)?'ON':'OFF')
					.replace('$OFF',this.hasUseFlags(i)?'OFF':'ON')
				opts.push(config)
			}
	
	return opts.join(' ');
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getPrefix = function(version)
{
	if (this.pack.module == undefined)
		return this.prefix.prefix;
	else
		return this.prefix.prefix+"/Modules/installed/"+this.pack.module;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getPatchList = function(version)
{
	var final = [];
	
	var patches = this.getProperty("patch");
	if (patches != undefined)
	{
		for (var i in patches)
			final.push(this.prefix.getFile('share/homelinux/packages/db/patches/'+patches[i]));
	}
	
	return final;
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getSlot = function(version)
{
	return VersionHelper.getSlot(this.pack,version);
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getPackInstalled = function(version)
{
	return this.prefix.getFile("/share/homelinux/install-db/"+this.pack.name.replace(/[/]/g,"_")+"-"+this.getSlot(version)+".json");
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.isInstalled = function()
{
	var fname = this.getPackInstalled(this.getVersion());
	return fs.existsSync(fname);
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.applyVersionHints = function()
{
	if (this.hints == undefined)
		return;

	for (var i in this.hints)
	{
		if (this.prefix.versions[this.pack.name] != undefined)
			this.prefix.versions[this.pack.name] = VersionHelper.filterVersions(this.pack,this.hints[i].version,this.prefix.versions[this.pack.name]);
		if (this.pack.versions != undefined)
			return this.pack.versions = VersionHelper.filterVersions(this.pack,this.hints[i].version,this.pack.versions);
	}
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.replaceParentUseFlags = function(flags)
{
	var flags = flags.replace('[','').replace(']','').split(',');
	
	var ret = [];
	for (var i in flags)
	{
		if (flags[i].indexOf('#') != -1)
		{
			var flag = flags[i].replace(/[+#-]/g,'');
			if (this.hasUseFlags(flag))
				ret.push('+'+flag);
		} else if (flags[i] != '') {
			ret.push(flags[i]);
		}
	}
	
	return '['+ret.join(',')+']';
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.checkUseFlagHints = function()
{
	if (this.hints == undefined)
		return;
	
	var err = [];
	if (this.hints != undefined)
	{
		for (var i in this.hints)
		{
			if (this.hints[i].iuse != undefined)
			{
				var flags = this.hints[i].iuse.replace('[','').replace(']','').split(',');
				for (var j in flags)
				{
					var f = flags[j];
					var info = f + " ("+this.hints[i].parent.getNameSlot()+")";
					if (!this.hasUseFlags(f))
						err.push(info);
				}
			}
		}
	}
	
	if (err.length > 0)
	{
		console.error("Package "+this.pack.name+" has some missing useflags properties to match dependencies : "+err);
		process.exit(1);
	}
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getNameSlot = function()
{
	return this.pack.name+":"+this.getSlot(this.getVersion());
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
	script.push("HL_PREFIX=\""+this.prefix.prefix+"\"");
	script.push("HL_HOMECACHE=\""+this.userConfig.config.homecache+"\"");
	
	//setup package variables
	var version = this.getVersion();
	script.push("NAME=\""+this.pack.name+"\"");
	script.push("SHORT_NAME=\""+this.getShortName()+"\"");
	script.push("VERSION=\""+version+"\"");
	script.push("URLS=\""+this.pack.urls.join(' ')+"\"");
	script.push("MD5=\""+this.pack.md5[version] != undefined?this.pack.md5[version]:''+"\"");
	script.push("SUBDIR=\""+this.pack.subdir+"\"");
	script.push("PREFIX=\""+this.getPrefix(version)+"\"");
	script.push("BUILD_OPTIONS=\""+this.buildOptions()+"\"");
	script.push("PATCHES=\""+this.getPatchList(version)+"\"");
	script.push("SLOT=\""+this.getSlot(version)+"\"");
	if (this.pack.module == undefined)
		script.push("MODULE=\"\"");
	else
		script.push("MODULE=\""+this.pack.module+"\"");
	
	script.push('');
	this.pack.version = version;
	this.pack.slot = this.getSlot(version);
	script.push("PACK_INSTALLED=\""+this.getPackInstalled(version)+"\"");
	script.push("PACK_JSON=\""+JSON.stringify(this.pack,null,'\t').replace(/[\\]/g,'\\\\').replace(/"/g,'\\"').replace(/[$]/g,'\\$')+"\"");
	script.push('');
	
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
	script.push("hl_pack_finish")
	script.push("start_stop Finish $NAME-$VERSION");
	
	return script.join('\n');
}

/*******************  FUNCTION  *********************/
PackageBuilder.prototype.getVersion = function()
{
	var ret;
	if (this.pack.version != undefined)
		ret = this.pack.version;
	else if (this.prefix.config.versions != undefined && this.prefix.config.versions[this.pack.name] != undefined)
		ret = this.prefix.config.versions[this.pack.name];
	else if (this.prefix.versions[this.pack.name] != undefined)
		ret = this.prefix.versions[this.pack.name][this.prefix.versions[this.pack.name].length-1];
	else
		ret = this.pack.versions[this.pack.versions.length-1];
	if (ret == undefined)
	{
		console.error(this.hints);
		throw "Fail to find version for package "+this.pack.name+" dep requirement might be too strict and eliminate all possibilities !";
	}
	return ret;
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
