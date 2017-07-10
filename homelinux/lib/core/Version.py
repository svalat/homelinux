# -*- coding: utf-8 -*-

class Version:
    """Class to handle version numbers"""

    def __init__(self,version):
        """Version constructor

        Keyword arguments:
        version -- The version string to apply (eg 3.5.8a)
        """
        self.version = version
    
    @staticmethod
    def __fillNumber(number,pad = 8):
        """Fill all the number component such a way they ues the same size.
        Eg. 1.3.35a becore 0001.0003.0035a-beta.

        Keyword arguments:
        number -- The version number, eg 10a
        pad -- The padding to apply (default 8)
        """

        #search first pos which is not a number
        endNum = 0
        while (endNum < len(number) and number[endNum] >= '0' and number[endNum] <= '9'):
            endNum = endNum + 1

        #build repearted 0 for padding
        fillPre = '0' * (pad -  endNum)
        fillPost = '0' * (pad - (len(number) - endNum))

        #build final
        return fillPre + number + fillPost

    @staticmethod
    def __buildPaddedVersion(version,pad = 8):
        """Build a version number as a string with padded 0s to easily
        use string compare operator for sorting and comparing
        
        Keyword arguments:
        version -- The version to pad (eg 3.5.1a)
        pad -- The padding to apply (default 8)
        """

        #split on points
        parts = version.split(".")

        #replace
        final = []
        for part in parts:
            final.append(Version.__fillNumber(part,pad))
        
        #ret
        return final

    @staticmethod
    def __compareVersions(version1,version2):
        """Compare two version and return status, -1, 0, +1
        if respectively version1 is smaller, equal or larger than
        version 2
        
        Keyword arguments:
        version1 -- First version to compare
        version2 -- Second version to compare
        """

        #prep
        pv1 = Version.__buildPaddedVersion(version1,8)
        pv2 = Version.__buildPaddedVersion(version2,8)

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
        """Compare two version
        
        Keyword arguments:
        other -- Version to compare with current one
        """
        return self.__compareVersions(self.version,other.version)

    def __repr__(self):
        return "Version("+self.version+")"
    def __str__(self):
        return self.version