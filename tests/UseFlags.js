/*****************************************************
*           Project : HomeLinux                      *
*           Version : 1.0.0                          *
*           Date    : 12/2015                        *
*           Licence : BSD                            *
*           Authors : Sebastien Valat                *
*****************************************************/

var test = require('unit.js');
var UseFlags = require('../libs/UseFlags')

describe("UseFlags.merge",function(){
   it("case-1",function(){
      test.array(UseFlags.merge(["+svg","+test","png","-debug"])).is(["+svg","+test","png","-debug"]); 
   });
   
   it("case-2",function(){
      test.array(UseFlags.merge(["+svg","+test","png","-debug","+debug","-test","svg"])).is(["+svg","-test","png","+debug"]); 
   });
   
   it("case-3",function(){
      test.array(UseFlags.merge(["+svg","+test","png","-debug"],["+debug","-test","svg"])).is(["+svg","-test","png","+debug"]); 
   });
   
   it("case-4",function(){
      test.array(UseFlags.merge(["+svg","+test","png","-debug"],["+debug","-test","svg"],true)).is(["+svg","-test","+png","+debug"]); 
   });
});


describe("UseFlags.status",function() {
    it("case-1",function() {
       test.string(UseFlags.status(["+svg","-debug","png"],"svg")).is("+");
    });
    
    it("case-2",function() {
       test.string(UseFlags.status(["+svg","-debug","png"],"debug")).is("-");
    });
    
    it("case-3",function() {
       test.string(UseFlags.status(["+svg","-debug","png"],"png")).is("");
    });
    
    it("case-4",function() {
       test.value(UseFlags.status(["+svg","-debug","png"],"jpg")).isNull();
    });
});

describe("UseFlags.apply",function() {
    it("case-1",function() {
        test.bool(UseFlags.apply(["+svg","-debug","png"],"+svg")).isTrue();
        test.bool(UseFlags.apply(["+svg","-debug","png"],"svg")).isTrue();
        test.bool(UseFlags.apply(["+svg","-debug","png"],"-svg")).isFalse();
    });
    
    it("case-2",function() {
        test.bool(UseFlags.apply(["+svg","-debug","png"],"+debug")).isFalse();
        test.bool(UseFlags.apply(["+svg","-debug","png"],"debug")).isFalse();
        test.bool(UseFlags.apply(["+svg","-debug","png"],"-debug")).isTrue();
    });
    
    it("case-3",function() {
        test.bool(UseFlags.apply(["+svg","-debug","png"],"+png")).isFalse();
        test.value(UseFlags.apply(["+svg","-debug","png"],"png")).isNull();
        test.bool(UseFlags.apply(["+svg","-debug","png"],"-png")).isFalse();
    });
});