// PIRMotionDetector.h

#ifndef _PIRMOTIONDETECTOR_h
#define _PIRMOTIONDETECTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class PIRMotionDetectorClass
{
 //protected:
public:
	uint8_t pinInput;
	bool motionDetected;

 public:
	 PIRMotionDetectorClass()
	 {
		 motionDetected = false;
		 pinInput = -1;
	 }

	 PIRMotionDetectorClass(uint8_t pin)
	 {
		 motionDetected = false;
		 pinInput = pin;
	 }

	int init();
	bool loop();
};

extern PIRMotionDetectorClass PIRMotionDetector;

#endif

