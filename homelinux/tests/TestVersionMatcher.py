import unittest

from lib import VersionMatcher
from lib import Version

class TestVersionMatcher(unittest.TestCase):
	def test_match_1(self):
		version1 = Version("1.3.5")
		version2 = Version("1.3.6")
		matcher = VersionMatcher("=1.3.5")
		self.assertTrue(matcher.match(version1,None))
		self.assertFalse(matcher.match(version2,None))
	
	def test_match_op_equal(self):
		self.assertTrue(VersionMatcher("=3.5").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("=3.5").match(Version("3.6"),None))
		self.assertFalse(VersionMatcher("=3.5").match(Version("4.5"),None))
	
	def test_match_op_sup(self):
		self.assertTrue(VersionMatcher(">3.5").match(Version("3.6"),None))
		self.assertTrue(VersionMatcher(">3.5").match(Version("3.5.1"),None))
		self.assertFalse(VersionMatcher(">3.5").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher(">3.5").match(Version("3.4"),None))
		self.assertFalse(VersionMatcher(">3.5.1").match(Version("3.5"),None))
	
	def test_match_op_sup_equal(self):
		self.assertTrue(VersionMatcher(">=3.5").match(Version("3.6"),None))
		self.assertTrue(VersionMatcher(">=3.5").match(Version("3.5.1"),None))
		self.assertTrue(VersionMatcher(">=3.5").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher(">=3.5").match(Version("3.4"),None))
		self.assertFalse(VersionMatcher(">=3.5.1").match(Version("3.5"),None))
	
	def test_match_op_inf(self):
		self.assertTrue(VersionMatcher("<3.5").match(Version("3.4"),None))
		self.assertTrue(VersionMatcher("<3.5.1").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("<3.5").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("<3.4").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("<3.5").match(Version("3.5.1"),None))

	def test_match_op_inf_equal(self):
		self.assertTrue(VersionMatcher("<=3.6").match(Version("3.4"),None))
		self.assertTrue(VersionMatcher("<=3.5.1").match(Version("3.5"),None))
		self.assertTrue(VersionMatcher("<=3.5").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("<=3.4").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("<=3.5").match(Version("3.5.1"),None))

	def test_match_op_not(self):
		self.assertTrue(VersionMatcher("!3.6").match(Version("3.5"),None))
		self.assertFalse(VersionMatcher("!3.6").match(Version("3.6"),None))
	
	def test_match_op_regex(self):
		self.assertTrue(VersionMatcher("~4.1").match(Version("4.1"),None))
		self.assertTrue(VersionMatcher("~4.1").match(Version("4.1.5"),None))
		self.assertFalse(VersionMatcher("~4.1").match(Version("4.2"),None))
		self.assertFalse(VersionMatcher("~4.1").match(Version("5"),None))
	
	def test_match_multiple(self):
		matcher = VersionMatcher(">=3.5 <=3.6.5")
		self.assertTrue(matcher.match(Version("3.5"),None))
		self.assertTrue(matcher.match(Version("3.5.3"),None))
		self.assertTrue(matcher.match(Version("3.6.2"),None))
		self.assertFalse(matcher.match(Version("3.6.6"),None))
		self.assertFalse(matcher.match(Version("3.7"),None))
	
	def test_match_slot_1(self):
		slots = {
			"3": ">=3.0 <4",
			"4": ">=4.0 <5"
		}
		matcher = VersionMatcher(":3")
		self.assertTrue(matcher.match(Version("3.5"),slots))
		self.assertTrue(matcher.match(Version("3.5.3"),slots))
		self.assertFalse(matcher.match(Version("4.6.2"),slots))
		self.assertFalse(matcher.match(Version("4.6.6"),slots))
		self.assertFalse(matcher.match(Version("5.7"),slots))
	
	def test_match_slot_2(self):
		slots = {
			"~": "([0-9]+.[0-9]+)"
		}
		matcher = VersionMatcher(":3.5")
		self.assertTrue(matcher.match(Version("3.5"),slots))
		self.assertTrue(matcher.match(Version("3.5.3"),slots))
		self.assertFalse(matcher.match(Version("4.6.2"),slots))
		self.assertFalse(matcher.match(Version("4.6.6"),slots))
		self.assertFalse(matcher.match(Version("5.7"),slots))
	
	def test_get_slot_1(self):
		slots = {
			"3": ">=3.0 <4",
			"4": ">=4.0 <5"
		}
		self.assertEqual("3",VersionMatcher.getSlot(Version("3.1"),slots))
		self.assertEqual("3",VersionMatcher.getSlot(Version("3.0"),slots))
		self.assertEqual("3",VersionMatcher.getSlot(Version("3.9.1"),slots))
		self.assertEqual("4",VersionMatcher.getSlot(Version("4.1"),slots))
	
	def test_filter_list_1(self):
		lst = [
			Version("1.2.1"),
			Version("1.2.2"),
			Version("1.3.1"),
			Version("2.3.1")
		]
		matcher = VersionMatcher(">=1.2.2 <=1.3.1")
		out = matcher.filterList(lst,None)
		expect = [
			Version("1.2.2"),
			Version("1.3.1")
		]
		self.assertEqual(out,expect)
	
	def test_filter_list_2(self):
		slots = {
			"~": "([0-9]+.[0-9]+)"
		}
		lst = [
			Version("1.2.1"),
			Version("1.2.2"),
			Version("1.3.1"),
			Version("2.3.1")
		]
		matcher = VersionMatcher(":1.2")
		out = matcher.filterList(lst,slots)
		expect = [
			Version("1.2.1"),
			Version("1.2.2")
		]
		self.assertEqual(out,expect)