/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
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
UseFlags.prototype.getApplyStatus = function(list,flag)
{
    var status = this.apply(list,flag);
    var name = flag.replace(/^[-+]/g,'');
    if (status == null)
        return "-"+name;
    else if (status)
        return (flag[0]=='+'?'+':'-')+name;
    else
        return (flag[0]=='-'?'+':'-')+name;
};

/*******************  FUNCTION  *********************/
UseFlags.prototype.apply = function(list,flag)
{
    //trivial
    if (flag == '' || flag == '+' || flag == 'always' || flag == '+always')
        return true;
    
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
        throw 'You try to apply a use flag which is never defined : "'+flag+'"';
    } else if (mode == '') {
        if (status == '')
            return null;
        else if (status == '+')
            return true;
        else if (status == '-')
            return false;
        else
            throw "Invalid status : "+status;
    } else if (mode == '+' && status == "+") {
        return true;
    } else if (mode == '-' && status == "-") {
        return true;
    } else {
        return false;
    }
};

/*******************  FUNCTION  *********************/
module.exports = new UseFlags();
