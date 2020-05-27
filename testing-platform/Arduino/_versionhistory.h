/*v02: first version with all the sensors (code almost totally rewritten)
 * v02_01 to v02_04: minor changes
 * v02_05: add delay after the reference sensor in the loop to avoid any perturbation to the low-cost sensor (change of current / voltage?)
 * v02_06: new safety (pump not running) + correction for the VL53L0X sensor: working code + limit of 3 m which is much more than the range (something like 1.2m or a little more...) + new experiment duration estimate
 * v02_07: MS5803_1A working code with the correct units + add down/_Up_ to indicate the direction of the pump
 */ 
