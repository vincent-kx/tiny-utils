# -*- coding: UTF-8 -*-
import time
import datetime
import json

holiday_config = '{"2016-1":[1,2,3],"2016-2":[7,8,9,10,11,12,13],"2016-4":[3,4,5],"2016-5":[1,2,3],"2016-6":[9,10,11],"2016-9":[15,16,17],"2016-10":[1,2,3,4,5,6,7]}'
workday_config = '{}'
[MONDAY,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY,SUNDAY] = range(1,8)

class MylibDate:
	"""docstring for MylibDate"""

	def Init(self):
		self.holidaysDict = json.loads(holiday_config);
		self.workdaysDict = json.loads(workday_config);
		self.nowDateTime = datetime.datetime.now()
		self.todayDate = self.nowDateTime.date()

	def IsWorkingDay(self,someDateTime):
		someDate = someDateTime.date()
		strKey = "%d-%d" % (someDate.year,someDate.month)
		#如果是法定节假日
		if self.holidaysDict.has_key(strKey) and (someDate.day in self.holidaysDict[strKey]):
			print "law holiday"
			return False #节假日
		if someDateTime.isoweekday() > FRIDAY:#如果是周六周日
			#如果是调休
			if self.workdaysDict.has_key(strKey) and (someDateTime.day in self.workdaysDict[strKey]):
				print 'adjust working day'
				return True
			else:#正常周末
				print 'normal weekend'
				return False
		print 'normal working day'
		return True

	def NextNumWorkingDay(self,someDateTime,num):
		tmpDateTime = someDateTime
		while num > 0:
			#从someDateTime开始算起，如果someDateTime是工作日则someDateTime为计算中的第一个工作日
			if self.IsWorkingDay(tmpDateTime):
				num -= 1
			if num != 0:
				tmpDateTime = tmpDateTime + datetime.timedelta(days = 1)
		return tmpDateTime

	def PreNumWorkingDayNoToday(self,someDateTime,num):
		tmpDateTime = someDateTime - datetime.timedelta(days = 1)
		while num > 0:
			if self.IsWorkingDay(tmpDateTime):
				num -= 1
			if num != 0:
				tmpDateTime = tmpDateTime - datetime.timedelta(days = 1)
		return tmpDateTime

	def GetLastWorkingDay(self):
		return self.PreNumWorkingDayNoToday(self.nowDateTime,1)

	def GetNextWorkingDay(self):
		if self.IsWorkingDay(self.nowDateTime):
			return self.NextNumWorkingDay(self.nowDateTime,2)
		else:
			return self.NextNumWorkingDay(self.nowDateTime,1)

	def GetLastWorkingDay15PmStr(self):
		lastWorkingDay = self.GetLastWorkingDay()
		return lastWorkingDay.date().isoformat() + " 15:00:00"

	def GetToday15PmStr(self):
		return datetime.datetime.now().date().isoformat() + " 15:00:00"

if __name__ == '__main__':
	mylibDate = MylibDate()
	mylibDate.Init()

	nowDateTime = datetime.datetime.now()#今天周三
	# print mylibDate.NextNumWorkingDay(nowDateTime,1)
	# print mylibDate.NextNumWorkingDay(nowDateTime,2)
	# print mylibDate.NextNumWorkingDay(nowDateTime,3)
	# print mylibDate.NextNumWorkingDay(nowDateTime,4)
	# print mylibDate.NextNumWorkingDay(nowDateTime,5)
	# print mylibDate.NextNumWorkingDay(nowDateTime,6)
	# print mylibDate.NextNumWorkingDay(nowDateTime,7)
	# print mylibDate.NextNumWorkingDay(nowDateTime,8)
	# print mylibDate.NextNumWorkingDay(nowDateTime,9)

	# nowDateTime = nowDateTime + datetime.timedelta(days = 3) #周6
	# print mylibDate.NextNumWorkingDay(nowDateTime,1)
	# print mylibDate.NextNumWorkingDay(nowDateTime,2)

	# nowDateTime = datetime.datetime.now()
	# print mylibDate.PreNumWorkingDayNoToday(nowDateTime,1)
	# print mylibDate.PreNumWorkingDayNoToday(nowDateTime,2)
	# print mylibDate.PreNumWorkingDayNoToday(nowDateTime,3)

	nowDateTime = nowDateTime + datetime.timedelta(days = 4) #周日
	print mylibDate.PreNumWorkingDayNoToday(nowDateTime,1)





