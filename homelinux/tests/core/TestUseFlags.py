import unittest

from lib.core import UseFlags

class TestUseFlags(unittest.TestCase):
    def test_constructor(self):
        use = UseFlags("qt gtk")
        self.assertEqual("gtk qt",use.toString())
    
    def test_merge_1(self):
        use = UseFlags()
        use.merge("-gtk +qt  svg")
        use.merge("kde")
        self.assertEqual("+qt -gtk svg kde",use.toString())

    def test_toString(self):
        use = UseFlags("-gtk +qt svg")
        self.assertEqual("+qt -gtk svg",use.toString())
        self.assertEqual("+svg +qt -gtk",use.toString(True))

    def test_getStatus(self):
        use = UseFlags("-gtk +qt svg")
        self.assertEqual(UseFlags.FLAG_ENABLED,use.getStatus("qt"))
        self.assertEqual(UseFlags.FLAG_DISABLED,use.getStatus("gtk"))
        self.assertEqual(UseFlags.FLAG_AUTO,use.getStatus("svg"))
    
    def test_getApplyStatus(self):
        flags = UseFlags("+qt -gtk svg")
	
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatus("qt"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatus("+qt"))
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatus("-qt"))
        
        self.assertEqual(UseFlags.FLAG_DISABLED,flags.getApplyStatus("gtk"))
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatus("+gtk"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatus("-gtk"))
        
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatus("svg"))
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatus("+svg"))
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatus("-svg"))
    
    def test_getApplyStatusWithAnd(self):
        flags = UseFlags("+qt +kde -gtk svg")
	
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("+qt"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("+qt&-gtk"))
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatusWithAnd("+qt&+gtk"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("qt&kde"))
        self.assertEqual(UseFlags.FLAG_AUTO,flags.getApplyStatusWithAnd("qt&svg"))

    def test_setAuto(self):
        flags = UseFlags("+qt -kde gtk svg")
        flags.setAuto(UseFlags.FLAG_ENABLED)

        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("qt"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("gtk"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("svg"))
        self.assertEqual(UseFlags.FLAG_DISABLED,flags.getApplyStatusWithAnd("kde"))
    
    def test_merge_2(self):
        flags = UseFlags("+qt +kde -gtk svg")
        flags2 = UseFlags("-qt -kde +gtk +svg")
        flags.merge(flags2)
        
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("gtk"))
        self.assertEqual(UseFlags.FLAG_ENABLED,flags.getApplyStatusWithAnd("svg"))
        self.assertEqual(UseFlags.FLAG_DISABLED,flags.getApplyStatusWithAnd("qt"))
        self.assertEqual(UseFlags.FLAG_DISABLED,flags.getApplyStatusWithAnd("kde"))
    
    def test_merge_3(self):
        flags = UseFlags()
        flags.merge("-gtk +qt  svg ")
        flags.merge(["+gtk", "kde"],True)
        self.assertEqual("+gtk +qt svg",flags.toString())
    
    def test_hasFlag(self):
        flags = UseFlags("+qt +kde -gtk svg")
        self.assertTrue(flags.hasFlag("qt"))
        self.assertFalse(flags.hasFlag("png"))