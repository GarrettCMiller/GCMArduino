// BlunoShield.h

#ifndef _BLUNOSHIELD_h
#define _BLUNOSHIELD_h


#ifdef ARDUINO_ARCH_ARC32
#include <BLEAttribute.h>
#include <BLECentral.h>
#include <BLECharacteristic.h>
#include <BLECommon.h>
#include <BLEDescriptor.h>
#include <BLEPeripheral.h>
#include <BLEService.h>
#include <BLETypedCharacteristic.h>
#include <BLETypedCharacteristics.h>
#include <BLEUuid.h>
#include "BLESerial.h"   
#endif

#include <Wire.h>
#include <PlainProtocol.h>
#include <string.h>
//#include <BlunoAccessory.h>
#include <Arduino.h>
#include <LED_RGB.h>
#include "SwitchDevice.h"
#include "BlunoBuzzer.h"
#include "Joystick.h"
#include "Knob.h"
#include "BlunoOLED.h"
//#include "DHT.h"

//need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21
#define dhtPin 2
#define dhtType DHT11
#define dhtCount 6

#define joystickPin 0
#define knobPin 1
#define relayPin 11
#define buzzerPin 8
#define redLedPin 9
#define greenLedPin 10
#define blueLedPin 3

#define RES 6    //LED reset pin PIN6
#define DC 7     //LED DC pin PIN3

//#define RIGHT 1
//#define UP 2
//#define LEFT 3
//#define DOWN 4
//#define PUSH 5
//#define MID 0

enum EJoystickValues
{
	eJoy_None	= 0,
	eJoy_Right	= 1,
	eJoy_Up		= 2,
	eJoy_Left	= 3,
	eJoy_Down	= 4,
	eJoy_Push	= 5
};

#define DHTPIN 2
#define DHTTYPE DHT11

#define RES 6    //LED reset pin PIN6
#define DC 7     //LED DC pin PIN3

//extern PlainProtocol myBLUNO;
//
////BlunoAccessory constructor,for setting the OLED
//extern BlunoAccessory oled;
//
////Buzzer constructor,for setting the buzzer
//extern BlunoBuzzer myBuzzer;
//
////Relay constructor,for setting the Relay
//extern Relay myRelay;
//
////Joystick constructor,for setting the joyStick
//extern Joystick myJoy;
//
////Knob constructor,for setting the knob
//extern Knob myKnob;
//
////Rgb constructor,for setting the RGB
//extern Rgb myLED;
//
////DHT constructor,for setting the temperature, humidity
//extern DHT dht;

class OLEDMenu;

class BlunoShield
{
public:
	typedef void (*DrawCallback)(OLED& pOled);
	typedef void (*InputCallback)(PlainProtocol& input);

	DrawCallback currentDrawCallback;
	InputCallback currentInputCallback;

	enum DrawMode
	{
		eDM_TemperatureOnly,
		eDM_HumidityOnly,
		eDM_TempAndHumidity,

		//other kinds of "default" views?

		eDM_Custom,

		eDM_Default		= eDM_TempAndHumidity,
	};

	enum InputProcessingMode
	{
		eIPM_Internal,
		eIPM_External,
		eIPM_Both
	};

	static BlunoShield* pInstance;

protected:
	uint8_t data[6];
	bool tempChanged = false;
	String messageText;

	DrawMode drawMode;

	InputProcessingMode ipMode;

	//bool displayPowerLevel = false;

 public:
	byte humidity = -1;
	byte temperature = -1;
	

	unsigned long _lastreadtime;
	bool firstreading;

 public:
	BlunoShield(DrawCallback newDrawCallback = NULL, InputCallback newInputCallback = NULL,
				InputProcessingMode newIPM = eIPM_Both)
		: currentDrawCallback(newDrawCallback), currentInputCallback(newInputCallback),
		drawMode(eDM_Default), ipMode(eIPM_Internal), messageText("System Ready")
	{
		pInstance = this;

		if (newDrawCallback != NULL)
			drawMode = eDM_Custom;

		if (newInputCallback != NULL)
			ipMode = newIPM;
		else
			ipMode = eIPM_Internal;
	}

	void SetMessageText(String str)
	{
		messageText = str;
	}

	String GetMessageText() const
	{
		return messageText;
	}

	void Init();

	void InitDHT();

	void InitRelay();

	void InitLED();

	void InitMenuPages(OLEDMenu& menu);

	float readTemperature(bool S = false);
	float readHumidity(void);

	void drawTempAndHumidity(void);
	bool CheckDHT();
	void Update();

	void UpdateKnob();

	float c2f(float);

	void UpdatePlainProtocol();
	void ProcessPlainProtocol();

	void UpdateOLED();
	void UpdateJoystick();
	void UpdateLED();
	void UpdateRelay();

	uint8_t GetJoystickValue() const
	{
		return GetJoystick().GetValue();
	}

	uint16_t GetKnobValue() const
	{
		return GetKnob().GetValue();
	}

	void SetDrawMode(DrawMode newDrawMode)
	{
		drawMode = newDrawMode;
	}

	DrawMode GetDrawMode() const
	{
		return drawMode;
	}

	void SetInputProcessingMode(InputProcessingMode newIPM)
	{
		ipMode = newIPM;
	}

	InputProcessingMode GetInputProcessingMode() const
	{
		return ipMode;
	}

	bool TempChanged() const
	{
		return tempChanged;
	}
	
	long readVcc()
	{
		long result;
		// Read 1.1V reference against AVcc
		ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
		delay(2); // Wait for Vref to settle
		ADCSRA |= _BV(ADSC); // Convert
		while (bit_is_set(ADCSRA, ADSC));
		result = ADCL;
		result |= ADCH << 8;
		result = 1126400L / result; // Back-calculate AVcc in mV
		return result;
	}

	PlainProtocol& GetProtocol();
	OLED& GetOLED();
	BlunoBuzzer& GetBuzzer();
	SwitchDevice& GetRelay();
	Joystick& GetJoystick();
	Knob& GetKnob();
	LED_RGB& GetLED();
	//SimpleDHT11& GetDHT();

	uint8_t drawKnobMeter(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h);
	uint8_t drawMeter(uint16_t val, uint16_t max, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h);

protected:
	bool dhtRead(void);
	void dhtBegin(void);

	void InitOLED();
};

extern BlunoShield blunoShield;

class MenuOption;
class OLEDPage;

extern MenuOption relaySettingsPageOptions[];
extern OLEDPage relaySettingsPage;

extern MenuOption ledSettingsPageOptions[];
extern OLEDPage ledSettingsPage;

//extern MenuOption buzzerSettingsPageOptions[];
//extern OLEDPage buzzerSettingsPage;

extern MenuOption mainSettingsPageOptions[];
extern OLEDPage mainSettingsPage;

#endif

