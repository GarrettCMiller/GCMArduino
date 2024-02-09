// WaterPump.h

#ifndef _WATERPUMP_h
#define _WATERPUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"

class WaterPump : public SwitchDevice
{
 protected:
	 ArduinoTimer offTimer;

 public:
	 WaterPump(uint8_t pin, uint8_t initialPumpStrength = 255,
				const char* name = "Water Pump",
				unsigned long timerLength = 5 TSECONDS)
		: SwitchDevice(pin, name, timerLength)//, pumpStrength(initialPumpStrength)
	{
		 SetPWM();
		 SetPWMStrength(initialPumpStrength);
	}

	virtual uint8_t Initialize()
	{
		return SwitchDevice::Initialize();
	}

	virtual uint8_t Update()
	{
		return SwitchDevice::Update();
	}
};

//extern WaterPump WaterPump;

#endif

