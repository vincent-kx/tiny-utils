import ConfigParser

class MylibConfig:
	"""docstring for MylibConfig"""
	def __init__(self, configFile):
		self.configFile = configFile
		self.config = ConfigParser.ConfigParser(allow_no_value=True)
		self.config.read(configFile)

	def GetStrValue(self,section,item):
		 return self.config.get(section,item)

	def GetIntValue(self,section,item):
		return int(self.config.get(section,item))

	def GetFloatValue(self,section,item):
		return float(self.config.get(section,item))

	def GetHexIntValue(self,section,item):
		return int(self.config.get(section,item),base=16)

	def GetStrListValue(self,section,item):
		values = self.config.get(section,item)
		return values.split(',')

	def GetIntListValue(self,section,item):
		values = self.config.get(section,item)
		return [int(values[i]) for i in range(0,len(values))]

	def GetFloatListValue(self,section,item):
		values = self.config.get(section,item)
		return [float(values[i]) for i in range(0,len(values))]

	def GetHexIntListValue(self,section,item):
		values = self.config.get(section,item)
		return [int(values[i],base = 16) for i in range(0,len(values))]

if __name__ == '__main__':
	config = MylibConfig("../current_srv_tools/current_srv_tools.ini")
	print '############################################'
	print "mysql:"
	print config.GetStrValue("mysql","host")
	print config.GetIntValue("mysql","port")
	print config.GetStrValue("mysql","user")
	print config.GetStrValue("mysql","password")
	print config.GetStrValue("mysql","db")
	print '############################################'
	print "sftp"
	print config.GetStrValue("sftp","host")
	print config.GetIntValue("sftp","port")
	print config.GetStrValue("sftp","user")
	print config.GetStrValue("sftp","passwd")
	print '############################################'
	print "file path:"
	print config.GetStrValue("file_path","upfile_path")
	print config.GetStrValue("file_path","downfile_path")
	print config.GetStrValue("file_path","za_upfile_path")
	print config.GetStrValue("file_path","za_downfile_path")
	print '############################################'
	print "trade_srv:"
	print config.GetHexIntValue("trade_srv","cmd")
	print config.GetStrValue("trade_srv","host")
	print config.GetIntValue("trade_srv","port")
	print '############################################'
	print "emails:"
	print config.GetStrListValue("warn_emails","emails")