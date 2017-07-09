import unittest

from lib import Version

class TestVersion(unittest.TestCase):
	def test_equal_1(self):
		v1 = Version("3.4.5")
		v2 = Version("3.4.6")
		self.assertTrue(v1 < v2)
		self.assertTrue(v2 > v1)
		self.assertFalse(v2 == v1)
	
	def test_equal_1(self):
		v1 = Version("3.4")
		v2 = Version("3.4.6")
		self.assertTrue(v1 < v2)
		self.assertTrue(v2 > v1)
		self.assertFalse(v2 == v1)

	def test_equal_1(self):
		v1 = Version("3.4.6")
		v2 = Version("3.4.6")
		self.assertFalse(v1 < v2)
		self.assertFalse(v2 > v1)
		self.assertTrue(v2 == v1)

	def test_sort(self):
		lst = [Version("1.2"),Version("1.3"),Version("1.2.1")]
		out = sorted(lst)
		self.assertEqual(out[0].version,"1.2")
		self.assertEqual(out[1].version,"1.2.1")
		self.assertEqual(out[2].version,"1.3")