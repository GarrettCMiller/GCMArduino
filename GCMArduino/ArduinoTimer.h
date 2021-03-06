// TimedDevice.h

#ifndef _TIMEDDEVICE_h
#define _TIMEDDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//ALL measured in ms (milliseconds, 1/1000th of a second)
#define TSECONDS	* 1000
#define TMINUTES	* 60 TSECONDS
#define THOURS		* 60 TMINUTES
#define TDAYS		* 24 THOURS

//template <_Type>
class ArduinoTimer
{
 public:
	 unsigned long timerLength;
	 unsigned long timerStart_ms;
	 unsigned long defaultTimerLength;

	 bool enabled;
	 bool autoResetOnReady;

 public:
	 ArduinoTimer(unsigned long time = 1000, bool runNow = false) : autoResetOnReady(true)
	 {
		 SetDefaultTimerLength(time);
		 enabled = runNow;
		 if (enabled)
			 Reset();
		 else
			timerStart_ms = 0;
	 }

	 void SetTimerLength(unsigned long length, bool resetTimer = true)
	 {
		 timerLength = length;
		 if (resetTimer)
			 Reset();
		 else
			 enabled = false;
	 }

	 bool IsReady()
	 {
		 if (!enabled)
			 return false;

		 bool isReady = (millis() >= timerStart_ms + timerLength);
		 if (autoResetOnReady && isReady)
			 Reset();
		 return isReady;
	 }

	 unsigned long GetTimeRunning() const
	 {
		 return millis() - timerStart_ms;
	 }

	 unsigned long GetNextTime() const
	 {
		 return timerStart_ms + timerLength;
	 }

	 void SetDefaultTimerLength(unsigned long length)
	 {
		 defaultTimerLength = length;
		 SetTimerLength(length);
	 }

	 void Reset()
	 {
		 timerStart_ms = millis();
		 enabled = true;
	 }
};

#endif

