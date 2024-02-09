// LEDToggleSwitch.h

#ifndef _LEDTOGGLESWITCH_h
#define _LEDTOGGLESWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ToggleSwitch.h"
#include "LED_RGB.h"

class LEDToggleSwitch : public ToggleSwitch
{
 protected:
	LED led;

 public:
	LEDToggleSwitch(uint8_t pinSwitch, uint8_t pinLED, String name = "LEDToggle")
		: ToggleSwitch(pinSwitch, name), led(pinLED)
	{

	}

	virtual uint8_t Initialize()
	{
		led.Initialize();
		return ToggleSwitch::Initialize();
	}

	virtual uint8_t Update()
	{
		led.Update();
		return ToggleSwitch::Update();
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		ToggleSwitch::ProcessInput(input);
	}

	void TurnLEDOn(bool on = true)
	{
		if (on)
			led.TurnOn();
		else
			led.TurnOff();
	}
};

//extern LEDToggleSwitch ledToggleSwitch;

#endif

