# boot.py -- run on boot-up
from network import Bluetooth
from network import WLAN
import pycom

if pycom.wifi_on_boot() == True:
  pycom.wifi_on_boot(False)

wlan = WLAN()
wlan.deinit()

bluetooth = Bluetooth()
bluetooth.deinit()

pycom.heartbeat(False)  #disable the pycom heartbeat
