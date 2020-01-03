from ctypes import *

library = cdll.LoadLibrary("ctools.dll");
library.init()

library.get_reply.argtype = [c_char_p, c_char_p]
library.get_reply.restype = c_void_p
reply = (c_char * 1024)()
str = '中文'
library.get_reply(str.encode('utf8'), reply)
print("out:", reply.value.decode('utf8'))