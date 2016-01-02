/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/***********************  DOC  **********************/
/**
 * This class provide some help functions to manage the version checking, mostly
 * to apply the version rules for dependencies.
**/

var compareVersion = require('compare-version');

/*********************  CLASS  **********************/
function VersionHelper()
{
}

/*******************  FUNCTION  *********************/
/**
 * Apply the slots regexp or min max and return the first slot which 
 * match the given version.
 * @param pack Define the package in which to take the slot definitions.
 * @param version Define the version to match.
**/
VersionHelper.prototype.getSlot = function(pack,version)
{
	//search slot
	if (pack.slots != undefined)
	{
		for (var i in pack.slots)
		{
			if (i == "~")
			{
				var regexp = new RegExp(pack.slots[i]);
				var ret = regexp.exec(version);
				if (ret != null)
					return ret[1];
			} else {
				if (this.filterVersion(pack,pack.slots[i],version))
					return i;
			}
		}
	}

	//no slot
	return '0';
};

/*******************  FUNCTION  *********************/
/**
 * Apply a version operator and operand onto the given version.
 * @param pack Define the package in use (only for the slot semantic)
 * @param op A string starting with operator (>, <, <=, >=, =, ~, ! and : for slot) and
 * a version as operand (no space between the two).
 * @param version The version to match with op.
**/
VersionHelper.prototype.applyVersionOp = function(pack,op,version)
{
	var tmp;
	var cnt = 1;
	var operator = op[0];
	if (op[1] == '=')
	{
		cnt++;
		operator += '=';
	}
	var operand = op.substring(cnt);
	
	var ret;
	switch(operator)
	{
		case '<=':
			tmp = compareVersion(version,operand);
			ret = (tmp == 0 || tmp == -1);
			break;
		case '<':
			ret = (compareVersion(version,operand) == -1);
			break;
		case '>=':
			tmp = compareVersion(version,operand);
			ret = (tmp == 0 || tmp == 1);
			break;
		case '>':
			ret = (compareVersion(version,operand) == 1);
			break;
		case '=':
			ret = (compareVersion(version,operand) == 0);
			break;
		case '!':
			ret = (compareVersion(version,operand) != 0);
			break;
		case '~':
			var regexp = new RegExp(operand);
			ret = regexp.test(version);
			break;
		case ':':
			var slot = this.getSlot(pack,version);
			//console.log(slot +"!="+operand);
			ret = (slot == operand);
			break;
		default:
			throw "Invalid operator "+ operator;
	}
	
	//console.error("Apply "+operator +" on "+operand+" and "+version+" => "+ret);
	return ret;
};

/*******************  FUNCTION  *********************/
/**
 * Sort a list of vesrions and remove duplicated entries.
 * @param arr Input array of versions to sort.
**/
VersionHelper.prototype.sortUniqVersions = function(arr) {
	return arr.sort(compareVersion).filter(function(el,i,a) {
		return (i==a.indexOf(el));
	});
};

/*******************  FUNCTION  *********************/
/**
 * Apply the rules onto the given version.
 * @param pack The current package, only to extract slot from version
 * @param rulse the list of rules to apply (operators and operand list with space separator)
**/
VersionHelper.prototype.filterVersion = function(pack,rules,version)
{
	if (version == undefined)
		return undefined;
	
	//split and trim
	var rulesArray = rules.split(' ');
	for (var j in rulesArray)
		rulesArray[j] = rulesArray[j].trim();
	
	var status = true;
	for (var j in rulesArray)
	{
		if (rulesArray[j] != '')
		{
			if (!this.applyVersionOp(pack,rulesArray[j],version))
			{
				status = false;
				break;
			}
		}
	}
	
	return status;
};

/*******************  FUNCTION  *********************/
/**
 * Filter a list of versions with the given rules.
 * @param pack Defineht the package in use for slot extraction
 * @param rules Define the list of rules to apply.
 * @param list Define the list of version to filter.
**/
VersionHelper.prototype.filterVersions = function(pack,rules,list)
{
	if (rules == undefined)
		return list;
	
	var ret = [];
	for (var i in list)
		if (this.filterVersion(pack,rules,list[i]))
			ret.push(list[i]);
	
	return ret;
};

/*********************  CONSTS  *********************/
module.exports = new VersionHelper();
