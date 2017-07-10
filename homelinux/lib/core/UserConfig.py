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
            "ccache": False,
            "homecache": False,
            "pyEnv": True,
            "packageOverride":{},
            "temp": "/tmp/homelinux-"+getpass.getuser(),
            "github": {
                "clientId": "",
                "clientSecret": ""
            },
            "crawlerThreads": 4,
            "crawlerTimeout": 60,
            "editor": "vim"
        }
        self.showHelp = False
        self.command = ""
        self.args = []
    
    def save(self):
        """Save default config in user home directory"""

        #build path
        path = os.path.expanduser("~")+"/.homelinux.json"

        #save
        with open(path,'w') as outfile:
            json.dump(self.config,outfile)

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

    def parseArgs(self,args):
        """Parse user arguments"""
        i = 1

        #Loop on all options
        while i < len(args):
            if args[i] == "--version":
                print "Homelinux 2.0.0-dev"
                sys.exit(0)
            elif args[i] == "-p" or args[i] == "--prefix":
                if i < len(args) - 1:
                    logging.critical("Option -p/--prefix requires an argument")
                del self.config["prefix"][:]
                i = i+1
                self.config["prefix"].append(args[i])
            elif args[i] == "--help" or args[i] == "-h":
                self.showHelp = True
            elif self.command == "":
                self.command = args[i]
            else:
                self.args.append(args[i])
            i = i+1