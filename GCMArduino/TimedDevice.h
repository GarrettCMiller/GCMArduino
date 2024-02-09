// TimedDevice.h

#ifndef _TIMEDDEVICE_h
#define _TIMEDDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define TSECONDS	* 1000
#define TMINUTES	* 60 TSECONDS
#define THOURS		* 60 TMINUTES
#define TDAYS		* 24 THOURS

//template <_Type>
class TimedDeviceClass
{
 public:
	 unsigned long timerLength;
	 unsigned long timer;
	 unsigned long defaultTimerLength;

	 bool runFirstTime;

 public:
	 TimedDeviceClass(unsigned long time = 1000, bool runNow = false)
	 {
		 SetDefaultTimerLength(time);
		 runFirstTime = runNow;
		 if (runFirstTime)
			 timer = millis() + timerLength;
		 else
			timer = 0;
	 }

	 void SetTimerLength(unsigned long length)
	 {
		 timerLength = length;
		 Reset();
	 }

	 bool IsReady() const
	 {
		 return millis() >= timer + timerLength;
	 }

	 unsigned long GetTime() const
	 {
		 return timer + timerLength;
	 }

	 void SetDefaultTimerLength(unsigned long length)
	 {
		 defaultTimerLength = length;
		 SetTimerLength(length);
	 }

	 void Reset()
	 {
		 timer = millis();
	 }
};

#endif

