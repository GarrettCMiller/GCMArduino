// RangedValue.h

#ifndef _RANGEDVALUE_h
#define _RANGEDVALUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "BlunoOLED.h"

/// <summary>
/// A (base) class that represents not only a value (of a particular type), but its relative minimum and maximum as well.
/// </summary>
class RangedValueBase
{
public:

protected:
	char name[16];

public:
	void SetName(const char* newName)
	{
		strncpy(name, newName, 16);
	}

	const char* GetName() const
	{
		return name;
	}

	virtual void PrintCurrentValue(OLED* pOled) const = 0;
	virtual void PrintMaxValue(OLED* pOled) const = 0;
	virtual void PrintMinValue(OLED* pOled) const = 0;
	virtual void PrintNewValue(OLED* pOled, uint16_t newVal) const = 0;

	virtual long MapKnobValue(uint16_t newVal) const = 0;
	virtual void SetNewValue(uint16_t newVal) = 0;
	virtual uint8_t GetArrowXPos() const = 0;
	virtual bool IsString() const = 0;
};

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

	union
	{
		_Type* targetValue;
		SetNewValueCallback newValCallback;
	};

public:
	RangedValue()
	{
		SetName("INVALID");
		currentValue = minValue = maxValue = 0;
	}

	RangedValue(const char* newName, _Type initialValue, _Type newMinValue, _Type newMaxValue)
		: currentValue(initialValue), minValue(newMinValue), maxValue(newMaxValue), targetValue(NULL)
	{
		SetName(newName);
	}

	RangedValue(const char* newName, _Type& pTargetValue, _Type newMinValue, _Type newMaxValue)
		: targetValue(pTargetValue), currentValue(*pTargetValue), minValue(newMinValue), maxValue(newMaxValue)
	{
		SetName(newName);
	}

	RangedValue(const char* newName, SetNewValueCallback newValueCallback/*, _Type newMinValue, _Type newMaxValue*/)
		//: minValue(newMinValue), maxValue(newMaxValue)
	{
		newValCallback = newValueCallback;
		SetName(newName);
	}

	RangedValue(_Type initialValue, _Type newMinValue, _Type newMaxValue)
		: currentValue(initialValue), minValue(newMinValue), maxValue(newMaxValue)
	{
		SetName(("<NO NAME>"));
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

	void SetCurrentValue(_Type newVal)
	{
		currentValue = newVal;
		if (targetValue != NULL)
			(*targetValue) = newVal;
	}

	operator _Type() const
	{
		return currentValue;
	}

	operator = (_Type newVal) const
	{
		SetCurrentValue(newVal);
	}
};

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
	RangedValue(const char* newName, bool initValue)
		: currentValue(initValue)
	{
		SetName(newName);
	}

	RangedValue(const char* newName, SetNewValueCallback newValCallback)
	{
		newValueCallback = newValCallback;
		SetName(newName);
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
		SetCurrentValue(newVal);
	}

	virtual uint8_t GetArrowXPos() const
	{
		return 120;//map(GetCurrentValue(), GetMinValue(), GetMaxValue(), 1, 126);
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
	RangedValue(const char* newName, String initValue)
		: currentValue(initValue)
	{
		SetName(newName);
	}

	RangedValue(const char* newName, SetNewValueCallback newValCallback)
	{
		newValueCallback = newValCallback;
		SetName(newName);
	}

	String GetMinValue() const
	{
		return "";
	}

	String GetMaxValue() const
	{
		return "";
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
		SetCurrentValue("");
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

/// <summary>
/// A typedef for an uint_8t ranged value
/// </summary>
typedef RangedValue<uint8_t> RangedValue8;
/// <summary>
/// A typedef for an uint_16t ranged value
/// </summary>
typedef RangedValue<uint16_t> RangedValue16;
/// <summary>
/// A typedef for an bool ranged value
/// </summary>
typedef RangedValue<bool> RangedValueB;
/// <summary>
/// A typedef for an String ranged value
/// </summary>
typedef RangedValue<String> RangedString;

#endif

