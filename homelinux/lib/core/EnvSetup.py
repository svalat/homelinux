# -*- coding: utf-8 -*-
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 07/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

import os
import subprocess
import logging

class EnvSetup:
    """Handle environnement variable to setup for user"""

    #list of handled variables
    __HANDLED = [
        "PATH",
        "LD_LIBRARY_PATH",
        "CPATH",
        "MANPATH",
        "PERL5LIB",
	    "PKG_CONFIG_PATH",
        "MODULEPATH",
        "PYTHONPATH",
        "CMAKE_PREFIX_PATH",
	    "LD_RUN_PATH",
        "HL_PREFIX_PATH"
    ]

    def __init__(self,userConfig):
        """Constrcutor, expect a UserConfig object as parameter"""

        #user config
        self.userConfig = userConfig
        #end map
        self.env = {}

        #setup empty to start
        self.setupEmpty()
    
    def setupEmpty(self):
        """Init the env with empty vars"""
        for v in EnvSetup.__HANDLED:
            self.env[v] = []
    
    def loadCurrent(self):
        """Load current environnement status"""

        #load
        for var in self.env:
            if var in os.environ:
             self.env[var] = os.environ[var].split(":")
        
        #special for pkg-config otherwise we miss the system one
        if not self.env["PKG_CONFIG_PATH"]:
            self.prepend("PKG_CONFIG_PATH","/usr/lib/pkgconfig")
            self.prepend("PKG_CONFIG_PATH","/usr/share/pkgconfig")
            self.prepend("PKG_CONFIG_PATH","/usr/local/lib/pkgconfig")
            self.prepend("PKG_CONFIG_PATH","/usr/local/share/pkgconfig")
    
    def hasPrefix(self,prefix):
        """
        Check if the given prefix is already loaded
        
        prefix -- Prefix to check
        """
        return prefix in self.env["HL_PREFIX_PATH"]
    
    def addPrefix(self,prefix):
        """
        Add a new prefix to the environnement variable. This function setup all
        the variables managed by homelinux for the given prefix.
        
        prefix -- Path of the prefix to use.
        """

        #check if already added
        if self.hasPrefix(prefix):
            return

        #add prefix
        self.prepend("HL_PREFIX_PATH",prefix)

        #basic
        self.prepend("PATH",prefix + "/bin/protected")
        self.prepend("PATH",prefix + "/bin")
        self.prepend("PATH",prefix + "/sbin")
        self.prepend("LD_LIBRARY_PATH",prefix + "/lib")
        self.prepend("LD_LIBRARY_PATH",prefix + "/lib64")
        self.prepend("LD_RUN_PATH",prefix + "/lib")
        self.prepend("LD_RUN_PATH",prefix + "/lib64")

        #CMAKE_PREFIX_PATH
        self.prepend("CMAKE_PREFIX_PATH",prefix)

        #more advanced
        self.prepend("MANPATH",prefix + "/share/man")
        self.prepend("CPATH",prefix + "/include")

        #perl
        self.prepend("PERL5LIB",prefix + "/lib/perl5")
        self.prepend("PERL5LIB",prefix + "/lib/perl5/site_perl")

        #pkg-config
        self.prepend("PKG_CONFIG_PATH",prefix + "/lib/pkgconfig")
        self.prepend("PKG_CONFIG_PATH",prefix + "/share/pkgconfig")

        #python
        pv = "2.7"
        #out = subprocess.check_output(["python","--version"])
        #pv = ".".join(out.split(" ")[1].split(".")[0:2])
        self.prepend("PYTHONPATH",prefix + "/lib/python"+pv+"/site-packages/")

        #module
        self.prepend("MODULEPATH",prefix + "/Modules/modulefiles")

        #hl
        self.prepend("HL_PREFIX_PATH",prefix)

    def removeExistingVar(self,varName):
        """
        Remove removeExisting paths for the given variable
        """
        
        #check
        if not varName in self.env:
            raise Exception("Invalid var name : %s"%varName)

        #copy loaded
        loaded = self.env["HL_PREFIX_PATH"][:]
        
        #remove
        final = []
        for entry in self.env[varName]:
            keep = True
            for prefix in loaded:
                if prefix in entry:
                    logging.debug("In %s, remove %s",varName,entry)
                    keep = False
            if keep:
                final.append(entry)
        
        #repush
        self.env[varName] = final
    
    def removeExisting(self):
        """
        Remove the existing paths from homelinux on all vars
        """

        #remove in vars
        for var in self.env:
            if var != "HL_PREFIX_PATH":
                self.removeExistingVar(var)

        #clear loaded prefix
        self.env["HL_PREFIX_PATH"] = []

    def genExport(self):
        """
        Print the nevironenemnt variable setup (mostly export calls)
        to be used by shell scripts (bash/sh)
        """

        out = ""
        for var in self.env:
            out += "export '%s=%s'\n"%(var,":".join(self.env[var]))
        return out

    def loadModules(self,load = True):
        """Load of unload the modules"""

        out = "if ! module 1>/dev/null 2>/dev/null ; then hl is-pack-installed sys-apps/modules > /dev/null && . $(hl prefix-of sys-apps/modules)/Modules/current/init/$(basename $SHELL); fi\n"
        for module in self.userConfig.config["modules"]:
            if load:
                out += "module load %s\n"%(module)
            else:
                out += "module unload %s\n"%(module)

        return out

    def enableCCache(self):
        """Generate export PATH to enable ccache"""
        if self.userConfig.config["ccache"]:
            return "export PATH=%s/bin/ccache-links:$PATH\n"%(self.userConfig.config["prefix"][0])
        else:
            return ""
    
    def enablePyEnv(self):
        """Generate export PATH to enable python wrapper"""
        if self.userConfig.config["pyEnv"]:
            return "export PATH=%s/bin/hl-py-env-bins:$PATH\n"%(self.userConfig.config["prefix"][0])
        else:
            return ""
    
    def prepend(self,varName,value):
        self.env[varName].insert(0,value)
    
    def append(self,varName,value):
        self.env[varName].append(value)
