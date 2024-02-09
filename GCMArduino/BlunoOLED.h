// BlunoOLED.h

#ifndef _BLUNOOLED_h
#define _BLUNOOLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef USE_U8G2
#include "U8g2_for_Adafruit_GFX.h"
#else
#include "u8glib.h"
#endif

#include "ArduinoDevice.h"

#define RES 6    //LED reset pin PIN6
#define DC 7     //LED DC pin PIN3

class BlunoOLED : public 
#ifdef USE_U8G2
	U8G2_FOR_ADAFRUIT_GFX
#else
	U8GLIB_SSD1306_128X64
#endif
	, public ArduinoDevice
{
public:
	BlunoOLED() :
#ifdef USE_U8G2
		U8G2_FOR_ADAFRUIT_GFX()
#else
		U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE)
#endif
		, ArduinoDevice("OLED")
	{
		//Serial.println("OLED");
	}

	virtual uint8_t Initialize()
	{
		Serial.println(F("\tInitializing on-board OLED screen..."));

		//////////////////////////////////////////////////////////////
		// The following block of code was part of the original
		// BlunoAccessoryShield library and is the only code I've
		// found that ACTUALLY gets the OLED to work. Believe me,
		// I went through tons of iterations and variations of code
		// based on the (multiple) original code base(s).
		pinMode(DC, OUTPUT);
		digitalWrite(DC, LOW);             //DC=0
		pinMode(RES, OUTPUT);
		digitalWrite(RES, HIGH);   delay(100);
		digitalWrite(RES, LOW);    delay(100);
		digitalWrite(RES, HIGH);   delay(100);
		// END ABOVE "QUOTED" BLOCK OF CODE (Can you really put quotes around the word quoted?)
		//////////////////////////////////////////////////////////////
		
		//setI2CAddress(0x7a);//0x3d x 2
		
		//Attempt to begin the device
		if (begin() != 0)
		{
			setColorIndex(1);				//set color mode to binary/black-and-white/monochrome
			//drawRFrame(0, 0, 127, 34, 5);	//for now, keep to see if I need
			
			Serial.println(F("\tSuccessfully initialized OLED screen!"));
			
			return 0;
		}
		
		Serial.println(F("! ! ! ! ! ! !*****Failed to initialize OLED screen!"));

		return -1;
	}

	virtual uint8_t Update()
	{
		return 0;
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == F("msg")) //?
		{
			//flash some sort of message?
		}
	}
};

typedef BlunoOLED OLED;

#endif

