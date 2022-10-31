# used to know the Sigfox ID and Sigfox pac
# https://docs.pycom.io/gettingstarted/registration/sigfox/

from network import Sigfox
import binascii

sigfox = Sigfox(mode=Sigfox.SIGFOX, rcz=Sigfox.RCZ1) # init Sigfox for: RCZ1 (Europe), RCZ4 (Australia)
print(binascii.hexlify(sigfox.id())) # print Sigfox Device ID --> b'004d5a47'
print(binascii.hexlify(sigfox.pac())) # print Sigfox PAC number --> b'ccf6eec7df836373'
