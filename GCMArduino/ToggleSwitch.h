// ToggleSwitch.h

#ifndef _TOGGLESWITCH_h
#define _TOGGLESWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ArduinoDevice.h"

class ToggleSwitch : public ArduinoDevice
{
 protected:


 public:
	ToggleSwitch(uint8_t pin, String name = "Toggle")
		: ArduinoDevice(pin, name)
	{
	}

	virtual uint8_t Initialize()
	{
		return ArduinoDevice::Initialize();
	}

	virtual uint8_t Update()
	{
		TurnOn(digitalRead(pinNumber) == HIGH);
		return ArduinoDevice::Update();
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		ArduinoDevice::ProcessInput(input);
	}
};

//extern ToggleSwitch toggleSwitch;

#endif

