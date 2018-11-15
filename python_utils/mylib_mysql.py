#-*-coding:utf-8-*-
import os
import sys
import types
import MySQLdb

class MylibMysql:

	"""docstring for MylibMysql"""

	def __init__(self,host,port,user,passwd):
		self.host = host
		self.port = port
		self.user = user
		self.passwd = passwd
		self.db = ''
		self.conn = None

	def __del__(self):
		if self.conn is not None:
			self.conn.close()

	def SetDatabase(self,dbname):
		if type(dbname) is not types.StringType:
			print 'dbname shoul be StringType'
			return 1
		self.db = dbname
		return 0

	def CreateConn(self):
		try:
			self.conn = MySQLdb.connect(host = self.host,
										port = self.port,
										user = self.user,
										passwd = self.passwd,
										db = self.db)
		except MySQLdb.Error, e:
			print 'create mysql connection fail,errmsg:%s' % str(e)
			self.conn = None
			raise

if __name__ == '__main__':
	myMysql = MylibMysql("127.0.0.1",3306,"test","test")
	myMysql.SetDatabase("test")
	myMysql.CreateConn()
	myCur = myMysql.conn.cursor()
	myCur.execute("select * from test.test_table")
	rows = myCur.fetchall()
	for row in rows:
		print row
	myCur.close()
