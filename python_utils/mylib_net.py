#-*-coding:utf-8-*-
import socket
import fcntl
import struct

errmsg = ''
def GetIpAddr(ifname):
	global errmsg
	ipAddr = ''
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		ipAddr = socket.inet_ntoa(fcntl.ioctl(
	    	s.fileno(),
	    	0x8915,  # SIOCGIFADDR
	    	struct.pack('256s', ifname[:15])
		)[20:24])
	except (socket.error,IOError,struct.error),e:
		errmsg = str(e)
		return None
	return ipAddr

def GetLocalEth0IpAddr():
	return GetIpAddr('eth0')

def GetLongValueOfIp(strIp):
	longIpValue = 0
	try:
		longIpValue = socket.ntohl(struct.unpack("I",socket.inet_aton(strIp))[0])
	except (IOError,struct.error),e:
		return 0
	return longIpValue

def Errmsg():
	global errmsg
	return errmsg

if __name__ == '__main__':
	print GetLocalEth0IpAddr()
	print GetIpAddr("eth2")

	print GetLongValueOfIp("111.222.211.221")
	print GetLongValueOfIp("a.b")
	print Errmsg()
