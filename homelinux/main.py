from lib.core import UserConfig
import sys

if __name__ == '__main__':
    userConfig = UserConfig()
    userConfig.load()
    userConfig.parseArgs(sys.argv)