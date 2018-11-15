import os
import sys
from zipfile import *
import zipfile

errmsg = ''

def Zip(filesToZip,filename):
	global errmsg
	try:
		myZipFile = zipfile.ZipFile(filename,'w',zipfile.ZIP_STORED)
	except (zipfile.BadZipfile,zipfile.LargeZipFile), e:
		errmsg = str(e)
		return 1
	try:
		for file in filesToZip:
			myZipFile.write(file,os.path.basename(file))
	except (zipfile.BadZipfile,zipfile.LargeZipFile), e:
		errmsg = str(e)
		return 2
	finally:
		myZipFile.close()
	return 0
		

def Unzip(fileToUnzip,path):
	global errmsg
	if not os.path.isfile(fileToUnzip):
		errmsg = '%s is not exist' % fileToUnzip
		return 1
	if not zipfile.is_zipfile(fileToUnzip):
		errmsg = '%s is not zip file' % fileToUnzip
		return 2
	try:
		myZipFile = zipfile.ZipFile(fileToUnzip,'r',zipfile.ZIP_STORED)
	except (zipfile.BadZipfile,zipfile.LargeZipFile), e:
		errmsg = str(e)
		return 3

	try:
		myZipFile.extractall(path)
	except (zipfile.BadZipfile,zipfile.LargeZipFile), e:
		errmsg = str(e)
		return 4
	finally:
		myZipFile.close()
		
	return 0

def ErrMsg():
	global errmsg
	return errmsg
# def Zip(filesToZip,filename):
# 	try:
# 		with zipfile.ZipFile(filename,'w',zipfile.ZIP_STORED) as myZipFile:
# 			for file in filesToZip:
# 				print 'ziping file:%s' % file
# 				myZipFile.write(file,os.path.basename(file))
# 		print 'success'
# 	except (zipfile.BadZipfile,zipfile.LargeZipFile), e:
# 		print '(zipfile.BadZipfile,zipfile.LargeZipFile) occured!!!'
# 		print str(e)
# 		raise
		

# def Unzip(fileToUnzip,path):
# 	if not os.path.isfile(fileToUnzip):
# 		print '%s is not exist' % fileToUnzip
# 		return 1
# 	if not zipfile.is_zipfile(fileToUnzip):
# 		print '%s is not zip file' % fileToUnzip
# 		return 2
# 	try:
# 		with zipfile.ZipFile(fileToUnzip,'r',zipfile.ZIP_STORED) as myZipFile:
# 			myZipFile.extractall(path)
# 			return 0
# 	except (zipfile.BadZipfile,zipfile.LargeZipFile), e:
# 		print str(e)
# 		raise
# 	return 0