// LED_RGB.h

#ifndef _LED_RGB_h
#define _LED_RGB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define MAX_BRIGHTNESS 255

#include "ArduinoDevice.h"
#include "RangedValue.h"
#include "ArduinoTimer.h"

enum ELEDState
{
	eLS_Off,
	eLS_OnSolid,
	eLS_OnBlink
};

class LED : public ArduinoDevice
{
public:

	enum EFlashLength
	{
		eFL_Short,
		eFL_Medium,
		eFL_Long
	};

	static const int flashLengthVeryShort = 20;
	static const int flashLengthShort = 50;
	static const int flashLengthMedium = 250;
	static const int flashLengthLong = 500;
	static const int flashDelayDefault = 2000;

	uint8_t pin;
	ELEDState eLEDState;
	RangedValueU8 value;
	RangedValueU8 brightness;

	RangedValueU8 blinkCount;
	RangedValueU16 blinkDelay;

	ArduinoTimer flashDelayTimer;
	ArduinoTimer flashOnTimer;
	ArduinoTimer flashOffTimer;

protected:
	bool isFlashing;
	uint8_t blinkIndex;

public:
	LED()
		: ArduinoDevice("LED")
	{

	}

	LED(uint8_t _pin, ELEDState ledState = eLS_Off)
		: ArduinoDevice(_pin, "LED"), eLEDState(ledState)
	{
		//Serial.println("LED");
		flashDelayTimer = ArduinoTimer(flashDelayDefault, true);
		flashDelayTimer.autoResetOnReady = false;
		flashOnTimer = ArduinoTimer(flashLengthVeryShort);
		flashOnTimer.autoResetOnReady = false;
		flashOffTimer = ArduinoTimer(flashLengthVeryShort);
		flashOffTimer.autoResetOnReady = false;

		value = RangedValueU8(F("Value"), (uint8_t)0, 0, 255);
		
		brightness = RangedValueU8(F("Brightness"), 1, 0, 255);
		blinkCount = RangedValueU8(F("Blink Count"), 2, 1, 10);
		blinkDelay = RangedValueU16(F("Blink Delay"), flashDelayTimer.timerLength, 100, 5000);
	}

	virtual uint8_t Initialize()
	{
		isFlashing = false;

		pinMode(pin, OUTPUT);

		return 0;
	}

	virtual uint8_t Update()
	{
		if (flashDelayTimer.timerLength != blinkDelay)
			flashDelayTimer.SetTimerLength(blinkDelay);

		if (isFlashing)
		{
			if (blinkIndex < blinkCount)
			{
				if (flashOnTimer.IsReady())
				{
					flashOnTimer.enabled = false;
					//Serial.println("Flash on timer ready! Turning off...");
					flashOffTimer.Reset();
					blinkIndex++;
					TurnOff();
				}
				else if (flashOffTimer.IsReady())
				{
					flashOffTimer.enabled = false;
					//Serial.println("Flash off timer ready! Turning on...");
					flashOnTimer.Reset();

					UpdateColor();
				}
			}
			else
			{
				//Serial.println("Finished blinking!");
				blinkIndex = 0;
				isFlashing = false;

				TurnOff();

				flashDelayTimer.Reset();
			}
		}
		else if (eLEDState == eLS_OnBlink && flashDelayTimer.IsReady())
		{
			Flash(flashOnTimer.timerLength, flashOffTimer.timerLength, blinkCount);
		}

		return 0;
	}

	void Flash(int msOn, int msOff, int times)
	{
		isFlashing = true;

		blinkIndex = 0;
		blinkCount = times;

		flashOnTimer.SetTimerLength(msOn);
		flashOffTimer.SetTimerLength(msOff, false);

		//for (blinkIndex = 0; i < times; i++)
		{
			UpdateColor();
			//delay(msOn);
			//turnOff();
			//delay(msOff);
		}

		//setColor(0, 0, 0);
		//turnOff();

		//Serial.println("Starting blinking...");
	}


	void TurnOn()
	{
		UpdateColor();
	}

	void TurnOff()
	{
		analogWrite(pin, 255);
	}


	void UpdateColor()
	{
		analogWrite(pin, 255 - (value * (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
	}

	virtual void ProcessInput(PlainProtocol& input)
	{

	}
};

///////////////////////////////////////////////////////////////////////////////

/// <summary>
/// The RGB LED on the Bluno Accessory Shield. Can be turned on/off with brightness adjustment, set to a solid color
/// or blinked a particular color a certain number of times (<c>blinkCount</c>), every <c>blinkDelay</c> ms
/// </summary>
class LED_RGB : public ArduinoDevice
{
public:
	static const uint8_t pinRedDefault = 9;
	static const uint8_t pinGreenDefault = 10;
	static const uint8_t pinBlueDefault = 3;

	static const uint8_t valueRedDefault = 255;
	static const uint8_t valueGreenDefault = 255;
	static const uint8_t valueBlueDefault = 255;

	/*uint8_t pinRed;
	uint8_t pinGreen;
	uint8_t pinBlue;*/

	/*RangedValueU8 valueRed;
	RangedValueU8 valueGreen;
	RangedValueU8 valueBlue;*/

	//RangedValueU8 brightness;

	/*RangedValueU8 blinkCount;
	RangedValueU16 blinkDelay;

	ArduinoTimer flashDelayTimer;
	ArduinoTimer flashOnTimer;
	ArduinoTimer flashOffTimer;*/

	LED ledRed;
	LED ledGreen;
	LED ledBlue;

 public:
	LED_RGB()
		: ArduinoDevice("LED_RGB"),
		ledRed(pinRedDefault), ledGreen(pinGreenDefault), ledBlue(pinBlueDefault)
	{
		//Serial.println("LED");
		/*flashDelayTimer = ArduinoTimer(flashDelayDefault, true);
		flashDelayTimer.autoResetOnReady = false;
		flashOnTimer = ArduinoTimer(flashLengthVeryShort);
		flashOnTimer.autoResetOnReady = false;
		flashOffTimer = ArduinoTimer(flashLengthVeryShort);
		flashOffTimer.autoResetOnReady = false;

		valueRed = RangedValueU8("Red", (uint8_t)0, 0, 255);
		valueGreen = RangedValueU8("Green", 255, 0, 255);
		valueBlue = RangedValueU8("Blue", (uint8_t)0, 0, 255);
		brightness = RangedValueU8("Brightness", 1, 0, 255);
		blinkCount = RangedValueU8("Blink Count", 2, 1, 10);
		blinkDelay = RangedValueU16("Blink Delay", flashDelayTimer.timerLength, 100, 5000);*/
	}

	virtual uint8_t Initialize()
	{
		ledRed.Initialize();
		ledGreen.Initialize();
		ledBlue.Initialize();

		UpdateColor();
		
		return 0;
	}

	virtual uint8_t Update()
	{
		ledRed.Update();
		ledGreen.Update();
		ledBlue.Update();

		return 0;
	}

	void flashColor(uint8_t red, uint8_t green, uint8_t blue, LED::EFlashLength flashLength, int times)
	{
		//isFlashing = true;

		int ms;
		switch (flashLength)
		{
			case LED::eFL_Short:
				ms = LED::flashLengthShort;
				break;

			case LED::eFL_Medium:
				ms = LED::flashLengthMedium;
				break;

			case LED::eFL_Long:
				ms = LED::flashLengthLong;
				break;
		}

		flashColor(red, green, blue, ms, ms, times);
	}

	void flashColor(uint8_t red, uint8_t green, uint8_t blue, int msOn, int msOff, int times)
	{
		SetColor(red, green, blue);
		flash(msOn, msOff, times);		
	}

	void flash(int msOn, int msOff, int times)
	{
		/*isFlashing = true;
		
		blinkIndex = 0;
		blinkCount = times;
		
		flashOnTimer.SetTimerLength(msOn);
		flashOffTimer.SetTimerLength(msOff, false);*/
		ledRed.Flash(msOn, msOff, times);
		ledGreen.Flash(msOn, msOff, times);
		ledBlue.Flash(msOn, msOff, times);

		//for (blinkIndex = 0; i < times; i++)
		{
			UpdateColor();
			//delay(msOn);
			//turnOff();
			//delay(msOff);
		}

		//setColor(0, 0, 0);
		//turnOff();

		//Serial.println("Starting blinking...");
	}

	void UpdateBlink()
	{

	}

	void TurnOn()
	{
		UpdateColor();
	}

	void TurnOff()
	{
		ledRed.TurnOff();
		ledGreen.TurnOff();
		ledBlue.TurnOff();
		/*analogWrite(pinRed, 255);
		analogWrite(pinGreen, 255);
		analogWrite(pinBlue, 255);*/
	}

	void UpdateColor()
	{
		ledRed.UpdateColor();
		ledGreen.UpdateColor();
		ledBlue.UpdateColor();

		//analogWrite(pinRed,   255 - (valueRed	* (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
		//analogWrite(pinGreen, 255 - (valueGreen	* (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
		//analogWrite(pinBlue,  255 - (valueBlue	* (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
	}

	uint8_t getRed() const
	{
		return ledRed.value;
	}

	void setRed(uint8_t val)
	{
		ledRed.value = val;
		UpdateColor();
	}

	uint8_t getGreen() const
	{
		return ledGreen.value;
	}

	void setGreen(uint8_t val)
	{
		ledGreen.value = val;
		UpdateColor();
	}

	uint8_t getBlue() const
	{
		return ledBlue.value;
	}

	void setBlue(uint8_t val)
	{
		ledBlue.value = val;
		UpdateColor();
	}

	void SetColor(uint8_t red, uint8_t green, uint8_t blue)
	{
		setRed(red);
		setGreen(green);
		setBlue(blue);
	}

	bool colorEquals(uint8_t red, uint8_t green, uint8_t blue) const
	{
		return (red == getRed() && green == getGreen() && blue == getBlue());
	}

	void getColor(uint8_t& red, uint8_t& green, uint8_t& blue) const
	{
		red = getRed();
		green = getGreen();
		blue = getBlue();
	}

	/*uint16_t GetBrightness() const
	{
		return brightness;
	}*/

	/*void SetBrightness(uint8_t b)
	{
		brightness = b;
		updateColor();
	}*/

	void PrintPins() const
	{
		Serial.print(F("Red Pin: "));
		Serial.print(ledRed.pin);

		Serial.print(F(", Green Pin: "));
		Serial.print(ledGreen.pin);

		Serial.print(F(", Blue Pin: "));
		Serial.println(ledBlue.pin);
	}

	/*void PrintColorComponents() const
	{
		Serial.print(F("Red: "));
		Serial.print(valueRed);

		Serial.print(F(", Green: "));
		Serial.print(valueGreen);

		Serial.print(F(", Blue: "));
		Serial.println(valueBlue);
	}*/

	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == "")
		{
			if (input == F("LED Color"))
			{
				ledRed.value = input.read();
				ledGreen.value = input.read();
				ledBlue.value = input.read();
				UpdateColor();
			}
		}
	}
};

#endif