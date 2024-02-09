// BiDiWaterPump.h

#ifndef _BIDIWATERPUMP_h
#define _BIDIWATERPUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "BlunoShield.h"
#include "WaterPump.h"

class BiDiWaterPump : public WaterPump
{
 protected:
	 bool forwardFlow = true;
	 
	 uint8_t forwardFlowPin;
	 uint8_t reverseFlowPin;

 public:
	BiDiWaterPump(uint8_t pin, 
					int8_t _forwardFlowPin, uint8_t _reverseFlowPin,
					bool _forwardFlow = true,
					uint8_t initialPumpStrength = 255,
					const char* name = "Water Pump",
					unsigned long timerLength = 5 TSECONDS)
		: WaterPump(pin, initialPumpStrength, name, timerLength),
			forwardFlow(_forwardFlow),
			forwardFlowPin(_forwardFlowPin), reverseFlowPin(_reverseFlowPin)
	{
		pinMode(forwardFlowPin, OUTPUT);
		pinMode(reverseFlowPin, OUTPUT);
	}

	virtual uint8_t Initialize()
	{
		return WaterPump::Initialize();
	}

	virtual uint8_t Update()
	{
		if (blunoShield.GetJoystickValue() == eJoy_Right)
			forwardFlow = !forwardFlow;

		digitalWrite(forwardFlowPin, forwardFlow ? HIGH : LOW);
		digitalWrite(reverseFlowPin, forwardFlow ? LOW : HIGH);

		return WaterPump::Update();
	}
};

//extern BiDiWaterPump biDiWaterPump;

#endif

