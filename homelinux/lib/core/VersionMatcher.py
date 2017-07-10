# -*- coding: utf-8 -*-
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 07/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

from Version import Version
import re
import logging

class VersionMatcher:
    """Contain rules to match certain versions for filtering"""

    def __init__(self,rules):
        """Constructor taking a rules formatted as a string

        Keyword arguments:
        rules -- List of rules as a string eg: >=3.4 <=5.3
        """
        self.ruleList = rules.split(' ')
    
    def match(self,version,slots):
        """ Apply all the selection rules on the given verison to check if it match.

        Keyword arguments:
        version -- Define the version to match.
        slots -- Give definition of slots to filter with slot operator.
        """
        for rule in self.ruleList:
            if self.__applyVersionOperator(rule,version,slots) == False:
                return False
        return True

    def filterList(self,versions,slots):
        """Apply matching on all element of the list and keep only
        the valid one.

        versions -- List of versions to match
        slots -- Give definition of slots to filter with slot operator.
        """
        final = []
        for version in versions:
            if self.match(version,slots):
                final.append(version)
        return final

    def __applyVersionOperator(self,op,version,slots):
        """Apply a version operation no given version
        Must be a single operation eg >=2.6

        op -- Operator to apply with its operand
        version -- version to check
        slots -- Slot rules to extract slot with :X operator
        """
        cnt = 1
        operator = op[0]

        #mangage >= & <=
        if op[1] == "=":
            cnt = cnt+1
            operator = operator + "="
        
        #extract operand
        operand = Version(op[cnt:])

        #apply operator
        if operator == "<=":
            return version <= operand
        elif operator == ">=":
            return version >= operand
        elif operator == ">":
            return version > operand
        elif operator == "<":
            return version < operand
        elif operator == "=":
            return version == operand
        elif operator == "!":
            return version != operand
        elif operator == "~":
            return None != re.match("^"+operand.version.replace(".","\\.")+".*",version.version)
        elif operator == ":":
            return self.getSlot(version,slots) == operand.version
        else:
            logging.critical("Invalid operator : %s"%op)

        
    @staticmethod
    def getSlot(version,slots):
        """Match a slot to the given version and return it

        version -- Version to match with slot
        slots -- Definition of slots to match
        """
        for slotName in slots:
            #regexp
            if slotName == "~":
                res = re.match("^"+slots[slotName].replace(".","\\.")+".*",version.version)
                if res == None:
                    loggin.critical("Version does not match slot : %s => %s"%(slots[slotName],version.vesrion))
                else:
                    return res.group(1)
            else:
                m = VersionMatcher(slots[slotName])
                if m.match(version,None):
                    return slotName
        #default
        return "0"