// RotaryEncoder.h

#ifndef _ROTARYENCODER_h
#define _ROTARYENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class RotaryEncoderClass
{
public:
	static RotaryEncoderClass* global;

	static const int pinCLKDefault = 2;
	static const int pinDTDefault = 3;
	static const int pinSWDefault = 4;
	static const int interruptDefault = 0;
	static const int defaultValueDefault = 100;

protected:

	uint8_t pinCLK;
	uint8_t pinDT;
	uint8_t pinSW;
	uint8_t interrupt;

public:
	bool decreaseToLeft;
	
	bool hasMinimum;
	int minimumValue;
	bool hasMaximum;
	int maximumValue;
	
	int lastCLK;
	int value;
	int defaultValue;

public:
	RotaryEncoderClass(uint8_t _pinCLK = pinCLKDefault, uint8_t _pinDT = pinDTDefault, uint8_t _pinSW = pinSWDefault,
			int _defaultValue = defaultValueDefault, int _interrupt = interruptDefault)
	{
		pinSW = _pinSW;
		pinCLK = _pinCLK;
		pinDT = _pinDT;
		interrupt = _interrupt;
		defaultValue = _defaultValue;
		
		decreaseToLeft = true;
		hasMaximum = true;
		hasMinimum = true;
		minimumValue = 0;
		maximumValue = 100;
	}

	void init();
	void loop();

	bool SwitchPressed();

	void Clamp()
	{
		if (hasMinimum && value < minimumValue)
			value = minimumValue;
		
		if (hasMaximum && value > maximumValue)
			value = maximumValue;
	}
};

#endif

