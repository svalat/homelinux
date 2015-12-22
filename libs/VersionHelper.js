var compareVersion = require('compare-version');

/*********************  CLASS  **********************/
function VersionHelper()
{
}

/*******************  FUNCTION  *********************/
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
}

/*******************  FUNCTION  *********************/
VersionHelper.prototype.applyVersionOp = function(pack,op,version)
{
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
			var tmp = compareVersion(version,operand);
			return (tmp == 0 || tmp == -1);
			break;
		case '<':
			ret = (compareVersion(version,operand) == -1);
			break;
		case '>=':
			var tmp = compareVersion(version,operand);
			return (tmp == 0 || tmp == 1);
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
			return regexp.test(version);
			break;
		case ':':
			var slot = this.getSlot(pack,version);
			//console.log(slot +"!="+operand);
			return (slot == operand);
			break;
		default:
			throw "Invalid operator "+ operator;
			break;
	}
	
	//console.error("Apply "+operator +" on "+operand+" and "+version+" => "+ret);
	return ret;
}

/*******************  FUNCTION  *********************/
VersionHelper.prototype.filterVersion = function(pack,rules,version)
{
	if (version == undefined)
		return undefined;
	
	//split and trim
	var rulesArray = rules.split(' ');
	for (var j in rulesArray)
		rulesArray[j] = rulesArray[j].trim();
	
	var operator = '';
	var operand = '';
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
}

/*******************  FUNCTION  *********************/
VersionHelper.prototype.filterVersions = function(pack,rules,list)
{
	if (rules == undefined)
		return list;
	
	var ret = [];
	for (var i in list)
		if (this.filterVersion(pack,rules,list[i]))
			ret.push(list[i]);
	
	return ret;
}

/*********************  CONSTS  *********************/
module.exports = new VersionHelper();
