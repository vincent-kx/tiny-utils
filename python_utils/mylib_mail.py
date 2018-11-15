# -*- coding: UTF-8 -*-
import sys
import types
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.header import Header
from email.utils import parseaddr, formataddr


class MylibMail:
	"""docstring for MylibMail"""
	MAIL_CHARSET = 'utf-8'
	def __init__(self):
		self.user = '123456789@qq.com'
		self.passwd = '12345678'
		self.receivers = []
		self.from_user = '测试用户'
		self.to_user = []
		self.timeout = 10000
		self.mail_host = 'smtp.mail.qq.com:465'
		self.subject = ''
		self.errmsg = ''

	def SetReceivers(self,recvs):
		self.receivers = recvs

	def SetFromUser(self,fromUser):
		self.from_user = fromUser

	def SetToUser(self,toUser):
		self.to_user = toUser

	def SetUser(self,user):
		self.user = user

	def SetPasswd(self,passwd):
		self.passwd = passwd

	def SetTimeOut(self,timeout):
		self.timeout = timeout

	def SetEmailSubject(self,subject):
		self.subject = subject

	def SetMailHost(self,mailHost):
		self.mail_host = mailHost

	def ErrMsg(self):
		return self.errmsg

	def SendEmail(self,content,fun = 'plain'):
		if type(content) is not types.StringType and type(content) is not types.UnicodeType:
			self.errmsg = 'content shuold be plain text'
			return 1
		self.msg = MIMEMultipart()
		self.message = MIMEText(content,fun,MylibMail.MAIL_CHARSET)
		self.msg['From'] = Header(self.from_user)
		self.msg['To'] = Header('; '.join(["<%s>" % x for x in self.to_user]))
		self.msg['Subject'] = Header(self.subject,MylibMail.MAIL_CHARSET)
		self.msg.attach(self.message)
		smtpObj = smtplib.SMTP_SSL()
		smtpObj.timeout = self.timeout
		smtpObj.connect(self.mail_host)
		try:
			smtpObj.verify(self.mail_host)
			#smtpObj.set_debuglevel(1) 
			print '登录服务器 ...'
			smtpObj.login(self.user,self.passwd)
			print '邮件发送中...'
			smtpObj.sendmail(self.user, self.receivers, self.msg.as_string())
			print "邮件发送成功"
			
		except smtplib.SMTPException,e:
			self.errmsg = str(e)
			return 1
		finally:
			smtpObj.quit()
		return 0

if __name__ == '__main__':
	emailSender = MylibMail()
	emailSender.SetUser('123456789@qq.com')
	emailSender.SetPasswd('1234567')
	emailSender.SetTimeOut(5)
	emailSender.SetMailHost('smtp.mail.qq.com:465')
	emailSender.SetFromUser('test_user')
	emailSender.SetToUser('test_rev')
	emailSender.SetReceivers(['1234@qq.com','2345@qq.com'])
	emailSender.SetEmailSubject('测试邮件')
	content = '这是用于测试所发的邮件，\n请忽略，谢谢！'
	emailSender.SendPlainTextEmail(content)
