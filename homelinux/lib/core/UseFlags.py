# -*- coding: utf-8 -*-
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 07/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

from Colors import Colors
import logging

class UseFlags:
    """Class to manage status of use flags"""

    #setup some consts
    FLAG_DISABLED = 0
    FLAG_ENABLED = 1
    FLAG_AUTO = 2

    def __init__(self,flags=""):
        """Object constructor

        flags -- flags List of flags to load, they can conatin +/- to set state. 
                 Use space as separator
        """
        self.flags = {}
        self.__mergeString(flags)
    
    def merge(self,flags,onlyIfExists = False):
        """Generic version of merge"""
        if  isinstance(flags,str):
            self.__mergeString(flags,onlyIfExists)
        elif isinstance(flags,UseFlags):
            self.__mergeObject(flags,onlyIfExists)
        elif isinstance(flags,list):
            self.__mergeList(flags,onlyIfExists)
        else:
            logging.fatal("Invalid type : %s"%type(flags))

    def __mergeString(self,flags,onlyIfExists = False):
        """Merge the given flags"""
        lst = flags.split(" ")
        for flag in lst:
            if flag != "":
                self.addOne(flag,onlyIfExists = onlyIfExists)
    
    def __mergeObject(self,flags,onlyIfExists = False):
        """Merge from object"""
        for flag in flags.flags:
            self.addOne(flag,flags.flags[flag],onlyIfExists = onlyIfExists)
    
    def __mergeList(self,flags,onlyIfExists = False):
        """Merge from list"""
        for flag in flags:
            self.addOne(flag,onlyIfExists = onlyIfExists)

    @staticmethod
    def __getStatus(flag):
        """Extract status from flag depending if it start by '-' or '+' or nothing"""

        if flag[0] == '-':
            return UseFlags.FLAG_DISABLED
        elif flag[0] == '+':
            return UseFlags.FLAG_ENABLED
        else:
            return UseFlags.FLAG_AUTO
    
    @staticmethod
    def __getFlagName(flag):
        if flag[0] == '+' or flag[0] == '-':
            return flag[1:]
        else:
            return flag
    
    def addOne(self,flag,status = None,onlyIfExists = False):
        """Add a flag and apply its status it it override an existing one

        flag -- Name of the flag to apply (can start with +/- to enable disable)
        status -- If defined will apply this status
        onlyIfExists -- Will apply flag status only if this flag already exist
        """

        #get status
        if status == None:
            status = UseFlags.__getStatus(flag)
        
        #get name
        name = UseFlags.__getFlagName(flag)

        #if not exist
        if onlyIfExists:
            if name not in self.flags:
                return
        
        #apply
        if status == UseFlags.FLAG_ENABLED or status == UseFlags.FLAG_DISABLED:
            self.flags[name] = status
        elif name not in self.flags:
            self.flags[name] = UseFlags.FLAG_AUTO
        
    def getStatus(self,flag):
        """Return the current status of given flag or crash if not defined"""

        #get name
        name = UseFlags.__getFlagName(flag)

        #check
        if name in self.flags:
            return self.flags[name]
        else:
            raise Exception("Undefined flag %s"%(flag))
    
    def getApplyStatusWithAnd(self,flags):
        """Apply flag status possibly using & to give multiple flags to check"""

        if "&" in flags:
            state = UseFlags.FLAG_ENABLED
            lst = flags.split("&")
            for flag in lst:
                tmp = self.getApplyStatus(flag)
                if tmp == UseFlags.FLAG_AUTO:
                    state = UseFlags.FLAG_AUTO
                elif state == UseFlags.FLAG_ENABLED and tmp == UseFlags.FLAG_DISABLED:
                    state = UseFlags.FLAG_DISABLED
            return state
        else:
            return self.getApplyStatus(flags)
    
    def getApplyStatus(self,flag):
        """ Check if the flag request need to be applied or not (for configure and deps).
        flag -- Flag string potentially with `+` or `-`. `-` will invert the return value.
        """

        #trivial
        if flag == "" or flag == "+" or flag == "always" or flag == "+always":
            return UseFlags.FLAG_ENABLED

        #select mode
        mode = UseFlags.FLAG_AUTO
        if flag[0] == '+':
            mode = UseFlags.FLAG_ENABLED
        elif flag[0] == '-':
            mode = UseFlags.FLAG_DISABLED
        
        #apply
        status = self.getStatus(flag)
        if mode == UseFlags.FLAG_AUTO:
            return status
        elif mode == UseFlags.FLAG_ENABLED and status == UseFlags.FLAG_ENABLED:
            return UseFlags.FLAG_ENABLED
        elif mode == UseFlags.FLAG_DISABLED and status == UseFlags.FLAG_DISABLED:
            return UseFlags.FLAG_ENABLED
        else:
            return UseFlags.FLAG_AUTO
    
    def setAuto(self,state):
        """Replace AUTO status by given status

        state -- Define the status used to replace AUTO (default ENABLED)
        """
        for flag in self.flags:
            if self.flags[flag] == UseFlags.FLAG_AUTO:
                self.flags[flag] = state
    
    def toListByState(self,out,state,force = False):
        """ Convert the use flags to string only for the given state. This is used by toString
        to give the flags in order.

        out -- The output list to fill
        state -- State to filter
        force -- If force equal true, flas auto are set to enabled.	
        """
        for flag in self.flags:
            cur = self.flags[flag]
            if cur == state or (cur == UseFlags.FLAG_AUTO and force and state == UseFlags.FLAG_ENABLED):
                #flag
                if state == UseFlags.FLAG_ENABLED:
                    out.append("+" + flag)
                elif state == UseFlags.FLAG_DISABLED:
                    out.append("-" + flag)
                elif state == UseFlags.FLAG_AUTO and force:
                    out.append("+" + flag)
                else:
                    out.append(flag)

    @staticmethod
    def __coloredList(list):
        """ Add colors to element in the list

        list -- List of flags to color
        """
        for el in list:
            if el[0] == "+":
                el = Colors.cyan(el)
            elif el[0] == "-":
                el = Colors.red(el)
            else:
                el = Colors.blue(el)

    def toString(self,force = False,colored = False):
        """Convert to string with or without colors"""

        #build
        lst = self.toList(force)
        
        #color
        UseFlags.__coloredList(lst)

        #ret
        return " ".join(lst)

    def toList(self,force):
        """Convert to list"""
        lst = []
        self.toListByState(lst,UseFlags.FLAG_ENABLED,force)
        self.toListByState(lst,UseFlags.FLAG_DISABLED,force)
        if not force:
            self.toListByState(lst,UseFlags.FLAG_AUTO,False)
        return lst

    def hasFlag(self,name):
        """Check if has flag"""

        if name in self.flags:
            return True
        else:
            return False