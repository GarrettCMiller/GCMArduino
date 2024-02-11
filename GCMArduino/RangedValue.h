/*
	Name:       RangedValue.h
	Created:	11/21/2020 3:41:33 AM
	Author:     Garrett Miller
	Notes:		This code heavily uses Editor Outlining as used in Visual Studio, with
				frequent use of #pragma region (Region Label/Name)\n ... \n ... \n #pragma endregion
				to collapse regions of code and comments into logical groupings.
				For those unfamiliar, Ctrl+M, Ctrl+L toggles all outlining open/closed
				while Ctrl+M, Ctrl+O collapses all outlining to closed. Handy for seeing related
				groups of code and temporarily "hiding" other code.
*/

#ifndef _RANGEDVALUE_h
#define _RANGEDVALUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "BlunoOLED.h"

#define RANGEDVALUE_MAX_NAME_LENGTH	8

#pragma region Abstract Base Class

/// <summary>
/// A (base) class that represents not only a value (of a particular type),
/// but its relative minimum and maximum as well. How you use that range is
/// up to you.
/// </summary>
class RangedValueBase
{
	//RangedValueBase();
	//RangedValueBase(RangedValueBase&& ctrArg);		//Need this to build w/o errors, will explore further
	//RangedValueBase(const RangedValueBase& ctrArg);

protected:
	bool hasCallback;
	char name[RANGEDVALUE_MAX_NAME_LENGTH];

	void SetHasCallback(bool hasCback)
	{
		hasCallback = hasCback;
	}

public:
	RangedValueBase(String newName = "")
	{
		SetName(newName);
	}

#pragma region Name Functions

	/// <summary>
	/// Sets the name of the ranged value, for menu and other purposes
	/// </summary>
	/// <param name="newName">The new name</param>
	void SetName(String newName)
	{
		strncpy(name, newName.c_str(), RANGEDVALUE_MAX_NAME_LENGTH);
	}

	/*void SetName(const __FlashStringHelper* newName)
	{
		SetName(newName);
	}*/

	/// <summary>
	/// Gets the name of the ranged value
	/// </summary>
	/// <returns>The name</returns>
	const char* GetName() const
	{
		return name;
	}

#pragma endregion

	bool HasCallback() const
	{
		return hasCallback;
	}

#pragma region Abstract Function Declarations (MUST be derived in base classes)

	/// <summary>
	/// Prints the current value to the screen, at whatever location last set with
	/// <c>OLED::SetCursorPos</c>
	/// </summary>
	/// <param name="pOled">A pointer to the OLED screen</param>
	virtual void PrintCurrentValue(OLED* pOled) const = 0;

	/// <summary>
	/// Prints the maximum value to the screen, at whatever location last set with
	/// <c>OLED::SetCursorPos</c>
	/// </summary>
	/// <param name="pOled">A pointer to the OLED screen</param>
	virtual void PrintMaxValue(OLED* pOled) const = 0;

	/// <summary>
	/// Prints the minimum value to the screen, at whatever location last set with
	/// <c>OLED::SetCursorPos</c>
	/// </summary>
	/// <param name="pOled">A pointer to the OLED screen</param>
	virtual void PrintMinValue(OLED* pOled) const = 0;

	/// <summary>
	/// Prints the value that is (possibly) about to be the new value. Based on what the knob value
	/// reads if on a "set value" screen.
	/// </summary>
	/// <param name="pOled">A pointer to the OLED screen</param>
	/// <param name="newVal">The (possibly temporary) new value, NOT YET ACTUALLY SET</param>
	virtual void PrintNewValue(OLED* pOled, uint16_t newVal) const = 0;

	/// <summary>
	/// Maps whatever value the knob is set to (relative to its own min and max values) to the range as
	/// specified by this object's min and max
	/// </summary>
	/// <param name="newVal">The current knob value</param>
	/// <returns>The newly-mapped value, in the range of this object's min and max</returns>
	virtual long MapKnobValue(uint16_t newVal) const = 0;

	/// <summary>
	/// ACTUALLY sets the current value, via <c>RangedValue::SetCurrentValue</c>
	/// </summary>
	/// <param name="newVal">The incoming, new value</param>
	virtual void SetNewValue(uint16_t newVal) = 0;

	/// <summary>
	/// Returns where the center of the downward-pointing arrow points to on the set value screen
	/// </summary>
	/// <returns></returns>
	virtual uint8_t GetArrowXPos() const = 0;

	/// <summary>
	/// Whether or not this class represents a String-type object. Kinda hacked and hard-coded, sorry,
	/// best I could do under constraints.
	/// </summary>
	/// <returns></returns>
	virtual bool IsString() const = 0;

#pragma endregion

};

#pragma endregion

#pragma region Standard Templated Base Class

/// <summary>
/// The default-, (derived- and non-abstract-), templated version of abstract base class RangedValueBase
/// </summary>
/// <typeparam name="_Type"></typeparam>
template <typename _Type> 
class RangedValue : public RangedValueBase
{
	
protected:
	_Type currentValue;
	_Type minValue;
	_Type maxValue;
	
	typedef void (*SetNewValueCallback)(_Type newVal);

	//We only EVER need one of these at ANY given time
	union
	{
		_Type* targetValue;
		SetNewValueCallback newValCallback;
	};

public:
	/// <summary>
	/// The default constructor. Immediately marks the value as "INVALID"
	/// </summary>
	RangedValue()// : RangedValueBase()
	{
		SetName(F("INVALID"));
		currentValue = minValue = maxValue = 0;
	}

	/// <summary>
	/// Constructor that takes the name, the initial value, the minimum value and the maximum value
	/// </summary>
	/// <param name="newName">The name</param>
	/// <param name="initialValue">Initial value</param>
	/// <param name="newMinValue">Minimum value</param>
	/// <param name="newMaxValue">Maximum value</param>
	RangedValue(String newName, _Type initialValue, _Type newMinValue, _Type newMaxValue)
		: RangedValueBase(newName), currentValue(initialValue), minValue(newMinValue), maxValue(newMaxValue), targetValue(NULL)
	{
		SetName(newName);
	}

	/*RangedValue(const __FlashStringHelper* newName, _Type initialValue, _Type newMinValue, _Type newMaxValue)
		: RangedValueBase(newName), currentValue(initialValue), minValue(newMinValue), maxValue(newMaxValue), targetValue(NULL)
	{
		SetName(newName);
	}*/

	/*RangedValue(const char* newName, _Type& pTargetValue, _Type newMinValue, _Type newMaxValue)
		: targetValue(pTargetValue), currentValue(*pTargetValue), minValue(newMinValue), maxValue(newMaxValue)
	{
		SetName(newName);
	}*/

	/// <summary>
	/// Constructor that takes the name and a callback function
	/// </summary>
	/// <param name="newName"></param>
	/// <param name="newValueCallback"></param>
	RangedValue(String newName, SetNewValueCallback newValueCallback/*, _Type newMinValue, _Type newMaxValue*/)
		//: hasCallback(true)//, minValue(newMinValue), maxValue(newMaxValue)
	{
		hasCallback = true;
		newValCallback = newValueCallback;
		SetName(newName);
	}

	/// <summary>
	/// Constructor that takes the name, a callback function AND BOTH a minimum and maximum
	/// </summary>
	/// <param name="newName"></param>
	/// <param name="newValueCallback"></param>
	/// <param name="newMinValue"></param>
	/// <param name="newMaxValue"></param>
	RangedValue(String newName, SetNewValueCallback newValueCallback, _Type newMinValue, _Type newMaxValue)
		: /*hasCallback(true),*/ minValue(newMinValue), maxValue(newMaxValue)
	{
		hasCallback = true;
		newValCallback = newValueCallback;
		SetName(newName);
	}

	/// <summary>
	/// Constructor that takes only an initial value, and both minimum and maximum values
	/// </summary>
	/// <param name="initialValue"></param>
	/// <param name="newMinValue"></param>
	/// <param name="newMaxValue"></param>
	RangedValue(_Type initialValue, _Type newMinValue, _Type newMaxValue)
		: currentValue(initialValue), minValue(newMinValue), maxValue(newMaxValue)
	{
		SetName(F("<NO NAME>"));
	}

	virtual void PrintCurrentValue(OLED* pOled) const
	{
		pOled->print(GetCurrentValue());
	}

	virtual void PrintMinValue(OLED* pOled) const
	{
		pOled->print(GetMinValue());
	}

	virtual void PrintMaxValue(OLED* pOled) const
	{
		pOled->print(GetMaxValue());
	}

	virtual void PrintNewValue(OLED* pOled, uint16_t newVal) const
	{
		pOled->print(MapKnobValue(newVal));
	}

	virtual long MapKnobValue(uint16_t newVal) const
	{
		return map(newVal, 0, 1023, GetMinValue(), GetMaxValue());
	}

	virtual void SetNewValue(uint16_t newVal)
	{
		if (newValCallback)
		{
			Serial.println(F("Start new value callback!"));
			newValCallback(newVal);
			Serial.println(F("Finish new value callback!"));
		}
		else
			SetCurrentValue(newVal);
	}

	virtual uint8_t GetArrowXPos() const
	{
		return map(GetCurrentValue(), GetMinValue(), GetMaxValue(), 1, 126);
	}

	virtual bool IsString() const
	{
		return false;
	}

	_Type GetMinValue() const
	{
		return minValue;
	}

	void SetMinValue(_Type newMin)
	{
		minValue = newMin;
	}

	_Type GetMaxValue() const
	{
		return maxValue;
	}

	void SetMaxValue(_Type newMax)
	{
		maxValue = newMax;
	}

	_Type GetCurrentValue() const
	{
		return currentValue;
	}

	_Type SetCurrentValue(const _Type& newVal)
	{
		currentValue = newVal;
		if (targetValue != NULL)
			(*targetValue) = newVal;
		return *this;
	}

	operator _Type() const
	{
		return currentValue;
	}

	_Type operator = (const _Type& newVal)
	{
		return SetCurrentValue(newVal);
	}
};

#pragma endregion

#pragma region Specific Implementations (bool and String)

/// <summary>
/// The bool-specific implementation of the templated class RangedValue
/// </summary>
template <>
class RangedValue<bool> : public RangedValueBase
{
public:
	typedef void (*SetNewValueCallback)(bool newVal);

protected:
	bool currentValue;

public:
	SetNewValueCallback newValueCallback;

public:
	RangedValue(String newName, bool initValue)
		: RangedValueBase(newName), currentValue(initValue)
	{
	}

	RangedValue(String newName, SetNewValueCallback newValCallback)
		: RangedValueBase(newName)
	{
		newValueCallback = newValCallback;
	}

	bool GetMinValue() const
	{
		return false;
	}

	bool GetMaxValue() const
	{
		return true;
	}

	virtual void PrintCurrentValue(OLED* pOled) const
	{
		pOled->print(GetCurrentValue() ? F("true") : F("false"));
	}

	virtual void PrintMinValue(OLED* pOled) const
	{
		pOled->print(GetMinValue() ? F("true") : F("false"));
	}

	virtual void PrintMaxValue(OLED* pOled) const
	{
		pOled->print(GetMaxValue() ? F("true") : F("false"));
	}

	virtual void PrintNewValue(OLED* pOled, uint16_t newVal) const
	{
		pOled->print(MapKnobValue(newVal) ? F("true") : F("false"));
	}

	virtual long MapKnobValue(uint16_t newVal) const
	{
		return map(newVal, 0, 1023, GetMinValue(), GetMaxValue());
	}

	virtual void SetNewValue(uint16_t newVal)
	{
		SetCurrentValue(newVal);
	}

	virtual uint8_t GetArrowXPos() const
	{
		return GetCurrentValue() ? 0 : 128;//120;//map(GetCurrentValue(), GetMinValue(), GetMaxValue(), 1, 126);
	}

	virtual bool IsString() const
	{
		return false;
	}

	//void SetMinValue(bool newMin)
	//{
	//	minValue = newMin;
	//}

	//void SetMaxValue(bool newMax)
	//{
	//	maxValue = newMax;
	//}

	bool GetCurrentValue() const
	{
		return currentValue;
	}

	void SetCurrentValue(bool newVal)
	{
		currentValue = newVal;
		if (newValueCallback != NULL)
			newValueCallback(currentValue);
	}

	operator bool() const
	{
		return currentValue;
	}
};

/// <summary>
/// The String-specific implementation of the templated class RangedValue
/// </summary>
template <>
class RangedValue<String> : public RangedValueBase
{
public:
	typedef void (*SetNewValueCallback)(String newVal);

protected:
	String currentValue;

public:
	SetNewValueCallback newValueCallback;

public:
	RangedValue(String newName, String initValue)
		: RangedValueBase(newName), currentValue(initValue)
	{
	}

	RangedValue(String newName, SetNewValueCallback newValCallback)
		: RangedValueBase(newName)
	{
		newValueCallback = newValCallback;
	}

	String GetMinValue() const
	{
		return F("");
	}

	String GetMaxValue() const
	{
		return F("");
	}

	virtual void PrintCurrentValue(OLED* pOled) const
	{
		pOled->print(GetCurrentValue());
	}

	virtual void PrintMinValue(OLED* pOled) const
	{
		pOled->print(GetMinValue());
	}

	virtual void PrintMaxValue(OLED* pOled) const
	{
		pOled->print(GetMaxValue());
	}

	virtual void PrintNewValue(OLED* pOled, uint16_t newVal) const
	{
		pOled->print(MapKnobValue(newVal));
	}

	virtual long MapKnobValue(uint16_t newVal) const
	{
		return map(newVal, 0, 1023, 0, 1000);
	}

	virtual void SetNewValue(uint16_t newVal)
	{
		SetCurrentValue(F(""));
	}

	virtual uint8_t GetArrowXPos() const
	{
		return 120;//map(GetCurrentValue(), GetMinValue(), GetMaxValue(), 1, 126);
	}

	virtual bool IsString() const
	{
		return true;
	}

	//void SetMinValue(bool newMin)
	//{
	//	minValue = newMin;
	//}

	//void SetMaxValue(bool newMax)
	//{
	//	maxValue = newMax;
	//}

	String GetCurrentValue() const
	{
		return currentValue;
	}

	void SetCurrentValue(String newVal)
	{
		currentValue = newVal;
		if (newValueCallback != NULL)
			newValueCallback(currentValue);
	}

	operator String() const
	{
		return currentValue;
	}
};

#pragma endregion

#pragma region Shortcut Typedefs

/// <summary>
/// A typedef for an uint_8t ranged value
/// </summary>
typedef RangedValue<uint8_t> RangedValueU8;
/// <summary>
/// A typedef for an uint_16t ranged value
/// </summary>
typedef RangedValue<uint16_t> RangedValueU16;
/// <summary>
/// A typedef for an bool ranged value
/// </summary>
typedef RangedValue<bool> RangedValueB;
/// <summary>
/// A typedef for an String ranged value
/// </summary>
typedef RangedValue<String> RangedString;

#pragma endregion

#endif

