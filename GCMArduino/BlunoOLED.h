// BlunoOLED.h

#ifndef _BLUNOOLED_h
#define _BLUNOOLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "u8glib.h"
#include "InputCommandListener.h"

class BlunoOLED : public U8GLIB_SSD1306_128X64, public IInputCommandListener
{
public:
	BlunoOLED() : U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE)
	{

	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == "msg") //?
		{
			//flash some sort of message?
		}
	}
};

typedef BlunoOLED OLED;

#endif

