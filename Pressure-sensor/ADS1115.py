# LIBRARY FROM https://gist.github.com/sidwarkd/ec427aefb41e21e77919c7f52df7ea63
import ustruct
import time

_REGISTER_MASK = const(0x03)
_REGISTER_CONVERT = const(0x00)
_REGISTER_CONFIG = const(0x01)
_REGISTER_LOWTHRESH = const(0x02)
_REGISTER_HITHRESH = const(0x03)

_OS_MASK = const(0x8000)
_OS_SINGLE = const(0x8000)  # Write: Set to start a single-conversion
_OS_BUSY = const(0x0000)  # Read: Bit=0 when conversion is in progress
_OS_NOTBUSY = const(0x8000)  # Read: Bit=1 when device is not performing a conversion

_MUX_MASK = const(0x7000)
_MUX_DIFF_0_1 = const(0x0000)  # Differential P  =  AIN0, N  =  AIN1 (default)
_MUX_DIFF_0_3 = const(0x1000)  # Differential P  =  AIN0, N  =  AIN3
_MUX_DIFF_1_3 = const(0x2000)  # Differential P  =  AIN1, N  =  AIN3
_MUX_DIFF_2_3 = const(0x3000)  # Differential P  =  AIN2, N  =  AIN3
_MUX_SINGLE_0 = const(0x4000)  # Single-ended AIN0
_MUX_SINGLE_1 = const(0x5000)  # Single-ended AIN1
_MUX_SINGLE_2 = const(0x6000)  # Single-ended AIN2
_MUX_SINGLE_3 = const(0x7000)  # Single-ended AIN3

_PGA_MASK = const(0x0E00)
PGA_6_144V = const(0x0000)  # +/-6.144V range  =  Gain 2/3
PGA_4_096V = const(0x0200)  # +/-4.096V range  =  Gain 1
PGA_2_048V = const(0x0400)  # +/-2.048V range  =  Gain 2 (default)
PGA_1_024V = const(0x0600)  # +/-1.024V range  =  Gain 4
PGA_0_512V = const(0x0800)  # +/-0.512V range  =  Gain 8
PGA_0_256V = const(0x0A00)  # +/-0.256V range  =  Gain 16

_MODE_MASK = const(0x0100)
_MODE_CONTIN = const(0x0000)  # Continuous conversion mode
_MODE_SINGLE = const(0x0100)  # Power-down single-shot mode (default)

_DR_MASK = const(0x00E0)
_DR_128SPS = const(0x0000)  # 128 samples per second
_DR_250SPS = const(0x0020)  # 250 samples per second
_DR_490SPS = const(0x0040)  # 490 samples per second
_DR_920SPS = const(0x0060)  # 920 samples per second
_DR_1600SPS = const(0x0080)  # 1600 samples per second (default)
_DR_2400SPS = const(0x00A0)  # 2400 samples per second
_DR_3300SPS = const(0x00C0)  # 3300 samples per second

_CMODE_MASK = const(0x0010)
_CMODE_TRAD = const(0x0000)  # Traditional comparator with hysteresis (default)
_CMODE_WINDOW = const(0x0010)  # Window comparator

_CPOL_MASK = const(0x0008)
_CPOL_ACTVLOW = const(0x0000)  # ALERT/RDY pin is low when active (default)
_CPOL_ACTVHI = const(0x0008)  # ALERT/RDY pin is high when active

_CLAT_MASK = const(0x0004)  # Determines if ALERT/RDY pin latches once asserted
_CLAT_NONLAT = const(0x0000)  # Non-latching comparator (default)
_CLAT_LATCH = const(0x0004)  # Latching comparator

_CQUE_MASK = const(0x0003)
_CQUE_1CONV = const(0x0000)  # Assert ALERT/RDY after one conversions
_CQUE_2CONV = const(0x0001)  # Assert ALERT/RDY after two conversions
_CQUE_4CONV = const(0x0002)  # Assert ALERT/RDY after four conversions
_CQUE_NONE = const(0x0003)  # Disable the comparator and put ALERT/RDY in high state (default)

# A mapping of gain values to rail to rail voltage range
_GAINS = {
    PGA_6_144V: 12.288, # 2/3x
    PGA_4_096V: 8.192, # 1x
    PGA_2_048V: 4.096, # 2x
    PGA_1_024V: 2.048, # 4x
    PGA_0_512V: 1.024, # 8x
    PGA_0_256V: 0.512  # 16x
}
_CHANNELS = (_MUX_SINGLE_0, _MUX_SINGLE_1, _MUX_SINGLE_2, _MUX_SINGLE_3)
_DIFFS = {
    (0, 1): _MUX_DIFF_0_1,
    (0, 3): _MUX_DIFF_0_3,
    (1, 3): _MUX_DIFF_1_3,
    (2, 3): _MUX_DIFF_2_3,
}


class ADS1115:
    def __init__(self, i2c, address=0x48, gain=PGA_4_096V):
        self.i2c = i2c
        self.address = address
        if gain in _GAINS:
            self.gain = gain
        else:
            raise ValueError('gain must be one of the values in _GAINS')

    def _write_register(self, register, value):
        data = ustruct.pack('>BH', register, value)
        self.i2c.writeto(self.address, data)

    def _read_register(self, register):
        data = self.i2c.readfrom_mem(self.address, register, 2 )
        return ustruct.unpack('>h', data)[0]

    def read_config(self):
        return self.i2c.readfrom_mem(self.address, 1, 2)

    def get_voltage(self, channel):
        return _GAINS[self.gain] * (self.read(channel) / 65535)

    def read(self, channel):
        self._write_register(_REGISTER_CONFIG, _CQUE_NONE | _CLAT_NONLAT |
            _CPOL_ACTVLOW | _CMODE_TRAD | _DR_1600SPS | _MODE_SINGLE |
            _OS_SINGLE | self.gain | _CHANNELS[channel])
        while not self._read_register(_REGISTER_CONFIG) & _OS_NOTBUSY:
            time.sleep_ms(1)
        return self._read_register(_REGISTER_CONVERT)

    def diff(self, channel1, channel2):
        self._write_register(_REGISTER_CONFIG, _CQUE_NONE | _CLAT_NONLAT |
            _CPOL_ACTVLOW | _CMODE_TRAD | _DR_1600SPS | _MODE_SINGLE |
            _OS_SINGLE | self.gain | _DIFFS[(channel1, channel2)])
        while not self._read_register(_REGISTER_CONFIG) & _OS_NOTBUSY:
            time.sleep_ms(1)
        return self._read_register(_REGISTER_CONVERT)

    def alert_start(self, channel, threshold):
        self._write_register(_REGISTER_HITHRESH, threshold)
        self._write_register(_REGISTER_CONFIG, _CQUE_1CONV | _CLAT_LATCH |
            _CPOL_ACTVLOW | _CMODE_TRAD | _DR_1600SPS | _MODE_CONTIN |
            _MODE_CONTIN | self.gain | _CHANNELS[channel])

    def alert_read(self):
        return self._read_register(_REGISTER_CONVERT)
