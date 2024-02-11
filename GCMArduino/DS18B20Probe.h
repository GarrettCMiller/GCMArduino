// DS18B20Probe.h

#ifndef _DS18B20PROBE_h
#define _DS18B20PROBE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <DallasTemperature.h>
#include <OneWire.h>

#include "TemperatureSensor.h"

#define ONE_WIRE_BUS	53

class DS18B20Probe : public ITemperatureSensor
{
public:
	//static const uint8_t OneWireBus = 4;
	static void UpdateSensors()
	{
		sensors.requestTemperatures();
	}

protected:
	static OneWire oneWire;
	static DallasTemperature sensors;

	static bool sensorsBegun;

	static void BeginSensors()
	{
		if (!sensorsBegun)
		{
			Serial.print(F("Beginning DS18B20 sensors..."));
			sensors.begin();
			sensorsBegun = true;
		}
	}

	uint8_t sensorIndex;

public:
	DS18B20Probe(uint8_t _sensorIndex = 0, String name = "")
		: ITemperatureSensor(name), sensorIndex(_sensorIndex)
	{
		if (name == "")
			SetName(F("DS18B20"));
	}

	virtual uint8_t Initialize()
	{
		BeginSensors();

		return 0;
	}

	virtual uint8_t Update()
	{
		

		return 0;
	}

	float GetTemperature() /*const*/
	{
		//sensors.alarm
		return sensors.getTempFByIndex(sensorIndex);
	}

	virtual void ProcessInput(PlainProtocol& pp)
	{

	}
};

//extern DS18B20Probe DS18B20Probe;

#endif

