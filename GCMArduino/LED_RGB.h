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

/// <summary>
/// The RGB LED on the Bluno Accessory Shield. Can be turned on/off with brightness adjustment, set to a solid color
/// or blinked a particular color a certain number of times (<c>blinkCount</c>), every <c>blinkDelay</c> ms
/// </summary>
class LED_RGB : public IArduinoDevice
{
public:
	enum ELEDState
	{
		eLS_Off,
		eLS_OnSolid,
		eLS_OnBlink
	};

	enum EFlashLength
	{
		eFL_Short,
		eFL_Medium,
		eFL_Long
	};

 //protected:
public:
	static const int flashLengthVeryShort = 20;
	static const int flashLengthShort = 50;
	static const int flashLengthMedium = 250;
	static const int flashLengthLong = 500;
	static const int flashDelayDefault = 2000;

	static const uint8_t pinRedDefault = 9;
	static const uint8_t pinGreenDefault = 10;
	static const uint8_t pinBlueDefault = 3;

	static const uint8_t valueRedDefault = 255;
	static const uint8_t valueGreenDefault = 255;
	static const uint8_t valueBlueDefault = 255;

	uint8_t pinRed;
	uint8_t pinGreen;
	uint8_t pinBlue;

	ELEDState eLEDState;

	RangedValue8 valueRed;
	RangedValue8 valueGreen;
	RangedValue8 valueBlue;

	RangedValue8 brightness;

	RangedValue8 blinkCount;
	RangedValue16 blinkDelay;

	ArduinoTimer flashDelayTimer;
	ArduinoTimer flashOnTimer;
	ArduinoTimer flashOffTimer;

protected:
	bool isFlashing;
	uint8_t blinkIndex;

 public:
	LED_RGB(ELEDState ledState = eLS_OnBlink) : eLEDState(ledState)
	{
		flashDelayTimer = ArduinoTimer(flashDelayDefault, true);
		flashDelayTimer.autoResetOnReady = false;
		flashOnTimer = ArduinoTimer(flashLengthVeryShort);
		flashOnTimer.autoResetOnReady = false;
		flashOffTimer = ArduinoTimer(flashLengthVeryShort);
		flashOffTimer.autoResetOnReady = false;

		valueRed = RangedValue8("Red", 0, 0, 255);
		valueGreen = RangedValue8("Green", 255, 0, 255);
		valueBlue = RangedValue8("Blue", 0, 0, 255);
		brightness = RangedValue8("Brightness", 1, 0, 255);
		blinkCount = RangedValue8("Blink Count", 2, 1, 10);
		blinkDelay = RangedValue16("Blink Delay", flashDelayTimer.timerLength, 100, 5000);
	}

	uint8_t Initialize();
	uint8_t Update();

	void flashColor(uint8_t red, uint8_t green, uint8_t blue, EFlashLength flashLength, int times)
	{
		isFlashing = true;

		int ms;
		switch (flashLength)
		{
			case eFL_Short:
				ms = flashLengthShort;
				break;

			case eFL_Medium:
				ms = flashLengthMedium;
				break;

			case eFL_Long:
				ms = flashLengthLong;
				break;
		}

		flashColor(red, green, blue, ms, ms, times);
	}

	void flashColor(uint8_t red, uint8_t green, uint8_t blue, int msOn, int msOff, int times)
	{
		setColor(red, green, blue);
		flash(msOn, msOff, times);		
	}

	void flash(int msOn, int msOff, int times)
	{
		isFlashing = true;
		
		blinkIndex = 0;
		blinkCount = times;
		
		flashOnTimer.SetTimerLength(msOn);
		flashOffTimer.SetTimerLength(msOff, false);

		//for (blinkIndex = 0; i < times; i++)
		{
			updateColor();
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

	void turnOn()
	{
		updateColor();
	}

	void turnOff()
	{
		analogWrite(pinRed, 255);
		analogWrite(pinGreen, 255);
		analogWrite(pinBlue, 255);
	}

	void updateColor()
	{
		analogWrite(pinRed,   255 - (valueRed	* (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
		analogWrite(pinGreen, 255 - (valueGreen	* (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
		analogWrite(pinBlue,  255 - (valueBlue	* (min(max(brightness, 0), MAX_BRIGHTNESS) / (float)MAX_BRIGHTNESS)));
	}

	uint8_t getRed() const
	{
		return valueRed;
	}

	void setRed(uint8_t val)
	{
		valueRed = val;
		updateColor();
	}

	uint8_t getGreen() const
	{
		return valueGreen;
	}

	void setGreen(uint8_t val)
	{
		valueGreen = val;
		updateColor();
	}

	uint8_t getBlue() const
	{
		return valueBlue;
	}

	void setBlue(uint8_t val)
	{
		valueBlue = val;
		updateColor();
	}

	void setColor(uint8_t red, uint8_t green, uint8_t blue)
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

	uint16_t GetBrightness() const
	{
		return brightness;
	}

	void SetBrightness(uint8_t b)
	{
		brightness = b;
		updateColor();
	}

	void PrintPins() const
	{
		Serial.print(F("Red Pin: "));
		Serial.print(pinRed);

		Serial.print(F(", Green Pin: "));
		Serial.print(pinGreen);

		Serial.print(F(", Blue Pin: "));
		Serial.println(pinBlue);
	}

	void PrintColorComponents() const
	{
		Serial.print(F("Red: "));
		Serial.print(valueRed);

		Serial.print(F(", Green: "));
		Serial.print(valueGreen);

		Serial.print(F(", Blue: "));
		Serial.println(valueBlue);
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == "")
		{
			if (input == F("LED Color"))
			{
				valueRed = input.read();
				valueGreen = input.read();
				valueBlue = input.read();
				updateColor();
			}
		}
	}
};

#endif