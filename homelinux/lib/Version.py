# -*- coding: utf-8 -*-

class Version:
	"""Class to handle version numbers"""

	def __init__(self,version):
		"""Version constructor"""
		self.version = version
	
	def __fillNumber(self,number,pad):
		"""Fill all the number component such a way they ues the same size.
		Eg. 1.3.35a becore 0001.0003.0035a-beta"""

		#search first pos which is not a number
		endNum = 0
		while (endNum < len(number) and number[endNum] >= '0' and number[endNum] <= '9'):
			endNum = endNum + 1

		#build repearted 0 for padding
		fillPre = '0' * (pad -  endNum)
		fillPost = '0' * (pad - (len(number) - endNum))

		#build final
		return fillPre + number + fillPost

	def __buildPaddedVersion(self,version,pad):
		"""Build a version number as a string with padded 0s to easily
		use string compare operator for sorting and comparing"""

		#split on points
		parts = version.split(".")

		#replace
		final = []
		for part in parts:
			final.append(self.__fillNumber(part,pad))
		
		#ret
		return final

	def __compareVersions(self,version1,version2):
		"""Compare two version and return status, -1, 0, +1
		if respectively version1 is smaller, equal or larger than
		version 2"""

		#prep
		pv1 = self.__buildPaddedVersion(version1,8)
		pv2 = self.__buildPaddedVersion(version2,8)

		#fill missing if not same size
		zeros = '0' * 16
		m = max(len(pv1),len(pv2))
		for i in range(m):
			if i >= len(pv1):
				pv1.append(zeros)
			if i >= len(pv2):
				pv2.append(zeros)
		
		#compare
		for i in range(m):
			if pv1[i] < pv2[i]:
				return -1
			elif pv1[i] > pv2[i]:
				return 1

		#equal
		return 0
	
	def __cmp__(self,other):
		"""Compare two version"""
		print "ok"
		return self.__compareVersions(self.version,other.version)
