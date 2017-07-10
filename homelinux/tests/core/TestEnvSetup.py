# -*- coding: utf-8 -*-
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 07/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

import unittest
import os

from lib.core import UserConfig
from lib.core import EnvSetup

class TestEnvSetup(unittest.TestCase):
    def setUp(self):
        self.env = EnvSetup(UserConfig())

    def test_constructor(self):
        pass

    def test_loadCurrent(self):
        self.env.loadCurrent()

    def test_addPrefix(self):
        self.env.addPrefix("/home/USER/usr")

        #gen
        out = self.env.genExport()
        
        #load reg
        dir = os.path.dirname(os.path.abspath(__file__))
        file = os.path.join(dir,"data","addPrefix.txt")
        ref = open(file,"r").read()

        #save
        #f = open(file,'w')
        #f.write(out);
        #f.close()

        #check
        self.assertEqual(ref,out)

    def test_removeExisting(self):
        self.env.addPrefix("/home/USER/usr")
        self.env.removeExisting()

        #gen
        out = self.env.genExport()
        
        #load reg
        dir = os.path.dirname(os.path.abspath(__file__))
        file = os.path.join(dir,"data","removeExisting.txt")
        ref = open(file,"r").read()

        #save
        #f = open(file,'w')
        #f.write(out);
        #f.close()

        #check
        self.assertEqual(ref,out)

    def test_hasPrefix(self):
        self.assertFalse(self.env.hasPrefix("/home/USER/usr"))
        self.env.addPrefix("/home/USER/usr")
        self.assertTrue(self.env.hasPrefix("/home/USER/usr"))
    
    def test_loadModules(self):
        self.env.userConfig.config["modules"].append("gcc")
        out = self.env.loadModules(True)
        ref = "if ! module 1>/dev/null 2>/dev/null ; then hl is-pack-installed sys-apps/modules > /dev/null && . $(hl prefix-of sys-apps/modules)/Modules/current/init/$(basename $SHELL); fi\nmodule load gcc\n"
        self.assertEqual(ref,out)
    
    def test_unloadModules(self):
        self.env.userConfig.config["modules"].append("gcc")
        out = self.env.loadModules(False)
        ref = "if ! module 1>/dev/null 2>/dev/null ; then hl is-pack-installed sys-apps/modules > /dev/null && . $(hl prefix-of sys-apps/modules)/Modules/current/init/$(basename $SHELL); fi\nmodule unload gcc\n"
        self.assertEqual(ref,out)
    
    def test_enableCCache_1(self):
        out = self.env.enableCCache()
        ref = ""
        self.assertEqual(ref,out)

    def test_enableCCache_2(self):
        self.env.userConfig.config["prefix"] = ["/home/USER/usr"]
        self.env.userConfig.config["ccache"] = True
        out = self.env.enableCCache()
        ref = "export PATH=/home/USER/usr/bin/ccache-links:$PATH\n"
        self.assertEqual(ref,out)

    def test_enablePyEnv_1(self):
        self.env.userConfig.config["pyEnv"] = False
        out = self.env.enablePyEnv()
        ref = ""
        self.assertEqual(ref,out)

    def test_enablePyEnv_2(self):
        self.env.userConfig.config["prefix"] = ["/home/USER/usr"]
        self.env.userConfig.config["pyEnv"] = True
        out = self.env.enablePyEnv()
        ref = "export PATH=/home/USER/usr/bin/hl-py-env-bins:$PATH\n"
        self.assertEqual(ref,out)