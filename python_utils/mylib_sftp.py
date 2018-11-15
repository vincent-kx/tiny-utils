import sys
import os
import paramiko

class MylibSftp:
	errmsg = ''
	"""docstring for MylibSftp"""
	def __init__(self):
		pass

	def Init(self,host,port,user,passwd):
		self.host = host
		self.port = port
		self.user = user
		self.passwd = passwd
		try:
			self.transport = paramiko.transport.Transport((self.host,self.port))
			self.transport.connect(username=self.user,password=self.passwd)
		except paramiko.ssh_exception.SSHException, e:
			MylibSftp.errmsg = str(e)
			return 1
		else:
			self.sftp_client = paramiko.sftp_client.SFTPClient.from_transport(self.transport)
			return 0

	def __del__(self):
		if self.sftp_client:
			self.sftp_client.close()
		if self.transport:
			self.transport.close()

	def Close(self):
		try:
			if self.sftp_client:
				self.sftp_client.close()
			if self.transport:
				self.transport.close()
		except Exception, e:
			MylibSftp.errmsg = str(e)
			pass

	def UploadFile(self,filename,tofile):
		try:
			self.sftp_client.put(filename,tofile)
		except Exception, e:
			MylibSftp.errmsg = str(e)
			return 1
		return 0

	def DownloadFile(self,fromfile,tofile):
		try:
			self.sftp_client.get(fromfile,tofile)
		except Exception, e:
			MylibSftp.errmsg = str(e)
			return 1
		return 0

	def IsFileExists(self,filename,path):
		try:
			files = self.sftp_client.listdir(path)
			return (filename in files)
		except Exception, e:
			MylibSftp.errmsg = str(e)
			return False
		return False

	def IsPathExists(self,path):
		try:
			self.sftp_client.stat(path)
			print "stat path",path
			return True
		except IOError:
			return False

	def CreatePath(self,path):
		if not self.IsPathExists(path):
			try:
				self.sftp_client.mkdir(path)
			except IOError,e:
				MylibSftp.errmsg = str(e)
				return 1;
		return 0

	def ChgDir(self,path):
		self.sftp_client.chdir(path)

	def ErrMsg(self):
		return MylibSftp.errmsg
		
if __name__ == '__main__':
	sftpClient = MylibSftp()
	sftpClient.Init('127.0.0.1',12345,'test','123456');
	sftpClient.UploadFile('mylib_sftp.py','/test/sftp/data/upload')
	sftpClient.DownloadFile('/test/sftp/data/upload/mylib_sftp.py','/home/test/sftp/data/download/mylib_sftp.py')