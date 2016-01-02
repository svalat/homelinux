/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.1.0-dev                      *
*           Date    : 01/2016                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

var test = require('unit.js');
var VersionHelper = require('../libs/VersionHelper')

var packExample = {
    "slots": {
        "1": ">=1.0 <2.0",
        "2": ">=2.0"
    }            
};

var versions = [
        "0.1",
        "0.1.1",
        "0.2",
        "1.1",
        "1.2",
        "1.3",
        "2.1",
    ];

var packExempleRegExp = {
    "slots": {
        "~": "^([0-9]+)"
    }
};

describe("VersionHelper.getSlot",function() {
    it("None",function() {
        test.string(VersionHelper.getSlot(packExample,"0.0")).is("0");
    });
    
    it("MinMax", function() {
        test.string(VersionHelper.getSlot(packExample,"1.0")).is("1");
    });
    
    it("Min", function() {
        test.string(VersionHelper.getSlot(packExample,"2.0")).is("2");
        test.string(VersionHelper.getSlot(packExample,"2.1")).is("2");
    });
    
    it("RegExp", function() {
        test.string(VersionHelper.getSlot(packExempleRegExp,"3.2")).is("3");
    });
});

describe("VersionHelper.applyVersionOp",function() {
    it("=",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,"=1.1","1.0")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"=1.1","1.1")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"=1.1","1.2")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"=1.1","2.0")).isFalse();
    });
    
    it(">=",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,">=1.1","1.0")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,">=1.1","1.1")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,">=1.1","1.2")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,">=1.1","2.0")).isTrue();
    });
    
    it(">",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,">1.1","1.0")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,">1.1","1.1")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,">1.1","1.2")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,">1.1","2.0")).isTrue();
    });
    
    it("<=",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,"<=1.1","1.0")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"<=1.1","1.1")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"<=1.1","1.2")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"<=1.1","2.0")).isFalse();
    });
    
    it("<",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,"<1.1","1.0")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"<1.1","1.1")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"<1.1","1.2")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"<1.1","2.0")).isFalse();
    });
    
    it("!",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,"!1.1","1.0")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"!1.1","1.1")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"!1.1","1.2")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"!1.1","2.0")).isTrue();
    });
    
    it(":",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,":1","1.0")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,":1","1.1")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,":1","1.2")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,":1","2.0")).isFalse();
    });
    
    it("~",function() {
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.1","1.0")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.1","1.1")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.1","1.1.1")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.1","1.2")).isFalse();
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.1","2.0")).isFalse();
        
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.[0-3]","1.2")).isTrue();
        test.bool(VersionHelper.applyVersionOp(packExample,"~1.[0-3]","1.4")).isFalse();
    });
});

describe("VersionHelper.sortUniqVersions",function() {
    it("case-1",function() {
        test.array(VersionHelper.sortUniqVersions(["1.0","0.3","0.1","2.0","1.0"])).is(["0.1","0.3","1.0","2.0"]);
    });
});

describe("VersionHelper.filterVersions",function() {
    it("basic equal",function() {
        test.array(VersionHelper.filterVersions(packExample,"=0.1",versions)).is(["0.1"]);
        test.array(VersionHelper.filterVersions(packExample,"=1.1",versions)).is(["1.1"]);
        test.array(VersionHelper.filterVersions(packExample,">=1.1",versions)).is(["1.1","1.2","1.3","2.1"]);
    });
    
    it("case-2",function() {
        test.array(VersionHelper.filterVersions(packExample,">=1.1 <=1.2",versions)).is(["1.1","1.2"]);
    });
    
    it("case-3",function() {
        test.array(VersionHelper.filterVersions(packExample,">=1.1 <=1.3 !1.2",versions)).is(["1.1","1.3"]);
    });
});
