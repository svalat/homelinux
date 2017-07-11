# -*- coding: utf-8 -*-
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 07/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

from UseFlags import UseFlags
from Version import Version
from VersionMatcher import VersionMatcher
import json
import re

class PackageDef:
    """
    Package definition containing all the information needed to build a package.
    It also mange inheritance by providing function to merge content between
    packages.
    """

    def __init__(self):
        """Default constructor, init the data layout"""
        self.content = {
            #API in case of update
            "api": 1,
            #Name of the package.
            "name": "",
            #Home page of the project
            "homepage": "",
            #Inherit from given package
            "inherit": "",
            #List of version available for this package
            "versions": [],
            #Information, to crawl versions
            "vfetcher": {},
            #MD5 sums to check files
            "self": {},
            #subdir in archive
            "subdir": "",
            #List of deps. It uses a special format possibly filtering the deps.
	        #`+flag? depPackageName:SLOT [+flag1,-flag2] <=3.5 >=2.4 !2.4.4`
            "deps": [],
            #Name of this package on various hosts to check if present on system
            "host": {},
            #Map of configure arguemnt to use on configure script. Tha key represent
            #a filter based on flags and the values are list of string giving the list
            #of options to be used.
            "configure": {},
            #The values of this map can contain all entries to ovveride in package
            #it is applied only if the key (version selecor) match the selected version.
            #It permit to define special commands to apply for specific versions.
            #This make the global package unit for all versions.
            "vspecific": {},
            #Override some steps to provide commands to run durign install.
	        #Refer to package doc for more informations.
            "steps": {},
            #List of package which conflict with this one, this will produce failure
	        #at install if those package are present.
            "conflicts": [],
            #Warning message to display before installing this package
            "warn": [],
            #Install this package in module directory using the given name (which
	        #can contain `${SLOT}` or `${VESION}`).
            "module": "",
            #Load some script to have more bash commands in the package 
            "scripts": [],
            #Define some variable to be used by the bash commands of the package
            "vars": {},
            #List of flags (CFLAGS, CXXFLAGS) to append to system one. Can use
	        #`!-O3` to disable `-O3` defined by system.
            "flags": {
                "MAKEOPTS": [],
                "CFLAGS": [],
                "CXXFLAGS": [],
                "FFLAGS": [],
                "LDFLAGS": []
            },
            #List of URLs to use to download the package (can contain `${VERSION}`)?
	        #This is called in bash to it can also contain sub scripts to reformat
	        #version number for example with _ instead of .
            "urls": [],
            #List of patch to apply. Prefer to use this in version specific
            "patch": [],
            #List of slots
            "slots": {},
            #File containing the package
            "packageFile": ""
        }
        self.use = UseFlags()

    def loadFromFile(self,path):
        #check if exist
        if os.path.exists(path) == False:
            raise Exception("Package file %s does not exist !"%(path))
        
        #load from file
        with open(path) as dataFile:
            data = json.load(dataFile)
        
        #check API
        if data["api"] != 1:
            raise Exception("Invalid API %d in %s, should be 1 !"%(data["api"],path))
        
        #load content
        self.loadFromDic(data)
    
    def loadFromDic(self,json):
        """Apply the given json to current packet"""

        #apply all generic
        for var in self.content:
            if var in json:
                self.content[var] = json[var]

        #special case for versions
        if "versions" in json:
            self.content["versions"] = []
            for v in json["versions"]:
                self.content["versions"].append(Version(v))
            self.use.merge(self.content["use"])
    
    def saveToDic(self,out):
        """Fill the dir output with current status"""
        #generic
        for var in self.content:
            out[var] = self.content[var]
        #conver version list
        lst = []
        for v in self.content["versions"]:
            lst.append(str(v))
        out["versions"] = lst
        out["use"] = self.use.toList()

    def saveToFile(self,path):
        """Save package to given file in JSON format"""
        #build dict
        data = {}
        self.saveToDic(data)

        #save
        with open(path,'w') as outfile:
            json.dump(data,outfile)

    def __mergeString(self,package,var):
        if package[var] != "":
            self.content[var] = package[var]
    
    @staticmethod
    def __mergeList(out,input,replace = False):
        #handle string to list
        if isinstance(input,str):
            if input:
                if replace:
                    out = [input]
                else:
                    out.append(input)
            return
        
        #if not a list
        if not isinstance(input,list):
            raise Exception("Invalid type, expect list, get %s"%(type(input)))

        #ceck if empty
        if input:
            if replace:
                out = input
            else:
                for value in input:
                    if value[0] == '!':
                        tmp = value[1:]
                        out = [ v for v in out if not v == tmp ]
                    else:
                        out.append(value)
    
    @staticmethod
    def __mergeMap(out,input,replace = False):
        if not isinstance(input,dict):
            raise Exception("Invalid type, expect dict, get %s"%(type(input)))
        for entry in input:
            if isinstance(input[entry],str):
                out[entry] = input[entry]
            elif isinstance(input[entry],list):
                PackageDef.__mergeList(out[entry],intput[entry],replace)
            else:
                out[entry] = input[entry]

    def merge(self,package):
        self.content["api"] = package.content["api"]
        self.__mergeString(package,"name")
        self.__mergeString(package,"homepage")
        self.__mergeString(package,"inherit")
        self.__mergeList(self.content["versions"],package.content["versions"],True)
        if package.content["vfetcher"]:
            self.content["vfetcher"] = package.content["vfetcher"]
        self.__mergeMap(self.content["md5"],package.content["md5"])
        self.__mergeString(package,"subdir")
        self.__mergeList(self.content["deps"],package.content["deps"])
        #todo make better merge
        if package.content["host"]:
            self.content["host"] = package.content["host"]
        self.__mergeMap(self.content["configure"],package.content["configure"])
        self.__mergeMap(self.content["vspecific"],package.content["vspecific"])
        self.__mergeMap(self.content["steps"],package.content["steps"],True)
        self.__mergeList(self.content["conflicts"],package.content["conflicts"])
        self.__mergeList(self.content["use"],package.content["use"])
        self.__mergeList(self.content["warn"],package.content["warn"])
        self.__mergeString(package,"module")
        self.__mergeList(self.content["scripts"],package.content["scripts"])
        self.__mergeMap(self.content["vars"],package.content["vars"])
        self.__mergeMap(self.content["flags"],package.content["flags"])
        self.__mergeList(self.content["urls"],package.content["urls"])
        self.__mergeList(self.content["patch"],package.content["patch"])
        self.__mergeMap(self.content["slots"],package.content["slots"])
        self.__mergeString(package,"packageFile")
        self.use.merge(package.use)


    def getVersion(self):
        """
        Return the current version (best one).
        CAUTION, it consider that version habve already been ordered
        """
        if self.content["versions"]:
            return self.content["versions"][0]
        else:
            return "unknown"
    
    def getSlot(self):
        """
        Return the current slot considering the best version
        available
        CAUTION, it consider that version habve already been ordered
        """
        return VersionMatcher.getSlot(self.getVersion(),self.content["slots"])

    def sortVersions(self):
        """Sort the version in desc. order"""
        self.content["versions"] = sorted(self.content["versions"],reverse=True)

    def getNVersions(self,n):
        """
        Return the N first versions
        CAUTION, it consider that version habve already been ordered
        """
        return self.content["vesrions"][:n]

    def getSlotName(self):
        """
        Return the name to be used for stow installation
        This correspond to the package name (full) plus
        the slot with `:` separator.
        """
        return self.content["name"]+":"+self.getSlot()

    def getShortName(self):
        """
        Return the short name of package. For example, `hl/app-shells/bash`
        become `bash`.
        """
        return self.content["name"].slit("/")[-1]

    def getStawName(self):
        """
        Used for stow directories, it is the slot name replacing some charts
        by underscore. (mostly `/` and `:`).
        """
        return  re.sub("[/.-]","_",self.content["name"])

    def getShortVersion(self):
        """
        Compute short vestion which keep only the first two diggit.
        This is usefull into URLs because many packages use this shorten address
        into subdirectories to store the achives.
        """
        return getVersion().getShort()

    def getStowName(self):
        return re.sub("[/:]","_",self.getSlotName())

    def getRealPrefix(self,prefix,stow):
        """
        Return the prefix in which to install the package.
        Caution, this is not necerssarly the homelinux prefix
        as it could be completed by module paths if module
        is enable by this package.
        """
        module = self.content["module"]
        if module:
            return self.prefix+"/Modules/installed/"+module
        else:
            if stow and self.getShortName() != "stow":
                return prefix+"/stow/"+self.getStowName()
            else:
                return prefix
    
    def getPackInstalled(self,prefix):
        """
        Json script to write on when the package is installed
        or to check if it is installed.
        
        prefix -- Prefix to which we add the internal path.
        """
        return prefix+"/homelinux/install-db/"+self.getStowName()+".json"

    def getBuildOptions(self):
        """
        Generate build option based on `configure` entries of package
        depending on use flags enabling
        """

        #loop on all entries
        configure = self.content["configure"]
        options = []
        for criteria in configure:
            #get status
            try:
                state = self.use.getApplyStatusWithAnd(criteria)
            except Exception as e:
                logging.fatal("%s\nInvalid flag in %s, se previous message! "%(e.args[0],self.content["name"]))
            
            #we ignore auto
            if state != UseFlags.FLAG_AUTO:
                #status
                status = (state == UseFlags.FLAG_ENABLED)

                #loop on childs
                for option in configure[criteria]:
                    option.replace("$enable","enable" if status else "disable")
                    option.replace("$disable","disable" if status else "enable")
                    option.replace("$with","with" if status else "without")
                    option.replace("$without","without" if status else "with")
                    option.replace("$yes","yes" if status else "no")
                    option.replace("$no","no" if status else "yes")
                    option.replace("$ON","ON" if status else "OFF")
                    option.replace("$OFF","OFF" if status else "ON")
                    option.replace("-$no-","-" if status else "-no-") #for QT4

                    #push
                    options.append("\\\""+option+"\\\"")
        #ret final
        return " ".joint(options)
        
    def getPatchList(self,prefix):
        lst = []
        for patch in self.content["patch"]:
            lst.append(prefix+"/homelinux/packages/db/patches"+patch)
        return " ".join(lst)

    def genScript(self,userConfig,prefix,parallelInstall,useGnuStow):
        """
        Generate the install script.
 
        userConfig      -- Global configuration of homelinux
        parallelInstall -- Enable generation for parallel installation of
                           packages. This requires some minot tricks in the 
                           generated script.
        prefix          -- Prefix in which to install
        """
        #to fill
        out = []

        #setup bash
        out.append("#!/bin/bash")

        #setup variables
        out.append("")
        out.append("#HL vars")
        out.append("HL_TEMP=\"%s\""%(userConfig.config.temp))
        out.append("HL_PACKAGE=\"%s\""%(prefix+"/homelinux/packages"))
        out.append("HL_PREFIX=\"%s\""%(prefix))
        out.append("HL_HOMECACHE=\"%S\""%("true" if userConfig.config['homecache'] else "false"))

        #compiler flags
        cplFlags = ["MAKEOPTS","CFLAGS","CXXFLAGS","FFLAGS","LDFLAGS"]
        out.append("")
        out.append("#compiler flags")
        for flag in cplFlags:
            out.append("HL_%s=\"%s\""%(flag," ".join(self.content["flags"][flag])))

        #pack infos
        out.append("")
        out.append("#Pack infos")
        out.append("NAME='%s'"%(self.content["name"]))
        out.append("SHORT_NAME='%s'"%(self.getShortName()))
        out.append("VERSION='%s'"%(self.getVersion()))
        out.append("SVERSION='%s'"%(self.getShortVersion()))
        out.append("URLS=\"%s\""%(" ".join(self.conent["urls"])))
        out.append("MD5='%s'"%(self.content["md5"][eslf.getVersion().version]))
        out.append("SUBDIR=\"%s\""%(self.content["subdir"]))
        out.append("SLOT=\"%s\""%(self.getSlot()))
        out.append("PREFIX=\"%s\""%(self.getRealPrefix(prefix,useGnuStow)))
        out.append("BUILD_OPTIONS=\"%s\""%(self.getBuildOptions()))
        out.append("PATCHES=\"%s\""%(self.getPatchList(prefix)))
        out.append("USE=\"%s\""%(self.use.toString()))
        out.append("MODULE=\"%s\""%(self.content["module"]))
        if useGnuStow:
            out.append("STOW_NAME=\"%s\""%(self.getStowName()))
        else:
            out.append("STOW_NAME=''")
        if parallelInstall:
            out.append("PINSTALL='true'")
        else:
            out.append("PINSTALL='false'")
        
        #to mark install and keep track
        out.append("")
        out.append("#to mark install and keep track")
        out.append("PACK_INSTALLED=\"%s\""%(self.getPackInstall(prefix)))

        #dump package
        out.append("")
        out.append("#package json")
        p = {}
        self.saveToDic(p)
        p = json.dump(p).replace("\\","\\\\").replace("\"","\\\"").replace("$","\\$")
        out.append("USE=\"%s\""%(p))

        #load scripts
        out.append("")
        out.append("#load scripts")
        for script in self.content["scripts"]:
            out.append("source \"%s/homelinux/packages/%s\""%(prefix,script))
        
        #extra vars
        out.append("")
        out.append("#extra vars")
        out.append("function hl_pack_extra_vars()")
        out.append("{")
        if not self.content["vars"]:
            out.append("\ttrue")
        else:
            for var in self.content["vars"]:
                out.append("\t%s=\"%s\""%(var,self.content["vars"][var]))
        out.append("}")

        #build steps
        out.append("")
        out.append("#package steps")
        for step in self.content["steps"]:
            out.append("")
            out.append("#Step %s"%(step))
            out.append("function hl_pack_%s"%(step))
            out.append("{")
            out.append("\tinfo '%s'"%(step))
            if not self.content["steps"][step]:
                out.append("\techo 'Nothing to do'")
            else:
                for entry in self.content["steps"][step]:
                    out.append("\t%s"%(entry))
            out.append("}")

            #parallel install or not
            out.append("")
            out.append("#Main calls to run")
            if parallelInstall:
                out.append("set -e")
                out.append("setup_vars")
                out.append("eval \"$@\"")
            else:
                out.append("set -e")
                out.append("setup_vars")
                out.append("hl_start")
                out.append("hl_pack_main")
                out.append("hl_stop")
