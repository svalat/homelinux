import unittest
import os

from lib.core import UserConfig

class TestUserConfig(unittest.TestCase):
    def setUp(self):
        self.config = UserConfig()

    def test_load_valid(self):
        dir = os.path.dirname(os.path.abspath(__file__))
        file = os.path.join(dir,"data","userconfig.json")
        self.config.load(file)

        self.assertEqual("/home/USER/usr",self.config.config["prefix"][0])
        self.assertEqual(1,len(self.config.config["prefix"]))
        self.assertEqual("gentoo",self.config.config["host"])
        self.assertEqual(2,len(self.config.config["modules"]))
        self.assertEqual("gcc-5.0",self.config.config["modules"][0])
        self.assertEqual("openmpi",self.config.config["modules"][1])
        self.assertTrue(self.config.config["homecache"])
        self.assertTrue(self.config.config["ccache"])
        self.assertFalse(self.config.config["pyEnv"])
        self.assertEqual("--enable-debug",self.config.config["packageOverride"]["hl/sys-devel/gcc"]["configure"][""][0])
        self.assertEqual(4,self.config.config["crawlerThreads"])
        self.assertEqual(60,self.config.config["crawlerTimeout"])
        self.assertEqual("vim",self.config.config["editor"])
    
    def test_load_invalid(self):
        dir = os.path.dirname(os.path.abspath(__file__))
        file = os.path.join(dir,"data","userconfig2.json")
        self.config.load(file)
    
    def test_parse_args(self):
        self.config.parseArgs(["hl","--prefix","/usr/local","install","qt","gtk"])
        self.assertEqual(self.config.config["prefix"][0], "/usr/local")
        self.assertEqual(self.config.command,"install")
        self.assertEqual(self.config.args,["qt","gtk"])

