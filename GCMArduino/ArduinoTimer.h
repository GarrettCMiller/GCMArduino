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

#define TWEEKS		* 7 TDAYS

#define TMONTHS_D28	* 28 TDAYS
#define TMONTHS_D29	* 29 TDAYS
#define TMONTHS_D30	* 30 TDAYS
#define TMONTHS_D31	* 31 TDAYS

#define TMONTHS_W	* 4 TWEEKS


/// <summary>
/// A basic timer for Arduino based on millis()
/// </summary>
class ArduinoTimer
{
 public:
	 unsigned long timerLength;
	 unsigned long timerStart_ms;
	 unsigned long defaultTimerLength;

	 bool enabled;
	 bool autoResetOnReady;

 public:
	 /// <summary>
	 /// Create a basic timer
	 /// </summary>
	 /// <param name="time">How long the timer is set for</param>
	 /// <param name="runNow">Whether or not to start the timer immediately</param>
	 ArduinoTimer(unsigned long time = 1 TSECONDS, bool runNow = false) : autoResetOnReady(true)
	 {
		 SetDefaultTimerLength(time);
		 enabled = runNow;
		 if (enabled)
			 Reset();
		 else
			timerStart_ms = 0;
	 }

	 /// <summary>
	 /// Sets the length of the timer
	 /// </summary>
	 /// <param name="length">The new length of the timer</param>
	 /// <param name="resetTimer">Start running the timer now?</param>
	 void SetTimerLength(unsigned long length, bool resetTimer = true)
	 {
		 timerLength = length;
		 if (resetTimer)
			 Reset();
		 else
			 enabled = false;
	 }

	 /// <summary>
	 /// Whether or not the timer is ready to go and resets it if it is per <code>autoResetOnReady</code>
	 /// </summary>
	 /// <returns>Is the timer ready?</returns>
	 bool IsReady()
	 {
		 if (!enabled)
			 return false;

		 bool isReady = (millis() >= timerStart_ms + timerLength);
		 if (autoResetOnReady && isReady)
			 Reset();
		 return isReady;
	 }

	 /// <summary>
	 /// How long the timer has been running
	 /// </summary>
	 /// <returns>How long the timer has been active</returns>
	 unsigned long GetTimeRunning() const
	 {
		 return millis() - timerStart_ms;
	 }

	 /// <summary>
	 /// How long the timer has left before being ready
	 /// </summary>
	 /// <returns>How much time is left on the timer</returns>
	 long GetTimeRemaining() const
	 {
		 return max(timerLength - GetTimeRunning(), 0);
	 }

	 /// <summary>
	 /// Returns when the timer will fire next according to millis() time
	 /// </summary>
	 /// <returns></returns>
	 unsigned long GetNextTime() const
	 {
		 return timerStart_ms + timerLength;
	 }

	 /// <summary>
	 /// Sets the default timer length (does nothing beyond setting the timer length right now)
	 /// </summary>
	 /// <param name="length">The length of the timer</param>
	 void SetDefaultTimerLength(unsigned long length)
	 {
		 defaultTimerLength = length;
		 SetTimerLength(length);
	 }

	 /// <summary>
	 /// Resets the timer to "0" (sets to millis()) and starts it running
	 /// </summary>
	 void Reset(bool startRunning = true)
	 {
		 timerStart_ms = millis();
		 enabled = true;
	 }

	 void Start()
	 {
		 Reset();
	 }

	 void Stop()
	 {
		 Reset(false);
	 }
};

#endif

