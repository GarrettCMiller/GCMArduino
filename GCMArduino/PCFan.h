// PCFan.h

#ifndef _PCFAN_h
#define _PCFAN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"

class PCFan : public SwitchDevice
{
 protected:


 public:
	 PCFan(uint8_t _pin, const char* _name,
			unsigned long _timerLength = 30 TMINUTES, bool _isPWM = false)
		: SwitchDevice(_pin, _name, _timerLength)
	{
		 SetPWM(_isPWM);
	}

	void SetFanSpeed(uint8_t speed)
	{
		SetPWMStrength(speed);
	}

	uint8_t GetFanSpeed() const
	{
		return GetPWMStrength();
	}

	virtual uint8_t Initialize()
	{
		return SwitchDevice::Initialize();
	}

	virtual uint8_t Update()
	{
		return SwitchDevice::Update();
	}

	virtual void ProcessInput(PlainProtocol& input)
	{

	}
};

//extern PCFan PCFan;

#endif

