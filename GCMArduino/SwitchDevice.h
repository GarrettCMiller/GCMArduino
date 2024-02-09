// SwitchDevice.h

#ifndef _SWITCHDEVICE_h
#define _SWITCHDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "ArduinoTimer.h"
#include "RelaySwitch.h"

//Rather arbitrarily defined (and currently
#define MAX_SWITCH_DEVICES_PER_BOARD 32

/// <summary>
/// A <c>RelaySwitch</c>-derivative that includes a timer for on/off events as well
/// as basic functionality for "listening" to other Arduino devices for triggering
/// an on or off event as well.
/// </summary>
class SwitchDevice : public RelaySwitch
{
private:
	SwitchDevice();								//private default ctor
	//SwitchDevice(SwitchDevice&& ctrArg);		//private move ctor
	//SwitchDevice(const SwitchDevice& ctrArg);	//private copy ctor

//public:
	//static SwitchDevice* allDevices[MAX_SWITCH_DEVICES_PER_BOARD];
	//static uint8_t numTotalDevices;

public:
	/// <summary>
	/// Whether the device is turned on, turned off, or, both, by events, or not,
	/// which only calls to <c>TurnOn()</c> will change the status
	/// </summary>
	enum ESwitchDeviceMode
	{
		eSDM_ManualOnly,
		eSDM_EventOn,
		eSDM_EventOff,
		eSDM_EventOnEventOff,
	};

	/// <summary>
	/// Types of events the device can listen to
	/// </summary>
	enum ESwitchDeviceEvent
	{
		eSDE_None,

		eSDE_Timer,

		eSDE_Temperature,
		eSDE_Humidity,

		eSDE_Other
	};

	ArduinoTimer timerOn;
	ArduinoTimer timerOff;
	uint8_t timesWoken = 0;

protected:
	ESwitchDeviceMode currentSwitchMode;
	ESwitchDeviceEvent eventOn;
	ESwitchDeviceEvent eventOff = eSDE_Timer;

public:
	/// <summary>
	/// Creates a switch device
	/// </summary>
	/// <param name="pin">The pin the device is linked to</param>
	/// <param name="_name">The name of the device</param>
	/// <param name="timerLength">The length of the timer (whether for turning on, off or both)</param>
	/// <param name="onByDefault">Whether or not the device is turned on at the beginning of the sketch</param>
	/// <param name="deviceMode">The initial device mode</param>
	SwitchDevice(uint8_t pin, const char* _name,
		unsigned long timerLength = 10 TMINUTES,
		bool onByDefault = DEFAULT_RELAY_MODE,
		ESwitchDeviceMode deviceMode = ESwitchDeviceMode::eSDM_ManualOnly)
		: RelaySwitch(pin, _name, onByDefault), currentSwitchMode(deviceMode)
	{
		//Serial.println("SWITCH");
		SetTimerOnPeriod(timerLength);
	}

	/// <summary>
	/// Creates a switch device
	/// </summary>
	/// <param name="pin">The pin the device is linked to</param>
	/// <param name="_name">The name of the device</param>
	/// <param name="deviceMode">The initial device mode</param>
	SwitchDevice(uint8_t pin, const char* _name, ESwitchDeviceMode deviceMode)
		: SwitchDevice(pin, _name)
	{
		currentSwitchMode = deviceMode;
	}

	/// <summary>
	/// Same as <c>RelaySwitch::TurnOn()</c>, but resets the internal timer too
	/// </summary>
	/// <param name="on">Whether or not the device is to be turned on</param>
	/// <returns>Whether or not the device is on after setting the new mode</returns>
	virtual bool TurnOn(bool on = true)
	{
		timerOn.Reset();
		//Serial.println(timerOn.timerLength);
		return RelaySwitch::TurnOn(on);
	}

	/// <summary>
	/// Sets the period (length) of the internal timer
	/// </summary>
	/// <param name="time">The length of time in milliseconds</param>
	virtual void SetTimerOnPeriod(unsigned long time)
	{
		timerOn.SetTimerLength(time);
	}

	/// <summary>
	/// Gets the period (length) of the internal timer
	/// </summary>
	/// <returns>The length of time in milliseconds</returns>
	unsigned long GetTimerOnPeriod() const
	{
		return timerOn.timerLength;
	}

	/// <summary>
	/// Sets the period (length) of the internal OFF timer
	/// </summary>
	/// <param name="time">The length of time in milliseconds</param>
	virtual void SetTimerOffPeriod(unsigned long time)
	{
		timerOff.SetTimerLength(time);
	}

	/// <summary>
	/// Gets the period (length) of the internal timer
	/// </summary>
	/// <returns>The length of time in milliseconds</returns>
	unsigned long GetTimerOffPeriod() const
	{
		return timerOff.timerLength;
	}

	/// <summary>
	/// How long the device has been running
	/// </summary>
	/// <returns>The length of time since the device was last turned on, unless it is off, which returns -1</returns>
	long GetRunningTime() const
	{
		if (IsOn())
			return timerOn.GetTimeRunning();
		return -1;
	}

	/// <summary>
	/// How long the device has been off
	/// </summary>
	/// <returns>The length of time since the device was last turned off, unless it is on, which returns -1</returns>
	long GetOffTime() const
	{
		if (!IsOn())
			return timerOff.GetTimeRunning();
		return -1;
	}

	/// <summary>
	/// Sets the new device mode
	/// </summary>
	/// <param name="newMode">The new mode</param>
	virtual void SetSwitchMode(ESwitchDeviceMode newMode)
	{
		currentSwitchMode = newMode;
	}

	/// <summary>
	/// Gets the name, in String format, of the current mode
	/// </summary>
	/// <returns>The name, in String format, of the current mode</returns>
	virtual String GetCurrentModeName() const
	{
		return RelaySwitch::GetCurrentModeName();
	}

	/// <summary>
	/// Gets the current device mode in enum form
	/// </summary>
	/// <returns>The current device mode in enum form</returns>
	ESwitchDeviceMode GetCurrentSwitchMode() const
	{
		return currentSwitchMode;
	}

	/// <summary>
	/// Can be triggered
	/// </summary>
	/// <returns></returns>
	virtual bool IsTriggerable() const
	{
		return currentSwitchMode == eSDM_EventOn || currentSwitchMode == eSDM_EventOnEventOff;
	}

	/// <summary>
	/// Reacts to the internal timer or not
	/// </summary>
	/// <returns></returns>
	virtual bool IsTimed() const
	{
		return currentSwitchMode == eSDM_EventOff || currentSwitchMode == eSDM_EventOnEventOff;
	}

	/// <summary>
	/// Initializes the device
	/// </summary>
	virtual uint8_t Initialize()
	{
		//Serial.print("Switch Init: ");
		//Serial.println(GetName());
		return RelaySwitch::Initialize();
	}

	/// <summary>
	/// Updates the <c>RelaySwitch</c> and also reacts to the internal timer, if necessary
	/// </summary>
	virtual uint8_t Update()
	{
		if (IsTimed() && timerOn.IsReady())
		{
			if (IsOn() && eventOff == eSDE_Timer)
			{
				Serial.print(F("Timing device \""));
				Serial.print(this->GetName());
				Serial.print(F("\" off..."));
				TurnOn(false);
			}
			else if (!IsOn() && eventOn == eSDE_Timer)
				TurnOn();
		}

		return RelaySwitch::Update();
	}
};

#endif

