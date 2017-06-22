
/*****************************************************
			 PROJECT  : homelinux
			 VERSION  : 2.0.0-dev
			 DATE     : 06/2017
			 AUTHOR   : Valat Sébastien
			 LICENSE  : CeCILL-C
*****************************************************/

/****************************************************/
var cheerio = require('cheerio');
var fs = require('fs');

/****************************************************/
if (process.argv.length <= 4)
{
	console.error("Missing arguement, usage : node index.js {FILE} {BALIS} {REFEXP}")
	process.exit(1);
}

/****************************************************/
var file = process.argv[2];
var balis = process.argv[3];
var regexp = new RegExp(process.argv[4]);

/****************************************************/
var html = fs.readFileSync(file).toString();

/****************************************************/
var $ = cheerio.load(html);
$(balis).each(function() {
	var text = $(this).text();
	var ret = regexp.exec(text);
	if (ret != null)
		console.log(ret[1]);
});