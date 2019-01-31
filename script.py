
# import module exposed from busmaster simulated node
import busmaster

import isotp


busmaster.Trace("Message from python")

with open("abc_from_py.txt", 'w') as asd:
    asd.write("Hello world from embedded python")

from time import time,ctime

busmaster.Trace('Today is ' + ctime(time()) )

try:
    isotp.SendResponse(EcuId = 0, data = bytes([0x10, 0x20]), DataLen = 2)
except:
    busmaster.Trace("exception while calling isotp.SendResponse(0, bytes([10, 20]), 2) ")