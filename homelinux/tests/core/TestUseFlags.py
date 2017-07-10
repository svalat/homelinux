import unittest

from lib.core import UseFlags

class TestUseFlags(unittest.TestCase):
	def test_constructor(self):
		use = UseFlags("qt gtk")
		self.assertEqual("qt gtk",use.toString())