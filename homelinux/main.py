from lib import UserConfig

if __name__ == '__main__':
    userConfig = UserConfig()
    userConfig.load()
    userConfig.parseArgs()