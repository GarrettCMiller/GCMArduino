// LightSensor.h

#ifndef _LIGHTSENSOR_h
#define _LIGHTSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "PinDevice.h"

class LightSensor : public PinDevice
{
 protected:
	 uint16_t lightValue;
	 uint16_t thresholdValue;

 public:
	void init()
	{

	}

	void loop()
	{
		lightValue = 1023 - analogRead(pinNumber);
		//Serial.println(lightValue);
	}

	void setThreshold(uint16_t threshold)
	{
		thresholdValue = threshold;
	}

	uint16_t getThreshold() const
	{
		return thresholdValue;
	}

	uint16_t getValue() const
	{
		return lightValue;
	}

	bool aboveThreshold() const
	{
		return lightValue > thresholdValue;
	}

	bool belowThreshold() const
	{
		return lightValue < thresholdValue;
	}

	float Percent() const
	{
		return lightValue / 1023.0f;
	}

	LightSensor(uint8_t pin, uint16_t threshold = 512)
		: PinDevice(pin), thresholdValue(threshold)
	{
		pinMode(pin, INPUT);
	}
};

//extern LightSensorClass LightSensor;

#endif

