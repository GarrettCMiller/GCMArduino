// ArduinoDevice.h

#ifndef _ARDUINODEVICE_h
#define _ARDUINODEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "InputCommandListener.h"

#define DEVICE_NAME_MAX_LENGTH 16

#define USE_INIT_INSTEAD_OF_BEGIN
#define USE_UPDATE_INSTEAD_OF_LOOP

/// <summary>
/// An interface for defining objects that require initialization
/// and/or updating (every frame, or on a timer)
/// </summary>
/// <remarks>I chose <c>uint8_t</c> over int or other types because, except in
/// very specific and extreme cases, I can't imagine a function needing to
/// return MORE than 256 different answers.</remarks>
class IArduinoDevice : public IInputCommandListener
{
//protected:
//	bool enabled;
//	uint8_t pinNumber;							//the pin this device is connected to
//	char name[DEVICE_NAME_MAX_LENGTH];
//	bool isOn;

public:
#ifdef USE_INIT_INSTEAD_OF_BEGIN
	virtual uint8_t Initialize() = 0;
#else
	virtual uint8_t begin() = 0;
#endif

#ifdef USE_UPDATE_INSTEAD_OF_LOOP
	virtual uint8_t Update() = 0;
#else
	virtual uint8_t loop() = 0;
#endif

	/*virtual bool IsEnabled() const
	{
		return enabled;
	}

	virtual void Enable(bool _enabled = true)
	{
		enabled = _enabled;
	}*/
};

class ArduinoDevice : public IArduinoDevice
{
protected:
	//bool enabled;
	uint8_t pinNumber;							//the pin this device is connected to
	char name[DEVICE_NAME_MAX_LENGTH];
	bool isOn;

public:
	ArduinoDevice(uint8_t pin, String name)
		//: pinNumber(pin)
	{
		Set(pin, name);
	}

	ArduinoDevice(String name)
	{
	}

	virtual uint8_t Initialize()
	{
		if (pinNumber > 0 && pinNumber < NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS)
		{
			pinMode(pinNumber, OUTPUT);
			return 0;
		}

		if (pinNumber >= A0 && pinNumber <= A0 + NUM_ANALOG_INPUTS)
		{
			Serial.println(F("Analog pins are not valid output pins!"));
		}

		Serial.println(F("INVALID PIN NUMBER!"));
		Serial.println(F("Invalid relay switch pin!"));

		return -1;
	}

	virtual uint8_t Update()
	{
		return 0;
	}

	/// <summary>
	/// Self-explanatory
	/// </summary>
	/// <returns>Whether the device is on or off</returns>
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

	/// <summary>
	/// Turns the device on or off
	/// </summary>
	/// <param name="on">Whether or not HIGH or LOW gets digitally written to the pin, unless <c>isPWM == true</c> where
	/// either <c>0</c> or <c>pwmStrength</c> is analogically (?? is that even a word?) written to the pin</param>
	/// <returns></returns>
	virtual bool TurnOn(bool on = true)
	{
		//if (isOn != on)
		//{
		//	Serial.print("Turning device \"");
		//	Serial.print(name);
		//	Serial.println(on ? "\" on" : "\" off");
		//	//Serial.println(".");
		//}

		isOn = on;
		/*if (immediateUpdate)
			UpdatePinOutput();*/

		return isOn;
	}

	virtual void Set(uint8_t pin, String _name)
	{
		pinNumber = pin;

		isOn = false;
		SetName(_name);
	}

	void SetName(String _name)
	{
		if (_name.length() >= DEVICE_NAME_MAX_LENGTH)
		{
			Serial.print(F("Device name too long: "));
			Serial.println(_name);
		}

		strncpy(name, _name.c_str(), DEVICE_NAME_MAX_LENGTH);
	}

	/// <summary>
	/// Gets the pin number the device is connected to
	/// </summary>
	/// <returns>The pin number the device is connected to</returns>
	uint8_t GetPin() const
	{
		return pinNumber;
	}

	/// <summary>
	/// Gets the name of the device
	/// </summary>
	/// <returns>The name of the device</returns>
	virtual const char* GetName() const
	{
		return name;
	}

	virtual void ProcessInput(PlainProtocol& input)
	{

	}
};
//#define ARDUINO_DEVICE_INIT_FUNCTION	\
//#ifdef USE_INIT_INSTEAD_OF_BEGIN		\
//	virtual uint8_t Initialize()		\
//#else									\
//	virtual uint8_t begin()				\
//#endif
//
//class TEST : public IArduinoDevice
//{
//	ARDUINO_DEVICE_INIT_FUNCTION
//	{
//
//	}
//};
#endif

