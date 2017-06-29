# -*- coding: utf-8 -*-
import os
import getpass
import json
import logging
import sys

class UserConfig:
    """Structure to store the configuration of Homelinux"""

    def __init__(self):
        """Default constructor of class"""

        #setup default
        self.config = {
            "prefix": [ os.path.expanduser("~")+"/usr" ],
            "host": "default",
            "modules" : [],
            "ccacge": False,
            "homecahe": False,
            "pyEnv": True,
            "packageOverride":{},
            "tmp": "/tmp/homelinux-"+getpass.getuser()
        }
        self.showHelp = False
        self.command = ""
        self.args = []

    def load(self,path = ""):
        """Load the given config file. By default it will load ~∕.homelinux.json"""

        #setup default
        if path == "":
            path = os.path.expanduser("~")+"/.homelinux.json"

        #check if exist
        if os.path.exists(path) == False:
            print "No "+path+", using default config !"
            return
        
        #load from file
        with open(path) as dataFile:
            data = json.load(dataFile)
        
        #merge
        self.config.update(data)

    def parseArgs(self):
        """Parse user arguments"""
        i = 1

        #Loop on all options
        while i < len(sys.argv):
            if sys.argv[i] == "--versoin":
                print "Homelinux 2.0.0-dev"
                sys.exit(0)
            elif sys.argv[i] == "-p" or sys.argv[i] == "--prefix":
                if i < len(sys.argv) - 1:
                    logging.critical("Option -p/--prefix requires an argument")
                del self.config["prefix"][:]
                i = i+1
                self.config["prefix"].append(sys.argv[i])
            elif sys.argv[i] == "--help" or sys.argv[i] == "-h":
                self.showHelp = True
            elif self.command == "":
                self.command = sys.argv[i]
            else:
                self.args.append(sys.argv[i])
            i = i+1