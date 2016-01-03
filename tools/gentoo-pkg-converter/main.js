var request = require('request-sync');
var fs = require('fs');
var child_process = require('child_process');

//extract args
var pack = process.argv[2];
//console.log("Import : "+pack);

//fetch url
var url = "https://gitweb.gentoo.org/repo/gentoo.git/plain/"+pack;
//console.log(" + Fetch : "+url);
// var rep = request(url);

//check error
// if (rep.statusCode != 200)
// 	throw "Invalid status : "+rep.statusCode;

//extract value
// var genpack = rep.body.toString();

//write to file
var fname = "/tmp/hl-gentoo-pack-to-convert.ebuild"
//console.log(" + Save to temp : "+fname);
// fs.writeFileSync(fname,genpack);
genpack = fs.readFileSync(fname);

//extarct parts
var parts = {
	home: child_process.execSync('./ebuild-extractor.sh home '+fname).toString().trim(),
	deps: child_process.execSync('./ebuild-extractor.sh deps '+fname).toString(),
	config: child_process.execSync('./ebuild-extractor.sh config '+fname).toString().trim(),
	use: child_process.execSync('./ebuild-extractor.sh use '+fname).toString().trim()
};

function pushIf(array,key,value)
{
	if (array[key] == undefined)
		array[key] = [];
	if (array[key].indexOf(value) == -1)
		array[key].push(value);
}

//gen configure
var config = parts.config.split(' ');
var cfg = {};
for (var i in config)
{
	if (config[i][0] == '@')
	{
		var args = config[i].split('@');
		pushIf(cfg,args[2],"--$"+args[1]+"-"+args[3]);
	} else {
		pushIf(cfg,'',config[i]);
	}
}

function getPackNameInfo(pack)
{
	var regexp = new RegExp("^([a-zA-Z0-9_-]+/[a-zA-Z0-9_-]+)/([a-zA-Z0-9_-]+)-([0-9.]+)(-r[0-9]+)?.ebuild$");
	var res = regexp.exec(pack);
	if (res == null)
		throw "Invalid package name "+pack;
	return {
		name:res[1],
		shortName:res[2],
		version:res[3]
	};
}

function convertDepName(dep)
{
	return dep.replace(/[=<>]/g,'')
		.replace('[]','')
		.trim()
		.replace(/:$/,'')
		.replace(/[0-9]+\\.[0-9]+(\\.[0-9]+)*/,'');
}

function convertDeps(depString)
{
	var deps = depString.split('\n');
	var res = {};
	var state = [];
	
	var regexpUseFilter = new RegExp("^([a-zA-Z0-9_-]+)\\? \\((.+)\\)$");
	var regexpOpen = new RegExp("^([a-zA-Z10-9_-]+)\\? \\($");
	var regexpClose = new RegExp("^\\)$");
	
	for (var i in deps)
	{
		var d = deps[i].trim();
		if (regexpOpen.test(d)) {
			state.push(regexpOpen.exec(d)[1]);
		} else if (regexpClose.test(d)) {
			state.pop();
		} else if (regexpUseFilter.test(d)) {
			var filter = state.join('&');
			var infos = regexpUseFilter.exec(d);
			if (filter != '')
				filter+='&';
			filter += infos[1];
			pushIf(res,filter,convertDepName(infos[2]));
		} else if (d != '') {
			pushIf(res,state.join('&'),convertDepName(d));
		}
	}
	
	return res;
}

//final
var final = {
	name: getPackNameInfo(pack).name,
	versions: [ getPackNameInfo(pack).version ],
	inherit: "models/autotools",
	vfetcher: {
			mode: "http",
			url: "",
			regexp: getPackNameInfo(pack).shortName+"-([0-9]+\\.[0-9]+\\.?[0-9]*\\.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz)"
	},
	md5: {},
	subdir: getPackNameInfo(pack).shortName+"-${VERSION}",
	urls: [ "http://XXXXXX/"+getPackNameInfo(pack).shortName+"-${VERSION}.tar.bz2" ],
	deps: convertDeps(parts.deps),
	configure: cfg,
	use: parts.use.split(' '),
	host: {
		'default': false,
		debian8: [ getPackNameInfo(pack).shortName ]
	}
}

console.log(JSON.stringify(final,null,'\t'));