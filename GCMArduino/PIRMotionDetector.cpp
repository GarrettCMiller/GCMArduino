// 
// 
// 

#include "PIRMotionDetector.h"

int PIRMotionDetectorClass::init()
{
	if (pinInput == -1)
	{
		Serial.println("Invalid pin number for PIR");
		return -1;
	}

	pinMode(pinInput, INPUT_PULLUP);

	return 0;
}

bool PIRMotionDetectorClass::loop()
{
	static int frameCounter = 0;
	if (digitalRead(pinInput) == LOW)
	{
		if (!motionDetected)
		{
			motionDetected = true;
			Serial.println("Motion detected!");
		}
	}
	else
	{
		if (motionDetected)
		{
			Serial.println("Motion detector resetting...");
			motionDetected = false;
		}
	}

	return motionDetected;
}