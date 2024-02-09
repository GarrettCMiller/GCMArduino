// 
// 
// 

#include "DS18B20Probe.h"

OneWire DS18B20Probe::oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20Probe::sensors(&DS18B20Probe::oneWire);
bool DS18B20Probe::sensorsBegun = false;

//void DS18B20Probe::init()
//{
//
//
//}
//
//
//DS18B20Probe DS18B20Probe;

