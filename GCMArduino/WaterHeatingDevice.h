// WaterHeatingDevice.h

#ifndef _WATERHEATINGDEVICE_h
#define _WATERHEATINGDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"
#include "RangedValue.h"

class TempProbe
{
public:
	int GetTemperature() const
	{
		return 0;
	}
};

class WaterHeatingDevice : public SwitchDevice
{
	static const uint8_t tempRange = 5;

	TempProbe tempProbe;

	RangedValueU16 waterTempRange;

	WaterHeatingDevice();
	WaterHeatingDevice(WaterHeatingDevice&& ctrArg);
	WaterHeatingDevice(const WaterHeatingDevice& ctrArg);

public:
	WaterHeatingDevice(uint8_t pinHeaterRelay, uint8_t pinTempProbe)
		: SwitchDevice(pinHeaterRelay, "Water Heater"),
		waterTempRange("Water Temp", 100, 90, 110)
	{

	}

	virtual uint8_t Update()
	{
		int temp = tempProbe.GetTemperature();

		if (temp < waterTempRange.GetCurrentValue() - tempRange)
			TurnOn();
		else if (temp > waterTempRange.GetCurrentValue() + tempRange)
			TurnOn(false);

		SwitchDevice::Update();
	}

	bool IsWaterReady() const
	{
		if (IsOn())
			return false;

		return tempProbe.GetTemperature() >= waterTempRange.GetCurrentValue();
	}
};

#endif

