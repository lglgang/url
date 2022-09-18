#this is my python code 
#这个是读取编码器的代码，使用树莓派
from ctypes import *
encoder=cdll.LoadLibrary("/home/pi/./ls7366M0.so")
encoder.init_encoder2()
while 1:
    xx=encoder.get_counter2()
    print(xx)
