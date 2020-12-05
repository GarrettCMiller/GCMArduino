#include "RotaryEncoder.h"

RotaryEncoderClass* RotaryEncoderClass::global = NULL;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//The interrupt handlers
void ClockChanged()
{
	if (RotaryEncoderClass::global == NULL)
		return;

	RotaryEncoderClass* g = RotaryEncoderClass::global;

	int clkValue = digitalRead(RotaryEncoderClass::pinCLKDefault);//Read the CLK pin level
	int dtValue = digitalRead(RotaryEncoderClass::pinDTDefault);//Read the DT pin level
	if (g->lastCLK != clkValue)
	{
		g->lastCLK = clkValue;
		int val = (clkValue != dtValue ? 1 : -1);
		
		g->value += (g->decreaseToLeft ? -val : val);//CLK and inconsistent DT + 1, otherwise - 1

		g->Clamp();
		Serial.print("count:");
		Serial.println(RotaryEncoderClass::global->value);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void RotaryEncoderClass::init()
{
	value = defaultValue;

	pinMode(pinSW, INPUT);
	digitalWrite(pinSW, HIGH);
	pinMode(pinCLK, INPUT);
	pinMode(pinDT, INPUT);
	attachInterrupt(interrupt, ClockChanged, CHANGE);//Set the interrupt 0 handler, trigger level change

	global = this;
}

///////////////////////////////////////////////////////////////////////////////

void RotaryEncoderClass::loop()
{
	if (SwitchPressed() && value != defaultValue) //Read the button press and the count value to 0 when the counter reset
	{
		value = defaultValue;
		Serial.print("count:");
		Serial.println(value);
	}
}

bool RotaryEncoderClass::SwitchPressed()
{
	return !digitalRead(pinSW);
}

///////////////////////////////////////////////////////////////////////////////
