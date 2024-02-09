// ReptileMister.h

#ifndef _REPTILEMISTER_h
#define _REPTILEMISTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"
#include "RangedValue.h"
#include "WaterHeatingDevice.h"

class ReptileMister;

class WaterHeatingDevice;

/// <summary>
/// A class representing a water valve
/// </summary>
class MistingValve : public SwitchDevice
{
	friend ReptileMister;

	MistingValve() : SwitchDevice(-1, "INVALID MISTING VALVE")
	{
	}

	ReptileMister* mistingDevice;

public:
	MistingValve(uint8_t pin, const char* valveName, ReptileMister& mister)
		: SwitchDevice(pin, valveName), mistingDevice(&mister)
	{

	}

	virtual uint8_t Initialize()
	{
		return SwitchDevice::Initialize();
	}

	virtual bool TurnOn(bool on);

	//virtual void ProcessInput()
};

/// <summary>
/// A class representing a misting device for a reptile cage
/// </summary>
class ReptileMister : public SwitchDevice
{
	static const uint8_t MAX_VALVES = 20;
	static const uint8_t NUM_VALVES_DEFAULT = 8;

	MistingValve mistingValves[MAX_VALVES];
	uint8_t numValves;

	uint8_t activeValveCount;

	WaterHeatingDevice* pWaterHeater;

public:
	ReptileMister(uint8_t pinMistingUnit, const char* misterName = "Misting Devince")
		: SwitchDevice(pinMistingUnit, misterName),
			activeValveCount(0), numValves(0)
	{
		if (numValves == 0)
			memset(mistingValves, 0, sizeof(MistingValve) * MAX_VALVES);
	}

	ReptileMister(uint8_t pinMistingUnit, //WaterHeatingDevice& waterHeater,
					uint8_t pinFirstValve, uint8_t _numValves = 0/*NUM_VALVES_DEFAULT*/,
					const char* misterName = "Main Mister")
		: SwitchDevice(pinMistingUnit, misterName),
			activeValveCount(0), numValves(_numValves)
			//, pWaterHeater(&waterHeater)
		  //waterHeater(pinWaterHeater, pinWaterTempProbe)
	{
		if (numValves == 0)
			memset(mistingValves, 0, sizeof(MistingValve) * MAX_VALVES);
		else
		{
			for (uint8_t valve = 0; valve < min(numValves, MAX_VALVES); valve++)
			{
				String valveName = "MistingValve" + valve;
				mistingValves[valve] = MistingValve(pinFirstValve + valve, valveName.c_str(), *this);
			}
		}
	}

	void TurnValveOn(MistingValve& valve, bool on)
	{
		for (int v = 0; v < numValves; v++)
		{
			if (mistingValves[v].GetName() == valve.GetName())
			{
				if (mistingValves[v].IsOn() != on)
				{
					IncrementValveCount(on);
					return;
				}
			}
		}
	}

	virtual bool TurnOn(bool on = true)
	{
		/*if (on)
			;*/

		return SwitchDevice::TurnOn(on);
	}

	virtual uint8_t Update()
	{
		//waterHeater.Update();

		/*if (!IsOn() && activeValveCount > 0 && pWaterHeater->IsWaterReady())
			TurnOn();
		else if (activeValveCount == 0)
			TurnOn(false);
		else
			Serial.println(F("Negative active valve count!"));*/

		SwitchDevice::Update();
	}

	MistingValve* GetMistingValveByIndex(uint8_t index)
	{
		if (index >= numValves)
			return NULL;
		return &mistingValves[index];
	}

protected:
	void IncrementValveCount(bool increment)
	{
		activeValveCount += (increment ? 1 : 0);
	}
};

#endif

