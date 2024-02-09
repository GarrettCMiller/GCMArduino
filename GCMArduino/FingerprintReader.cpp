#include "FingerprintReader.h"
#include <Adafruit_Fingerprint.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

uint8_t FingerprintReader::Initialize()
{
	//led.init();

	/*pinInput = defaultInPin;
	pinOutput = defaultOutPin;*/

	//mySerial = new SoftwareSerial(pinInput, pinOutput);
	finger = new Adafruit_Fingerprint(&mySerial);

	// set the data rate for the sensor serial port
	finger->begin(57600);

	if (finger->verifyPassword())
	{
		Serial.println("Found fingerprint sensor!");
	}
	else
	{
		Serial.println("Did not find fingerprint sensor :(");
		//while (1) { delay(1); }
		//led.flashColor(255, 0, 0, 100, 100, 3);
	}
}

///////////////////////////////////////////////////////////////////////////////

uint8_t FingerprintReader::Update()
{
	static int frameCounter = 0;
	frameCounter++;
	Serial.println(frameCounter);

	switch (currentState)
	{
		case eRS_Idle:
			if (0)//frameCounter > 10 /*&& led.rotary.SwitchPressed()*/)
			{
				currentState = eRS_Reading;
				frameCounter = 0;
			}
			break;

		case eRS_Reading:
			getFingerprintID();
			if (frameCounter > 60 /*&& led.rotary.SwitchPressed()*/)
			{
				currentState = eRS_Idle;
				frameCounter = 0;
			}
			delay(50);
			break;
	}

	//led.loop();
}

///////////////////////////////////////////////////////////////////////////////

uint8_t FingerprintReader::getFingerprintID()
{
	uint8_t p = finger->getImage();
	switch (p)
	{
		case FINGERPRINT_OK:
			Serial.println("Image taken");
			//led.flashColor(255, 255, 0, 100, 100, 3);
			break;
		case FINGERPRINT_NOFINGER:
			//Serial.println("No finger detected");
			return p;
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			//led.flashColor(0, 255, 255, LED_RGB::eFL_Medium, 3);
			return p;
		case FINGERPRINT_IMAGEFAIL:
			Serial.println("Imaging error");
			//led.flashColor(255, 0, 255, LED_RGB::eFL_Medium, 3);
			return p;
		default:
			Serial.println("Unknown error");
			//led.flashColor(255, 0, 0, LED_RGB::eFL_Short, 10);
			return p;
	}

	// OK success!
	p = finger->image2Tz();
	switch (p)
	{
		case FINGERPRINT_OK:
			Serial.println("Image converted");
			break;
		case FINGERPRINT_IMAGEMESS:
			Serial.println("Image too messy");
			//led.flashColor(255, 127, 0, LED_RGB::eFL_Medium, 3);
			return p;
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			//led.flashColor(255, 0, 0, LED_RGB::eFL_Medium, 3);
			return p;
		case FINGERPRINT_FEATUREFAIL:
			Serial.println("Could not find fingerprint features");
			//led.flashColor(255, 255, 0, LED_RGB::eFL_Medium, 3);
			return p;
		case FINGERPRINT_INVALIDIMAGE:
			Serial.println("Could not find fingerprint features");
			//led.flashColor(255, 255, 0, LED_RGB::eFL_Medium, 3);
			return p;
		default:
			Serial.println("Unknown error");
			//led.flashColor(255, 0, 0, LED_RGB::eFL_Short, 10);
			return p;
	}

	// OK converted!
	p = finger->fingerFastSearch();
	if (p == FINGERPRINT_OK)
	{
		Serial.println("Found a print match!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		Serial.println("Communication error");
		//led.flashColor(255, 0, 0, LED_RGB::eFL_Medium, 3);
		return p;
	}
	else if (p == FINGERPRINT_NOTFOUND)
	{
		Serial.println("Did not find a match");
		//led.flashColor(255, 90, 0, LED_RGB::eFL_Medium, 5);
		return p;
	}
	else
	{
		Serial.println("Unknown error");
		//led.flashColor(255, 0, 0, LED_RGB::eFL_Short, 10);
		return p;
	}

	//led.flashColor(0, 255, 0, LED_RGB::eFL_Short, 10);

	// found a match!
	Serial.print("Found ID #"); Serial.print(finger->fingerID);
	Serial.print(" with confidence of "); Serial.println(finger->confidence);

	return finger->fingerID;
}

///////////////////////////////////////////////////////////////////////////////

// returns -1 if failed, otherwise returns ID #
int FingerprintReader::getFingerprintIDez()
{
	uint8_t p = finger->getImage();
	if (p != FINGERPRINT_OK)  return -1;

	p = finger->image2Tz();
	if (p != FINGERPRINT_OK)  return -1;

	p = finger->fingerFastSearch();
	if (p != FINGERPRINT_OK)  return -1;

	// found a match!
	Serial.print("Found ID #"); Serial.print(finger->fingerID);
	Serial.print(" with confidence of "); Serial.println(finger->confidence);
	return finger->fingerID;
}