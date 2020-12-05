// SwitchDevice.h

#ifndef _RELAYSWITCH_h
#define _RELAYSWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "InputCommandListener.h"

#define DEFAULT_RELAY_MODE	false
#define DEVICE_NAME_MAX_LENGTH 32

//#define DEBUG_RELAY_SWITCH_CLASS

//class RelayTimer : public ArduinoTimer
//{
//public:
//	RelayTimer(unsigned long time = 1000)
//};

/// <summary>
/// Represents a relay device, controlled by the pin number
/// found in member variable pinNumber
/// </summary>
class RelaySwitch : public IInputCommandListener
{
public:
	uint8_t pinNumber;	//the pin this device is connected to

protected:
	char name[DEVICE_NAME_MAX_LENGTH];
	bool isOn;
	bool isOnByDefault;		//whether or not the device is switched on when Init() is called
	bool immediateUpdate;	//whether or not we change the pin output immediately when SetOn(true/false) is called, as opposed to in Update()
	
#ifdef DEBUG_RELAY_SWITCH_CLASS
	bool setupComplete;
#endif
	bool isPWM;
	uint8_t pwmStrength;

public:
	RelaySwitch() : immediateUpdate(true), pwmStrength(255), isPWM(false)
	{
		Set(-1);
	}

	RelaySwitch(uint8_t pin) : immediateUpdate(true), pwmStrength(255), isPWM(false)
	{
		Set(pin);
	}

	RelaySwitch(uint8_t pin, const char* _name, bool onByDefault = DEFAULT_RELAY_MODE)
		: immediateUpdate(true), pwmStrength(255), isPWM(false)
	{
		Set(pin, _name, onByDefault);
	}

	virtual void Set(uint8_t pin, const char* _name = "<Set Name>", bool onByDefault = DEFAULT_RELAY_MODE)
	{
#ifdef DEBUG_RELAY_SWITCH_CLASS
		if (setupComplete)
		{
			Serial.println(F("ERROR: RelaySwitch already setup!"));

			//normally return early, but until we see how this plays out, leave as is
			//return;
		}

		if (pin <= 0) //TODO: Check highest pin number, according to board type?
		{
			Serial.print(F("Device error: Invalid pin number: "));
			Serial.println(_name);
		}
		
		Serial.print("Creating relay device: Pin=");

		Serial.print(pinNumber);
		Serial.print(", Name=");
		Serial.println(_name);
#endif

		pinNumber = pin;
		
		isOnByDefault = onByDefault;
		isOn = onByDefault;
		strncpy(name, _name, DEVICE_NAME_MAX_LENGTH);
	}

	virtual void Set(const char* _name = "<Set Name>", bool onByDefault = DEFAULT_RELAY_MODE)
	{
		isOnByDefault = onByDefault;
		isOn = isOnByDefault;
		strncpy(name, _name, DEVICE_NAME_MAX_LENGTH);
	}

	virtual bool SetOn(bool on = true)
	{
		//if (isOn != on)
		//{
		//	Serial.print("Turning device \"");
		//	Serial.print(name);
		//	Serial.println(on ? "\" on" : "\" off");
		//	//Serial.println(".");
		//}

		isOn = on;
		if (immediateUpdate)
			UpdatePinOutput();
		
		return isOn;
	}

	virtual bool Toggle()
	{
		SetOn(!isOn);
	}

	virtual bool IsOn() const
	{
#ifdef DEBUG_RELAY_SWITCH_CLASS
		//if ((digitalRead(pinNumber) == HIGH) != (isOn))
		//{
		//	Serial.print("Device "); Serial.print(name);
		//	Serial.println(" has inconsistent internal state!");
		//	//digitalWrite(pinNumber, isOn ? HIGH : LOW);
		//}
#endif // DEBUG_RELAY_SWITCH_CLASS

		return isOn;
	}

	virtual void SetPWMStrength(uint8_t str)
	{
		pwmStrength = str;
		if (immediateUpdate)
			UpdatePinOutput();
	}

	void SetPWM(bool pwm)
	{
		isPWM = pwm;
		if (immediateUpdate)
			UpdatePinOutput();
	}

	bool IsPWM() const
	{
		return isPWM;
	}

	uint8_t GetPin() const
	{
		return pinNumber;
	}

	virtual const char* GetName() const
	{
		return name;
	}

	virtual const char* GetCurrentModeName() const
	{
		return IsOn() ? "On " : "Off";
	}

	virtual void Init()
	{
		//Serial.print("Relay Init: ");
		//Serial.println(GetName());
		if (pinNumber != -1)
		{
			pinMode(pinNumber, OUTPUT);
			isOn = isOnByDefault;
			UpdatePinOutput();
		}
	}

	virtual void Update()
	{
		if (immediateUpdate)
			return;

		if (isOn)
		{
#ifdef DEBUG_RELAY_SWITCH_CLASS
			Serial.print(F("Pin #"));
			Serial.print(pinNumber);
			Serial.print(F(" is "));
			Serial.println(isOn ? F("On") : F("Off"));
#endif
		}

		if (pinNumber != -1)
		{
			UpdatePinOutput();
		}
	}

	virtual void UpdatePinOutput()
	{
		if (!isPWM)
			digitalWrite(pinNumber, isOn ? HIGH : LOW);
		else
			analogWrite(pinNumber, isOn ? pwmStrength : 0);
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == "Relay")
		{
			String inputName = input.readString();
			String thisName = GetName();
			String isOn = input.readString();

			if (thisName == inputName)
			{
				if (isOn == "On")
					SetOn();
				else if (isOn == "Off")
					SetOn(false);
			}
		}
	}
};

#endif

