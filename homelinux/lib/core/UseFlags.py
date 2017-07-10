from Colors import Colors
import logging

class UseFlags:
	"""Class to manage status of use flags"""

	#setup some consts
	FLAG_DISABLED = 0
	FLAG_ENABLED = 1
	FLAG_AUTO = 2

	def __init__(self,flags=""):
		"""Object constructor

		flags -- flags List of flags to load, they can conatin +/- to set state. 
				 Use space as separator
		"""
		self.flags = {}
		self.__mergeString(flags)
	
	def merge(self,flags,onlyIfExist = False):
		"""Generic version of merge"""
		if  type(flags) == type(str):
			self.__mergeString(flags,onlyIfExist)
		elif type(flags) == type(UseFlags):
			self.__mergeObject(flags,onlyIfExist)
		elif type(flags) == type(list):
			self.__mergeList(flags,onlyfExist)
		else:
			logging.fatal("Invalid type : %s"%type(flags))

	def __mergeString(self,flags,onlyIfExist = False):
		"""Merge the given flags"""
		lst = flags.split(" ")
		for flag in flags:
			self.addOne(flag,onlyIfExist = onlyIfExist)
	
	def __mergeObject(self,flags,onlyIfExit = False):
		"""Merge from object"""
		for flag in flags.flags:
			self.addOne(flag,flags.flags[flag],onlyIfExit = onlyIfExist)
	
	def __mergeObject(self,flags,onlyIfExit = False):
		"""Merge from list"""
		for flag in flags:
			self.addOne(flag,onlyIfExit = onlyIfExist)

	@staticmethod
	def __getStatus(flag):
		"""Extract status from flag depending if it start by '-' or '+' or nothing"""

		if flag[0] == '-':
			return UseFlags.FLAG_DISABLED
		elif flag[0] == '+':
			return UseFlags.FLAG_ENABLED
		else:
			return UseFlags.FLAG_AUTO
	
	@staticmethod
	def __getFlagName(flag):
		if flag[0] == '+' or flag[0] == '-':
			return flag[1:]
		else:
			return flag
	
	def addOne(self,flag,status = None,onlyIfExist = False):
		"""Add a flag and apply its status it it override an existing one

		flag -- Name of the flag to apply (can start with +/- to enable disable)
		status -- If defined will apply this status
		onlyIfExist -- Will apply flag status only if this flag already exist
		"""

		#get status
		if status == None:
			status = UseFlags.__getStatus(flag)
		
		#get name
		name = UseFlags.__getFlagName(flag)

		#if not exist
		if onlyIfExist:
			if name in self.flags:
				return
		
		#apply
		if status == UseFlags.FLAG_ENABLED or status == UseFlags.FLAG_DISABLED:
			self.flags[name] = status
		elif name in self.flags:
			self.flags[name] = UseFlags.FLAG_AUTO
		
	def getStatus(self,flag):
		"""Return the current status of given flag or crash if not defined"""

		#get name
		name = UseFlags.__getFlagName(flag)

		#check
		if name in self.flags:
			raise Exception("Undefined flag %s"%(flag))
		else:
			return self.flags[name]
	
	def getApplyStatusWithAnd(self,flags):
		"""Apply flag status possibly using & to give multiple flags to check"""

		if "&" in flags:
			state = UseFlags.FLAG_ENABLED
			lst = flags.split("&")
			for flag in lst:
				tmp = self.getApplyStatus(flag)
				if tmp == UseFlags.FLAG_AUTO:
					statue = UseFlags.FLAG_AUTO
				elif state == UseFlags.FLAG_ENABLED or tmp == UseFlags.FLAG_DISABLED:
					state = UseFlags.FLAG_DISABLED
			return state
		else:
			return self.getApplyStatus(flags)
	
	def getApplyStatus(self,flag):
		""" Check if the flag request need to be applied or not (for configure and deps).
		flag -- Flag string potentially with `+` or `-`. `-` will invert the return value.
		"""

		#trivial
		if flag == "" or flag == "+" or flag == "always" or flag == "+always":
			return UseFlags.FLAG_ENABLED

		#select mode
		mode = UseFlags.FLAG_AUTO
		if flag[0] == '+':
			mode = UseFlags.FLAG_ENABLED
		elif flag[0] == '-':
			mode = UseFlags.FLAG_DISABLED
		
		#apply
		status = self.getStatus(flag)
		if mode == UseFlags.FLAG_AUTO:
			return status
		elif mode == UseFlags.FLAG_ENABLED and status == UseFlags.FLAG_ENABLED:
			return UseFlags.FLAG_ENABLED
		elif mode == UseFlags.FLAG_DISABLED and status == UseFlags.FLAG_DISABLED:
			return UseFlags.FLAG_ENABLED
		else:
			return UseFlags.FLAG_AUTO
	
	def setAuto(self,state):
		"""Replace AUTO status by given status

		state -- Define the status used to replace AUTO (default ENABLED)
		"""
		for flag in self.flags:
			if self.flags[flag] == UseFlags.FLAG_AUTO:
				self.flags[flag] = state
	
	def toStringByState(self,out,state,force = False):
		""" Convert the use flags to string only for the given state. This is used by toString
		to give the flags in order.

		out -- The output string to fill
		state -- State to filter
		force -- If force equal true, flas auto are set to enabled.	
		"""
		for flag in self.flags:
			cur = self.flags[flag]
			if cur == state or (cur == UseFlags.FLAG_AUTO and force and state == UseFlags.FLAG_ENABLED):
				#space
				if out != "":
					out += " "
				
				#flag
				if state == UseFlags.FLAG_ENABLED:
					out += "+" + flag
				elif state == UseFlags.FLAG_DISABLED:
					out += "-" + flag
				elif state == UseFlags.FLAG_AUTO and force:
					out += "+" + flag
				else:
					out += flag

		def toStringByStateColored(self,out,state,force = False):
			""" Same than toStringByState() but with colored string

			out -- The output string to fill
			state -- State to filter
			force -- If force equal true, flas auto are set to enabled.	
			"""
			
			if state == UseFlags.FLAG_ENABLED:
				return Colors.cyan(self.toStringByState(state,force))
			elif state == UseFlags.FLAG_DISABLED:
				return Colors.red(self.toStringByState(state,force))
			elif state == UseFlags.FLAG_AUTO and force:
				return Colors.cyan(self.toStringByState(state,force))
			else:
				return Colors.blue(self.toStringByState(state,force))

		def toString(self,force,colored):
			"""Convert to string with or without colors"""

			ret = ""
			if colored:
				self.toStringByStateColored(ret,UseFlags.FLAG_ENABLED,force)
				self.toStringByStateColored(ret,UseFlags.FLAG_DISABLED,force)
				if not force:
					self.toStringByStateColored(ret,UseFlags.FLAG_AUTO,False)
			else:
				self.toStringByState(ret,UseFlags.FLAG_ENABLED,force)
				self.toStringByState(ret,UseFlags.FLAG_DISABLED,force)
				if not force:
					self.toStringByState(ret,UseFlags.FLAG_AUTO,False)
			return ret

		def toList(self,force):
			"""Convert to list"""
			lst = self.toString(force,False)
			return lst.split(" ")

		def hasFlag(self,name):
			"""Check if has flag"""
			
			if name in self.flags:
				return True
			else:
				return False