########################
# CONFIG FILE !!!!
########################
# PARAMETERS
nb_samples= 25 # Number of samples for the advanced measure of the HC-SR04
loraok= True # Send data through Lora ?
sigfoxok=False # Send data through Sigfox ?
needsleep= True # Enable deep sleep
powersaving= True # Optimize the code for power saving
isDHT11=False # True for DTH11 and False for DTH22
batcoeff=1.98 # Realvoltage=batcoeff * measurevoltage

# SIGFOX CONFIGURATION
# ==> device needs to be registered on SIGFOX !!!
# cf. sigfox website

# LORAWAN create an ABP authentication params to secure datas
TTNregion="AS923" # "AU915" or "AS923"
# device name: 'Saturday Night Fever'
dev_addr='???????'
nwk_swkey='????????????????????????????????'
app_swkey='????????????????????????????????'
sf = 7 #spreading factor (integer between 7 and 12)
dr = 5 #Data Rate

# PINs
DHT = 'P23' # old pin 'P10'
relay = 'P22' #old pin 'P9'
echo  = 'P20'
trig  = 'P19'
voltmeter='P16'

# led colors
OFF = 0x000000
WHITE=0xFFFFCC
RED = 0xff0000
BLUE = 0x0000ff
GREEN = 0x00ff00
YELLOW = 0x7f7f00
PURPLE = 0x7f007f
ORANGE = 0xFF9900
