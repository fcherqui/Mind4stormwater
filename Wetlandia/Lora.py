import config
import utime
from network import LoRa
import socket
import binascii
import struct

class _LoraAU915: #Australia AU915
    def setupLora():
        utime.sleep_ms(5)

        #thethingsnetwork parameters
        dev_addr = struct.unpack(">l", binascii.unhexlify(config.dev_addr))[0]
        nwk_swkey = binascii.unhexlify(config.nwk_swkey)
        app_swkey = binascii.unhexlify(config.app_swkey)

        #initialize LoRa in LORAWAN mode for Australia AU915
        lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.AU915)

        # remove all the non-default channels and leave channels 8-15 and 65
        for index in range(0, 8):
           lora.remove_channel(index)  # remove 0-7
        for index in range(16, 65):
           lora.remove_channel(index)  # remove 16-64
        for index in range(66, 72):
          lora.remove_channel(index)   # remove 66-71

        # join a network using ABP (Activation By Personalization)
        lora.join(activation=LoRa.ABP, auth=(dev_addr,nwk_swkey,app_swkey))

        # create a LoRa socke
        s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

        # make the socket non-blocking
        s.setblocking(False)

        #print("LoRa connexion set up")
        utime.sleep_ms(5)
        return s

class _LoraAS923: #Asia - Australia AS923
    def setupLora():
        utime.sleep_ms(5)

        #thethingsnetwork parameters
        dev_addr = struct.unpack(">l", binascii.unhexlify(config.dev_addr))[0]
        nwk_swkey = binascii.unhexlify(config.nwk_swkey)
        app_swkey = binascii.unhexlify(config.app_swkey)

        #initialize LoRa in LORAWAN mode for Asia AS923
        lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.AS923)

        # join a network using ABP (Activation By Personalization)
        lora.join(activation=LoRa.ABP, auth=(dev_addr,nwk_swkey,app_swkey))

        # create a LoRa socke
        s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

        # make the socket non-blocking
        s.setblocking(False)

        # print("LoRa connexion set up")
        utime.sleep_ms(5)
        return s

    def testLora():
        _Lora.setupLora()
        s.send('testLora')
        print("test sent through LoRa")
        utime.sleep(5) #wait to prevent socket from overloading
