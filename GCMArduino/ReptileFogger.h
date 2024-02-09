// ReptileFogger.h

#ifndef _REPTILEFOGGER_h
#define _REPTILEFOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//#include "ArduinoTimer.h"
#include "SwitchDevice.h"
#include "WaterHeatingDevice.h"

class WaterLevelDetector
{
public:

};

/// <summary>
/// A class that represents a relay attached to the board that controls a fogger/humidifier
/// </summary>
/// <inheritdoc/>
class ReptileFogger : public SwitchDevice
{
	WaterHeatingDevice* pWaterHeater;
	bool waitingForWaterTemp;

private:
	ReptileFogger();						//private default ctor, not acceptable outside this class
	ReptileFogger(ReptileFogger&& ctrArg);		//private move ctor, not acceptable outside this class
	ReptileFogger(const ReptileFogger& ctrArg);	//private copy ctor, not acceptable outside this class
public:
	/// <summary>
	/// Create a fogger device
	/// </summary>
	/// <param name="pin">The pin number the fogger relay control wire is linked to</param>
	/// <param name="waterHeater">The pre-eisting water heating device</param>
	/// <param name="foggerName">The name of the fogger (for menu use and input processing)</param>
	/// <param name="maxRunTime">The maximum time the fogger will be allowed to run before being shut off for a while</param>
	/// <remarks>
	/// Only allowable constructor to ensure proper initialization
	/// </remarks>
	ReptileFogger(uint8_t pin, WaterHeatingDevice* waterHeater, const char* foggerName = "Fogger", unsigned long maxRunTime = 60 TMINUTES)
		: SwitchDevice(pin, foggerName, maxRunTime),	//don't forget the base class ctor
			pWaterHeater(waterHeater),
			waitingForWaterTemp(false)					//assume we are not waiting for the water to heat up in the beginning
	{
		//any special code for the fogger can be inserted here
	}

	virtual uint8_t Initialize()
	{
		return SwitchDevice::Initialize();
	}

	virtual uint8_t Update()
	{
		/*if (waitingForWaterTemp && pWaterHeater->IsWaterReady())
		{
			waitingForWaterTemp = false;
			TurnOn();
		}*/

		return SwitchDevice::Update();
	}

	virtual bool TurnOn(bool on = true)
	{
		/*if (!pWaterHeater->IsWaterReady())
		{
			waitingForWaterTemp = true;
			return false;
		}*/

		Serial.print(F("Turning reptile fogger "));
		Serial.println(on ? "on" : "off");

		return SwitchDevice::TurnOn(on);
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
		if (input == F("Fogger"))
		{
			String arg = input.readString();

			if (arg == F("On"))
				TurnOn();
			else if (arg == F("Off"))
				TurnOn(false);
		}
	}
};

#endif

