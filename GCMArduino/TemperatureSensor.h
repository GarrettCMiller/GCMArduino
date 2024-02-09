// TemperatureSensor.h

#ifndef _TEMPERATURESENSOR_h
#define _TEMPERATURESENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ArduinoDevice.h"

/*extern*/ float c2f(float tempC);

/// <summary>
/// A device that can measure temperature
/// </summary>
class ITemperatureSensor : public ArduinoDevice
{
 public:
	 ITemperatureSensor(String name) : ArduinoDevice(name)
	 {

	 }

	 virtual float GetTemperature() /*const*/ = 0;
};

//extern TemperatureSensor TemperatureSensor;

#endif

