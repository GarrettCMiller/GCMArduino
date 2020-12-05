// PinDevice.h

#ifndef _PINDEVICE_h
#define _PINDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class PinDevice
{
protected:
	uint8_t pinNumber;
	bool input;
	bool isOn;

public:
	PinDevice(uint8_t pin, bool _input = false) : pinNumber(pin), input(_input), isOn(false)
	{
		pinMode(pinNumber, input ? INPUT : OUTPUT);
		if (input)
			isOn = PinIsOn();
	}

	/*virtual bool IsOn() const
	{
		return isOn;
	}*/

	uint8_t GetPinNumber() const
	{
		return pinNumber;
	}

	bool PinIsOn() const
	{
		if (input)
			return digitalRead(pinNumber) == HIGH;

		return isOn;
	}
};

#endif

