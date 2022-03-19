/*v02: first version with all the sensors (code almost totally rewritten)
 * v02_01 to v02_04: minor changes
 * v02_05: add delay after the reference sensor in the loop to avoid any perturbation to the low-cost sensor (change of current / voltage?)
 * v02_06: new safety (pump not running) + correction for the VL53L0X sensor: working code + limit of 3 m which is much more than the range (something like 1.2m or a little more...) + new experiment duration estimate
 * v02_07: MS5803_1A working code with the correct units + add down/_Up_ to indicate the direction of the pump
 * v03_01: INA219 low-cost sensor added (current sensor for the new pressure sensor ALS) - sensors HC_SR04 and VL53l0x commented for memory limit problems
 * v03_02: Added one blank low-cost measure after the reference measure (that may perturb the low-cost measure) + reduce waiting time after measuring temperature + increase delay at the end of the main loop
 * v03_03: Added INA219 sensor for the ALS pressure sensor
 * v03_04: Added H710B sensor (2x pressure sensors)
 * v04_01: Added ADS1115 sensor (voltage converter) + new offset because systems has been opened (previous 0.150) --> offset=0.143
 */ 
