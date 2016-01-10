/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * This file procide all the necessary functions to manage the useflag semantic.
**/

/********************  GLOBALS  *********************/

/*********************  CLASS  **********************/
function UseFlags()
{
    
}

/*******************  FUNCTION  *********************/
/**
 * Exctract the flags name removine the enable/disable symbol.
 * So, ``+flag` and `-flag` becomes `flag`.
 * @param flag string to convert
**/
UseFlags.prototype.getFlagName = function(flag)
{
	if (flag[0] == '+' || flag[0] == '-')
		return flag.slice(1);
	else
		return flag;
}

/*******************  FUNCTION  *********************/
/**
 * Merge two list of use flags by removing the double definition.
 * It also manage the status changing if `+flags` follow `-flag` or
 * invert.
 * @param values Define the initial list of flags
 * @param additional Define the list of flags to add.
 * @param force Force usage of `+` and `-`.
**/
UseFlags.prototype.merge = function(values,additional,force)
{
	if (values == undefined)
		values = [];
	
	//merge
	if (additional != undefined)
		values = values.concat(additional);
	
	//loop and build status list
	var status = {};
	for (var i in values)
	{
		var value = values[i];
		var flag = value.replace(/^[-+]/g,'');
		if (value[0] == '-')
			status[flag] = '-';
		else if (value[0] == '+')
			status[flag] = '+';
		else if (force)
			status[flag] = '+';
		else if (status[flag] == undefined)
			status[flag] = '';
	}
	
	//build list
	var ret = [];
	for (var i in status)
		ret.push(status[i]+i);
	
	return ret;
};

/*******************  FUNCTION  *********************/
/**
 * Return the stats for the given flag. The status is returned
 * as `+` for enable, `-` for disable and empty string for undefined.
 * @param list List of flag status.
 * @param flag Flag to test (without any `+` and `-`, only flag name).
**/
UseFlags.prototype.status = function(list,flag)
{
	var status = null;
	for (var i in list)
	{
		var value = list[i];
		var vflag = value.replace(/^[-+]/g,'');
		if (vflag == flag)
		{
			if (value[0] == '-')
				status = '-';
			else if (value[0] == '+')
				status = '+';
			else
				status = '';
		}
	}
	
	return status;
};

/*******************  FUNCTION  *********************/
/**
 * Check if the flag request need to be applied or not (for configure and deps).
 * @param list List of defined flags status.
 * @param flag Flag string potentially with `+` or `-`. `-` will invert the return value.
**/
UseFlags.prototype.getApplyStatus = function(list,flag)
{
	var status = this.apply(list,flag);
	var name = flag.replace(/^[-+]/g,'');
	if (status == null)
		return name;
	else if (status)
		return (flag[0]=='+'?'+':'-')+name;
	else
		return (flag[0]=='-'?'+':'-')+name;
};

/*******************  FUNCTION  *********************/
/**
 * Similary to getApplyStatus but manage `&` operator between multiple flags
 * and return `true`, `false` of `null` instead of a string.
 * @param list List of defined flags status.
 * @param flag Flag string potentially with `+` or `-`. `-` will invert the return value.
**/
UseFlags.prototype.apply = function(list,flag)
{
	//trivial
	if (flag == '' || flag == '+' || flag == 'always' || flag == '+always')
		return true;
	
	//if value contain &
	if (flag.indexOf('&') != -1)
	{
		var state = true;
		var lst = flag.split("&");
		for (var i in lst)
		{
			var tmp = this.apply(list,lst[i].trim());
			if (tmp == null)
				state = null;
			else if (state == true && tmp == false)
				state = false;
		}
		return state;
	}
    
	//selcet mode
	var mode = '';
	if (flag[0] == '+')
		mode = '+';
	else if (flag[0] == '-')
		mode = '-';
	
	//remove
	flag = flag.replace(/^[+-]/g,'');
	
	//apply
	var status = this.status(list,flag);
	if (status == null)
	{
		throw new Error('You try to apply a use flag which is never defined : "'+flag+'"');
	} else if (mode == '') {
		if (status == '')
			return null;
		else if (status == '+')
			return true;
		else if (status == '-')
			return false;
		else
			throw new Error("Invalid status : "+status);
	} else if (mode == '+' && status == "+") {
		return true;
	} else if (mode == '-' && status == "-") {
		return true;
	} else {
		return null;
	}
};

/*******************  FUNCTION  *********************/
module.exports = new UseFlags();
