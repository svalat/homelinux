/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/********************  GLOBALS  *********************/
var fs = require('fs');

/***********************  DOC  **********************/
/**
 * This scripe provide some of the basic function to handle the
 * quick package system. reading file and build the complete
 * package from this into the given provider.
**/

/*******************  FUNCTION  *********************/
function QuickPackage(prefix)
{
	//keep track
	this.prefix = prefix;

	//set lit
	this.config = {
		deps: null,
		patch: null,
		config: null,
		version: null,
		module: null,
		type: null,
		subdir: null
	};
	
	//load
	for (var i in this.config)
		this.config[i] = this.loadQuickFile(i);
}

/*******************  FUNCTION  *********************/
QuickPackage.prototype.getInfos = function(packageName,defaults)
{
	//vars
	var final = {};
	
	//check
	if (defaults == undefined)
		defaults= {};
	
	//apply
	for (var i in this.config)
		final[i] = this.getQuickInfo(i,packageName,defaults[i]);

	//final return
	return final;
}

/*******************  FUNCTION  *********************/
/**
 * Load a file from quickpackages directory. It also make the splitting to convert
 * it into JS objects and arrays.
 * @param prop Name of the property to load (will be converted to prop.txt)
**/
QuickPackage.prototype.loadQuickFile = function(prop)
{
	var fname = this.prefix.getFile('homelinux/packages/quickpackages/'+prop+'.txt');
	var content = fs.readFileSync(fname);
	var lines = content.toString().split('\n');
	
	var ret = {};
	for (var i in lines)
	{
		if (lines[i][0] != '#' && lines[i][0] != '')
		{
			var infos = lines[i].split(' ');
			ret[infos[0]] = infos.slice(1);
		}
	}
	
	return ret;
};

/*******************  FUNCTION  *********************/
/**
 * Get a property from quickpackages system.
 * @param prop Define the property to read
 * @param name Define the name of the package
 * @param defaultValue Optional default value to apply if not present. Let to undefined by default.
**/
QuickPackage.prototype.getQuickInfo = function(property,packageName,defaultValue)
{
	switch(property)
	{
		case 'deps':
		case 'patch':
		case 'config':
			return (this.config[property][packageName] != undefined) ? this.config[property][packageName] : [];
			break;
		case 'version':
		case 'module':
		case 'type':
		case 'subdir':
			return (this.config[property][packageName] != undefined) ? this.config[property][packageName][0] : defaultValue;
		default:
			throw new Error("Invalid part : "+prop);
	}
};

/*******************  FUNCTION  *********************/
QuickPackage.prototype.genFullPackage = function(qp)
{
	//build package
	var pack = {
		"name": qp.name,
		"inherit": this.getQuickInfo('type',qp.name,qp.type == undefined ? 'models/auto' : qp.type),
		"versions": Array.isArray(qp.version)? qp.version: [ qp.version ],
		"subdir" : qp.subdir != undefined? qp.subdir : this.getQuickInfo('subdir',qp.name,qp.name.split('/').pop()+"-${VERSION}"),
		"urls": Array.isArray(qp.url)? qp.url : [ qp.url ],
		"deps": qp.deps == undefined ? this.getQuickInfo('deps',qp.name,[]) : this.getQuickInfo('deps',qp.name,[]).concat(qp.deps) ,
		"patch": this.getQuickInfo('patch',qp.name,[]), 
		"host": qp.host,
		"configure": qp.configure == undefined ? [] : { "":qp.configure },
		"provide": qp.provide,
		"md5": {},
		"steps": qp.steps,
		"module": this.getQuickInfo('module',qp.name)
	};
	
	return pack;
}

module.exports = QuickPackage;