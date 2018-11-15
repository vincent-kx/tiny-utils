import sys
import types
import time
import mylib_proto
import mylib_net

sys.path.append("../python_libs")
import zmq

class MyZmqClient:
	"""docstring for MyZmqClient"""
	
	sequence = 0

	def __init__(self, strConn = ''):
		self.local_ip = mylib_net.GetLocalEth0IpAddr()
		self.ul_local_ip = mylib_net.GetLongValueOfIp(self.local_ip)
		self.zmq_context = zmq.Context(1)
		self.time_begin= 0
		self.time_end = 0
		self.time_used = 0
		self.connect = strConn
		self.err_msg = ''

	def __del__(self):
		self.zmq_context.destroy()

	def TimeCountBegin(self):
		self.time_begin = time.time()

	def TimeCountEnd(self):
		self.time_end = time.time()
		self.time_used = self.time_end - self.time_begin

	def TimeUsed(self):
		return self.time_used

	def ErrMsg(self):
		return self.err_msg

	def SetConnection(self,strConn):
		if type(strConn) is not types.StringType:
			self.err_msg = 'connect should be string type'
			return 1
		self.connect = strConn
		return 0

	def GetSeq(self):
		MyZmqClient.sequence += 1
		return MyZmqClient.sequence

	def DoZmqReqRsp(self,reqHead,reqBody,rspHead,rspBody,timeout = 1000):
		if type(timeout) is not types.IntType:
			return 1

		self.TimeCountBegin()

		socket = self.zmq_context.socket(zmq.REQ)
		socket.RCVTIMEO = timeout
		socket.SNDTIMEO = timeout
		socket.setsockopt(zmq.LINGER,0)
		socket.connect(self.connect)
		reqPkg = mylib_proto.Encode(reqHead,reqBody)
		if not reqPkg:
			self.err_msg = mylib_proto.ErrMsg()
			return 2
		try:
			socket.send(reqPkg)
			rspPkg = socket.recv()
		except (TypeError,ValueError,zmq.ZMQError),e:
			self.err_msg = str(e)
			return 3
		else:
			ret = mylib_proto.Decode(rspPkg,rspHead,rspBody)
			if ret != 0:
				self.err_msg = mylib_proto.ErrMsg()
				return 4
		finally:
			if not socket.closed:
				socket.close()
		self.TimeCountEnd()
		return 0
