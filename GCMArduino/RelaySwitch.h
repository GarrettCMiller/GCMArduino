// SwitchDevice.h

#ifndef _RELAYSWITCH_h
#define _RELAYSWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//#include "PinID.h"

#include "ArduinoDevice.h"

#define DEFAULT_RELAY_MODE	false

//#define DEBUG_RELAY_SWITCH_CLASS

/// <summary>
/// Represents a relay device, controlled by the pin number found in member variable <c>pinNumber</c>.
/// Can be turned on or off as well as toggled from whatever current state into the other.
/// Can be PWM-ed, if this particular pin is able.
/// 
/// <para> See <see cref="RelaySwitch::TurnOn" /> and <see cref="RelaySwitch::Toggle" /> </para>
/// See also <seealso cref="RelaySwitch::SetPWMStrength"/>
/// </summary>
class RelaySwitch : public ArduinoDevice
{
#pragma region Ctors
	RelaySwitch();								//private default ctor
	//RelaySwitch(RelaySwitch&& ctrArg);			//private move ctor
	//RelaySwitch(const RelaySwitch& ctrArg);		//private copy ctor
#pragma endregion

#pragma region Fields
	//uint8_t pinNumber;							//the pin this device is connected to
	//char name[DEVICE_NAME_MAX_LENGTH];
	//bool isOn;
	bool isOnByDefault;							//whether or not the device is switched on when Init() is called
	bool immediateUpdate;						//whether or not we change the pin output immediately when SetOn(true/false) is called, as opposed to in Update()

#ifdef DEBUG_RELAY_SWITCH_CLASS
	bool setupComplete;
#endif

	bool isPWM;									//whether we use digitalWrite(isOn => LOW/HIGH) or analogWrite(pwmStrength => 0-255)
	uint8_t pwmStrength;

#pragma endregion

	/// <summary>
	/// Sets pin data, name and default device status, called ONLY from ctor
	/// </summary>
	/// <param name="pin"></param>
	/// <param name="_name"></param>
	/// <param name="onByDefault"></param>
//	virtual void Set(uint8_t pin, const char* _name)
//	{
//#ifdef DEBUG_RELAY_SWITCH_CLASS
//		if (setupComplete)
//		{
//			Serial.println(F("ERROR: RelaySwitch already setup!"));
//
//			//normally return early, but until we see how this plays out, leave as is
//			//return;
//		}
//
//		if (pin <= 0) //TODO: Check highest pin number, according to board type?
//		{
//			Serial.print(F("Device error: Invalid pin number: "));
//			Serial.println(_name);
//		}
//
//		Serial.print("Creating relay device: Pin=");
//
//		Serial.print(pinNumber);
//		Serial.print(", Name=");
//		Serial.println(_name);
//#endif
//
//		pinNumber = pin;
//
//		//isOnByDefault = onByDefault;
//		//isOn = onByDefault;
//		strncpy(name, _name, DEVICE_NAME_MAX_LENGTH);
//	}

public:
	/// <summary>
	/// Creates a <c>RelaySwitch</c> device
	/// </summary>
	/// <param name="pin">The Arduino pin that the control wire is linked to</param>
	/// <param name="_name">The name of the device</param>
	/// <param name="_isPWM">Is in PWM mode or not (<c>analogWrite()</c> vs <c>digitalWrite()</c>)</param>
	/// <param name="_pwmStrength">The strength of the PWM signal, directly written as <c>analogWrite(pwmStrength)</c></param>
	/// <param name="onByDefault">Whether or not the device is turned on at the beginning of the sketch</param>
	RelaySwitch(uint8_t pin, const char* _name,
		bool _isPWM = false, uint8_t _pwmStrength = 255,
		bool onByDefault = DEFAULT_RELAY_MODE)
		:	ArduinoDevice(pin, name),
			immediateUpdate(true), pwmStrength(_pwmStrength), isPWM(_isPWM)
	{
		//Serial.println("RELAY");
		//Set(pin, _name, onByDefault);
	}

	/// <summary>
	/// Turns the device on or off
	/// </summary>
	/// <param name="on">Whether or not HIGH or LOW gets digitally written to the pin, unless <c>isPWM == true</c> where
	/// either <c>0</c> or <c>pwmStrength</c> is analogically (?? is that even a word?) written to the pin</param>
	/// <returns></returns>
	virtual bool TurnOn(bool on = true)
	{
		ArduinoDevice::TurnOn(on);

		if (immediateUpdate)
			UpdatePinOutput();
		
		return isOn;
	}

	/// <summary>
	/// Flips the device state essentially. If it's on, turn it off. If it's off, turn it on.
	/// </summary>
	/// <returns>The state of the device after being toggled, true => on</returns>
	virtual bool Toggle()
	{
		return TurnOn(!isOn);
	}

//	/// <summary>
//	/// Self-explanatory
//	/// </summary>
//	/// <returns>Whether the device is on or off</returns>
//	virtual bool IsOn() const
//	{
//#ifdef DEBUG_RELAY_SWITCH_CLASS
//		//if ((digitalRead(pinNumber) == HIGH) != (isOn))
//		//{
//		//	Serial.print("Device "); Serial.print(name);
//		//	Serial.println(" has inconsistent internal state!");
//		//	//digitalWrite(pinNumber, isOn ? HIGH : LOW);
//		//}
//#endif // DEBUG_RELAY_SWITCH_CLASS
//
//		return isOn;
//	}

	/// <summary>
	/// Sets the PWM strength that will be written to the pin via <c>analogWrite(pwmStrength)</c>
	/// </summary>
	/// <param name="str">The new PWM strength</param>
	virtual uint8_t SetPWMStrength(uint8_t str)
	{
		pwmStrength = str;
		if (immediateUpdate)
			UpdatePinOutput();
		return pwmStrength;
	}

	/// <summary>
	/// Returns the strength of the PWM signal, 0 - 255
	/// </summary>
	/// <returns>The strength of the PWM signal, 0 - 255</returns>
	virtual uint8_t GetPWMStrength() const
	{
		return pwmStrength;
	}

	/// <summary>
	/// Sets the PWM mode of the device
	/// </summary>
	/// <param name="pwm">Whether or not to use <c>analogWrite(pwmStrength)</c> or <c>digitalWrite(isOn ? HIGH : LOW)</c></param>
	void SetPWM(bool pwm = true)
	{
		isPWM = pwm;
		if (immediateUpdate)
			UpdatePinOutput();
	}

	/// <summary>
	/// Whether or not PWM mode is enabled
	/// </summary>
	/// <returns>Whether or not PWM mode is enabled</returns>
	bool IsPWM() const
	{
		return isPWM;
	}

	/*/// <summary>
	/// Gets the pin number the device is connected to
	/// </summary>
	/// <returns>The pin number the device is connected to</returns>
	uint8_t GetPin() const
	{
		return pinNumber;
	}*/

	/*/// <summary>
	/// Gets the name of the device
	/// </summary>
	/// <returns>The name of the device</returns>
	virtual const char* GetName() const
	{
		return name;
	}*/

	/// <summary>
	/// Returns a string representation of whether the device is "On" or "Off"
	/// </summary>
	/// <returns>The device mode: "On" or "Off"</returns>
	virtual String GetCurrentModeName() const
	{
		return IsOn() ? F("On") : F("Off");
	}

	/// <summary>
	/// Initialize the device. Sets the pin mode to OUTPUT and either writes
	/// <c>HIGH</c> if the device is <c>onByDefault</c> or <c>LOW</c> if the
	/// device is not on by default.
	/// </summary>
	virtual uint8_t Initialize()
	{
		//Serial.print("Relay Init: ");
		//Serial.println(GetName());
		/*if (pinNumber > 0 && pinNumber < NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS)
		{
			pinMode(pinNumber, OUTPUT);
			isOn = isOnByDefault;
			UpdatePinOutput();
			return 0;
		}

		if (pinNumber >= A0 && pinNumber <= A0 + NUM_ANALOG_INPUTS)
		{
			Serial.println(F("Analog pins are not valid output pins!"));
		}

		Serial.println(F("INVALID PIN NUMBER!"));
		Serial.println(F("Invalid relay switch pin!"));

		return -1;*/
		if (ArduinoDevice::Initialize() == 0)
		{
			TurnOn(isOnByDefault);
			UpdatePinOutput();
		}
	}

	/// <summary>
	/// Updates <c>digitalWrite()</c> or <c>analogWrite()</c> based on <c>isOn</c> and <c>PWMStrength</c>
	/// Does nothing if the device is in immediate update mode (which writes to the pin ONLY when the device
	/// changes instead of every frame, saving quite a bit of processing.
	/// </summary>
	virtual uint8_t Update()
	{
		if (immediateUpdate)
			return 0;

		if (isOn)
		{
#ifdef DEBUG_RELAY_SWITCH_CLASS
			Serial.print(F("Pin #"));
			Serial.print(pinNumber);
			Serial.print(F(" is "));
			Serial.println(isOn ? F("On") : F("Off"));
#endif
		}

		if (pinNumber > 0)
		{
			UpdatePinOutput();
			return 0;
		}
	}

	/// <summary>
	/// Writes to the pin:
	/// <c>analogWrite(off => 0, on => pwmStrength)</c> if <c>isPWM</c> is <c>true</c>,
	/// otherwise, <c>digitalWrite(off => LOW, on => HIGH)</c>
	/// </summary>
	virtual void UpdatePinOutput()
	{
		if (pinNumber < 0)
		{
			Serial.println(F("INVALID SWITCH DEVICE PIN NUMBER!"));
			return;
		}

		if (!isPWM)
			digitalWrite(pinNumber, isOn ? HIGH : LOW);
		else
			analogWrite(pinNumber, isOn ? pwmStrength : 0);
	}

	/// <summary>
	/// Process any input received via PlainProtocol
	/// </summary>
	/// <param name="input">A reference to the PlainProtocol object representing the input (and possible output)</param>
	/// <remarks>
	/// Derived from (and required by) IInputCommandListener
	/// </remarks>
	virtual void ProcessInput(PlainProtocol& input)
	{
		//if the command is "Relay"
		if (input == "Relay")
		{
			String inputName = input.readString();		//ideally, a relay (device) name
			String thisName = GetName();				//*this* device name
			String isOn = input.readString();			//set to on or off

			if (thisName == inputName)					// <Relay;(relay name);[On/Off];
			{
				if (isOn == "On")
					TurnOn();
				else if (isOn == "Off")
					TurnOn(false);
			}
		}
	}
};

#endif

