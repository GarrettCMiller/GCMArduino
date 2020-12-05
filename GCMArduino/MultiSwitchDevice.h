// MultiSwitchDevice.h

#ifndef _MULTISWITCHDEVICE_h
#define _MULTISWITCHDEVICE_h

#include "SwitchDevice.h"

#define MAX_MULTISWITCH_MODES 5
#define MAX_MULTISWITCH_MODE_NAME_LENGTH 5

struct MultiSwitchMode
{
	char name[MAX_MULTISWITCH_MODE_NAME_LENGTH] = { '\0' };
	uint8_t	pinNumber = -1;
	SwitchDevice* modeSwitch = NULL;

	MultiSwitchMode()
	{ }

	MultiSwitchMode(const char* _name, uint8_t pin)
	{
		strncpy(name, _name, MAX_MULTISWITCH_MODE_NAME_LENGTH);
		pinNumber = pin;
		modeSwitch = new SwitchDevice(pinNumber, name);
	}

	~MultiSwitchMode()
	{
		delete modeSwitch;
	}
};

class MultiSwitchDevice : public SwitchDevice
{
 protected:
	 MultiSwitchMode modes[MAX_MULTISWITCH_MODES];
	 uint8_t currentMode;

 public:
	 MultiSwitchDevice(uint8_t mainPin, const char* _name,
						MultiSwitchMode _modes[], bool onByDefault = DEFAULT_RELAY_MODE)
		 : SwitchDevice(mainPin, _name, onByDefault)
	 {
		 currentMode = 0;
		 if (_modes == NULL)
			 Serial.println("ERROR: Invalid mode list");
		 else
			memcpy(modes, _modes, min(sizeof(modes), sizeof(_modes)));
	 }

	 virtual void Init()
	 {
		Serial.print("Multi Init: ");
		Serial.println(GetName());
		SwitchDevice::Init();
		currentMode = -1;
	 }

	 virtual void Update()
	 {
		SwitchDevice::Update();

		for (int d = 0; d < 3; d++)
		{
			modes[d].modeSwitch->Update();
		}
	 }

	virtual bool SetOn(bool on = true)
	{
		if (!on)
			TurnOffOtherSwitches();

		return SwitchDevice::SetOn(on);
	}

	void SetCurrentMode(uint8_t mode)
	{
		/*if ((mode == 0 && !IsOn()) ||
			(mode > 0 && currentMode == mode - 1))
			return;*/

		//TurnOffOtherSwitches();

		if (mode > 0)
		{
			if (IsOn() && currentMode == mode - 1)
				return;

			modes[currentMode].modeSwitch->SetOn(false);
			currentMode = mode - 1;
			if (!IsOn())
				SetOn();
			modes[currentMode].modeSwitch->SetOn();
		}
		else if (IsOn())
			SetOn(false);
	}

	virtual const char* GetCurrentModeName() const
	{
		if (IsOn())
			return modes[currentMode].name;
		return "Off";
	}

protected:
	virtual void TurnOffOtherSwitches()
	{
		for (uint8_t id = 0; id < sizeof(modes) / sizeof(MultiSwitchMode); id++)
		{
			SwitchDevice* device = modes[id].modeSwitch;
			if (device->IsOn())
				device->SetOn(false);
		}
	}
};

#endif

