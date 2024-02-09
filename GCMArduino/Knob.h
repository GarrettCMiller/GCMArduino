/*
The value of this file is read a knob

September-December 2016

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
https://github.com/DFRobot/DFRobot_BlunoAccess
*/
#ifndef _Knob_H
#define _Knob_H
#define knobPin 1
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "ArduinoDevice.h"

class Knob : public ArduinoDevice
{
	int16_t knob = 0, prevknob = -3;
	bool knobChanged = false;
public:
	Knob()
		: ArduinoDevice(knobPin, "Knob")
	{
		//Serial.println("KNOB");
	}

	int readKnob(void);

	uint16_t GetValue()
	{
		return knob;
	}

	virtual uint8_t Update()
	{
		knob = readKnob();
		delay(10);

		if (abs(knob - prevknob) > 1)
		{
			knobChanged = true;
			prevknob = knob;
		}
		else
			knobChanged = false;

		return 0;
	}

	virtual uint8_t Initialize()
	{
		pinMode(knobPin, INPUT);
		return 0;
	}

	bool KnobChanged() const
	{
		return knobChanged;
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == "Knob")
		{
			input.write("Knob", GetValue());
		}
	}
};
#endif