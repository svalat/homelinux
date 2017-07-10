# -*- coding: utf-8 -*-
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 07/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

from lib.core import UserConfig
import sys

if __name__ == '__main__':
    userConfig = UserConfig()
    userConfig.load()
    userConfig.parseArgs(sys.argv)