/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

/*********************  CLASS  **********************/
function EnvSetup(userConfig)
{
	this.userConfig = userConfig;
	this.loadedPerfix = [];
	this.vars = {
		'PATH':process.env.PATH,
		'LD_LIBRARY_PATH':process.env.LD_LIBRARY_PATH,
		'CPATH':process.env.PATH,
		'MANPATH':process.env.PATH,
		'PERL5LIB':process.env.PATH,
		'PKG_CONFIG_PATH':process.env.PATH,
		'MODULEPATH':process.env.MODULEPATH,
	};
}

/*******************  FUNCTION  *********************/
EnvSetup.prototype.enableCCache = function()
{
	if (this.userConfig.config.ccache)
		this.prepend('PATH',this.userConfig.config.prefix + "/bin/ccache-links");
}

/*******************  FUNCTION  *********************/
EnvSetup.prototype.prepend = function(varname,value,sep)
{
	//select
	if (sep == undefined)
		sep = ":";

	//apply
	if (this.vars[varname] != '' && this.vars[varname] != undefined)
		this.vars[varname] = value + sep + this.vars[varname];
	else
		this.vars[varname] = value;
}

/*******************  FUNCTION  *********************/
function contains(arr,obj) {
    var i = arr.length;
    while (i--) {
        if (arr[i] === obj) {
            return true;
        }
    }
    return false;
}

/*******************  FUNCTION  *********************/
EnvSetup.prototype.addPrefix = function(prefix)
{
	//check if already loaded
	if (contains(this.loadedPerfix,prefix))
		return;
	else
		this.loadedPerfix.push(prefix);
	
	//basic
	this.prepend('PATH',prefix + "/bin");
	this.prepend('LD_LIBRARY_PATH',prefix + "/lib");
	this.prepend('LD_LIBRARY_PATH',prefix + "/lib64");
	
	//more advanced
	this.prepend('MANPATH',prefix + "/share/man");
	this.prepend('CPATH',prefix + "/include");
	
	//perl
	this.prepend('PERL5LIB',prefix + "/lib/perl5");
	this.prepend('PERL5LIB',prefix + "/lib/perl5/site_perl");
	
	//pkg-config
	this.prepend('PKG_CONFIG_PATH',prefix + "/lib/pkgconfig");
	this.prepend('PKG_CONFIG_PATH',prefix + "/share/pkgconfig");
	
	//python
	this.prepend('PYTHONPATH',prefix + "/lib/python"+this.userConfig.config.python+"/site-packages/");
	
	//module
	this.prepend('MODULEPATH',prefix + "/Modules/modulefiles");
}

/*******************  FUNCTION  *********************/
EnvSetup.prototype.print = function()
{
	for (var i in this.vars)
		console.log("export "+i+"="+this.vars[i]+"");
}

/*******************  FUNCTION  *********************/
module.exports = EnvSetup;