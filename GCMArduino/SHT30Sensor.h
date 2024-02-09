// SHT30Sensor.h

#ifndef _SHT30SENSOR_h
#define _SHT30SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Adafruit_SHT31.h"
#include "TemperatureSensor.h"

class SHT30Sensor : public Adafruit_SHT31, public ITemperatureSensor
{
 protected:


 public:
	SHT30Sensor()
		: ITemperatureSensor("SHT30"), Adafruit_SHT31()
	{
	}

	virtual uint8_t Initialize()
	{
		if (begin(0x44))
			return 0;
		Serial.print(F("Couldn't find SHT30!"));
		return -1;
	}

	virtual uint8_t Update()
	{
		return 0;
	}

	virtual void ProcessInput(PlainProtocol& pp)
	{

	}

	virtual bool IsInternalHeaterEnabled() /*const*/
	{
		return isHeaterEnabled();
	}

	virtual float GetTemperature() /*const*/
	{
		return c2f(readTemperature());
	}

	virtual float GetHumidity() /*const*/
	{
		return readHumidity();
	}

	virtual void EnableInternalHeater(bool enabled)
	{
		heater(enabled);
	}
};

//extern SHT30Sensor sHT30Sensor;

#endif

