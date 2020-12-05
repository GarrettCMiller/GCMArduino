// FingerprintReader.h

#ifndef _FINGERPRINTREADER_h
#define _FINGERPRINTREADER_h

/***************************************************
This is an example sketch for our optical Fingerprint sensor

Designed specifically to work with the Adafruit BMP085 Breakout
----> http://www.adafruit.com/products/751

These displays use TTL Serial to communicate, 2 pins are required to
interface
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution
****************************************************/
#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
#ifdef Serial1
#define mySerial Serial1
#else
#define mySerial Serial
#endif

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
//#include <SoftwareSerial.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//#include "LED_RGB.h"

class FingerprintReaderClass
{
public:
	enum EReaderState
	{
		eRS_Idle,
		eRS_Reading
	};

 protected:
	 const uint8_t defaultInPin = 2;
	 const uint8_t defaultOutPin = 3;

	 uint8_t pinInput;
	 uint8_t pinOutput;

	 //SoftwareSerial* mySerial/*(2, 3)*/;

	 Adafruit_Fingerprint* finger/* = Adafruit_Fingerprint(&mySerial)*/;

	 //LED_RGB led;

	 uint8_t getFingerprintID();
	 int getFingerprintIDez();
	 
	 EReaderState currentState;

public:
	FingerprintReaderClass()
	{
		pinInput = defaultInPin;
		pinOutput = defaultOutPin;
	}

	FingerprintReaderClass(uint8_t pinIn, uint8_t pinOut)
	{
		pinInput = pinIn;
		pinOutput = pinOut;
	}

	~FingerprintReaderClass()
	{
		//delete mySerial;
		delete finger;
	}

	void StartReading()
	{
		currentState = eRS_Reading;
	}

	bool IsReading() const
	{
		return currentState == eRS_Reading;
	}

	void init();
	void loop();
};

#endif

