// SwitchDevice.h

#ifndef _SWITCHDEVICE_h
#define _SWITCHDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "TimedDevice.h"
#include "RelaySwitch.h"

#define MAX_SWITCH_DEVICES_PER_BOARD 32

class SwitchDevice : public RelaySwitch
{
public:
	//static SwitchDevice* allDevices[MAX_SWITCH_DEVICES_PER_BOARD];
	//static uint8_t numTotalDevices;

public:
	enum ESwitchDeviceMode
	{
		eSDM_ManualOnly,
		eSDM_EventOn,
		eSDM_EventOff,
		eSDM_EventOnEventOff,
		//eSDM_TimedOff
	};

	enum ESwitchDeviceEvent
	{
		eSDE_None,

		eSDE_Timer,

		eSDE_Temperature,
		eSDE_Humidity
	};

	//enum ESwitchDevice

	ArduinoTimer timer;
	uint8_t timesWoken = 0;

protected:
	ESwitchDeviceMode currentSwitchMode;
	ESwitchDeviceEvent eventOn;
	ESwitchDeviceEvent eventOff = eSDE_Timer;

public:
	SwitchDevice()
	{ }

	SwitchDevice(uint8_t pin) : RelaySwitch(pin)
	{
		timer.SetTimerLength(1000);
	}

	SwitchDevice(uint8_t pin, const char* _name, unsigned long timerLength)
		: RelaySwitch(pin, _name)
	{
		SetSwitchMode(SwitchDevice::eSDM_EventOnEventOff);		
		SetTimerPeriod(timerLength);
	}

	SwitchDevice(uint8_t pin, const char* _name, bool onByDefault = DEFAULT_RELAY_MODE, ESwitchDeviceMode deviceMode = ESwitchDeviceMode::eSDM_ManualOnly)
		: RelaySwitch(pin, _name, onByDefault), currentSwitchMode(deviceMode)
	{
		String tempName = _name;
		//if (tempName.indexOf("Relay") == -1)
			//allDevices[numTotalDevices++] = this;
		timer.SetTimerLength(1000);
	}

	virtual bool SetOn(bool on = true)
	{
		timer.Reset();
		return RelaySwitch::SetOn(on);
	}

	void SetTimerPeriod(unsigned long time)
	{
		timer.SetTimerLength(time);
	}

	unsigned long GetTimerPeriod() const
	{
		return timer.timerLength;
	}

	long GetRunningTime() const
	{
		if (IsOn())
			return timer.GetTimeRunning();
		return -1;
	}

	long GetOffTime() const
	{
		if (!IsOn())
			return timer.GetTimeRunning();
		return -1;
	}

	virtual void SetSwitchMode(ESwitchDeviceMode newMode)
	{
		currentSwitchMode = newMode;
	}

	virtual const char* GetCurrentModeName() const
	{
		return RelaySwitch::GetCurrentModeName();
	}

	ESwitchDeviceMode GetCurrentSwitchMode() const
	{
		return currentSwitchMode;
	}

	virtual bool IsTriggerable() const
	{
		return currentSwitchMode == eSDM_EventOn || currentSwitchMode == eSDM_EventOnEventOff;
	}

	virtual bool IsTimed() const
	{
		return currentSwitchMode == eSDM_EventOff || currentSwitchMode == eSDM_EventOnEventOff;
	}

	virtual void Init()
	{
		//Serial.print("Switch Init: ");
		//Serial.println(GetName());
		RelaySwitch::Init();
	}

	virtual void Update()
	{
		RelaySwitch::Update();

		if (isOn && eventOff == eSDE_Timer && timer.IsReady())
			SetOn(false);
		else if (!isOn && eventOn == eSDE_Timer && timer.IsReady())
			SetOn();
	}
};

#endif

