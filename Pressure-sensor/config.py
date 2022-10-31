########################
# CONFIG FILE !!!!
########################
# PARAMETERS
loraok= False # Send data through Lora ?
sigfoxok=True # Send data through Sigfox ?
needsleep= True # Enable deep sleep
powersaving= True # Optimize the code for power saving
nsensors=2 #number of probes, min=1, max=3
withRTC=False #True if there is a RTC clock
sdmandatory=False #True if there is a SD card to save the data
batcoeff=2.08 # Realvoltage=batcoeff * measurevoltage (was 1.98)
sleeptime=2 #sleep time in minutes, default=10mn (must be an integer)

# SIGFOX CONFIGURATION
# ==> device needs to be registered on SIGFOX !!!
# cf. sigfoxinit.py

# LORAWAN create an ABP authentication params to secure data
TTNregion="EU868" # "AU915" or "AS923" or "Special" (Special for the mono frequency choosen below)
# YOU NEED TO CREATE AN APP AND A DEVICE (ABP) ON TTN FIRST:
#for the ads1115multiproberj ttn app:
dev_addr='26011E9C'
nwk_swkey='7BB07A03D70812905BCC1C092FA84278'
app_swkey='284C2EDC9FCD7F53C134AAC329D0BD3B'

frequency = 868100000 #use for the "Special" Lora
sf = 7 #spreading factor (integer between 7 and 12)
dr = 5 #Data Rate

# PINs configuration
relay = 'P19' #P19 coonected to the nano timer relay done pin
sclPin='P22' # RTC SCL PIN (or OLED SCL PIN)
sdaPin='P23' # RTC SDA PIN (or OLED SCL PIN)

# led colors
OFF = 0x000000
WHITE=0xFFFFCC
YELLOW = 0x7f7f00
ORANGE = 0xFF9900
RED = 0xff0000
PURPLE = 0x7f007f
GREEN = 0x00ff00
BLUE = 0x0000ff
