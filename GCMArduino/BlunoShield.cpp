// 
// 
// 

#include "BlunoShield.h"
#include "ArduinoTimer.h"
#include "OLEDMenu.h"
#include "TemperatureSensor.h"

#define DUAL_ESP_MEGA

#ifdef DUAL_ESP_MEGA
	#define PLAIN_PROTOCOL_SERIAL	Serial3
#else
	#define PLAIN_PROTOCOL_SERIAL	Serial
#endif

#pragma region Globals

/// <summary>
/// The ONE and ONLY BlunoShield object
/// </summary>
BlunoShield blunoShield;

/// <summary>
/// The Singleton pointer referring to the ONE and ONLY BlunoShield pointer
/// </summary>
BlunoShield* BlunoShield::pInstance = NULL;

/// <summary>
/// The Serial->PlainProtocol interface class
/// </summary>
PlainProtocol bleSerial(PLAIN_PROTOCOL_SERIAL, 115200);

/// <summary>
/// BlunoAccessory constructor for setting the pOled
/// </summary>
OLED oled;

/// <summary>
/// Buzzer constructor for setting the buzzer
/// </summary>
BlunoBuzzer myBuzzer;

/// <summary>
/// Relay constructor for setting the Relay
/// </summary>
SwitchDevice myRelay(relayPin, "Relay");

/// <summary>
/// Joystick constructor for setting the joyStick
/// </summary>
Joystick myJoy;

/// <summary>
/// Knob constructor for setting the knob
/// </summary>
Knob myKnob;

/// <summary>
/// RGB constructor for setting the RGB
/// </summary>
LED_RGB myLED;

#ifndef USE_OWN_DHT_CODE_INSTEAD_OF_LIBRARY

//DHT constructor,for setting the temperature, humidity
DHT dht(dhtPin, dhtType);
//SimpleDHT11 dht;

#endif // !USE_OWN_DHT_CODE_INSTEAD_OF_LIBRARY

#pragma endregion

#pragma region Global Object Accessors

PlainProtocol& BlunoShield::GetProtocol()
{
	return bleSerial;
}

OLED& BlunoShield::GetOLED()
{
	return oled;
}

BlunoBuzzer& BlunoShield::GetBuzzer()
{
	return myBuzzer;
}

SwitchDevice& BlunoShield::GetRelay()
{
	return myRelay;
}

Joystick& BlunoShield::GetJoystick()
{
	return myJoy;
}

Knob& BlunoShield::GetKnob()
{
	return myKnob;
}

LED_RGB& BlunoShield::GetLED()
{
	return myLED;
}

#ifndef USE_OWN_DHT_CODE_INSTEAD_OF_LIBRARY

DHT& BlunoShield::GetDHT()
{
	return dht;
}

#endif // !USE_OWN_DHT_CODE_INSTEAD_OF_LIBRARY

#pragma endregion

ArduinoTimer oledTimer(50, true);

///////////////////////////////////////////////////////////////////////
#pragma region BlunoShield methods

uint8_t BlunoShield::Initialize()
{
	Serial.println(F("BlunoShield::Initialize() begin..."));

	InitOLED();

#ifdef USE_OWN_DHT_CODE_INSTEAD_OF_LIBRARY
	InitDHT();
#else
	GetDHT().
#endif

	InitRelay();

	InitLED();

	InitBuzzer();

	InitJoystick();

	InitKnob();

	Serial.println(F("BlunoShield::Initialize() end"));

	return 0;
}

uint8_t BlunoShield::Update()
{
	CheckDHT();

	UpdateOLED();
	
	UpdateRelay();

	UpdateKnob();

	UpdateJoystick();

	UpdatePlainProtocol();

	UpdateLED();

	return 0;
}

#pragma region Initialization Methods

void BlunoShield::InitDHT()
{
	Serial.println(F("\tInitializing on-board DHT sensor..."));

	dhtBegin();

	Serial.println(F("\tSuccessfully initialized DHT sensor!"));
}

void BlunoShield::InitRelay()
{
	Serial.println(F("\tInitializing on-board relay..."));

	myRelay.Initialize();

	Serial.println(F("\tSuccessfully initialized relay!"));
}

void BlunoShield::InitLED()
{
	Serial.println(F("\tInitializing on-board LED..."));

	myLED.Initialize();
	myLED.SetColor(0, 0, 0);
	myLED.TurnOff();

	Serial.println(F("\tSuccessfully initialized LED!"));
}

void BlunoShield::InitOLED()
{
	oled.Initialize();
}

void BlunoShield::InitBuzzer()
{
	Serial.println(F("\tInitializing buzzer..."));

	GetBuzzer().Initialize();

	Serial.println(F("\tSuccessfully initialized buzzer!"));
}

void BlunoShield::InitJoystick()
{
	Serial.println(F("\tInitializing joystck..."));

	myJoy.Initialize();

	Serial.println(F("\tSuccessfuully initialized joystick!"));
}

void BlunoShield::InitKnob()
{
	Serial.println(F("\tInitializing knob..."));

	myKnob.Initialize();

	Serial.println(F("\tSuccessfully initialized knob!"));
}

void BlunoShield::InitMenuPages(OLEDMenu& menu)
{
	Serial.println(F("Initializing root menu pages..."));
	
	/*Serial.print("InitMenuPages: ");
	Serial.println(int(&menu), HEX);*/
	
	mainPage.Init(menu);
	relaySettingsPage.Init(menu);
	ledSettingsPage.Init(menu);
	buzzerSettingsPage.Init(menu);
	mainSettingsPage.Init(menu);

	Serial.println(F("Successfully initialized root menu pages!"));
}

#pragma endregion

#pragma region DHT

bool BlunoShield::dhtRead(void)
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	unsigned long currenttime;

	digitalWrite(dhtPin, HIGH);
	//  delay(250);

	currenttime = millis();
	if (currenttime < _lastreadtime)
	{
		// ie there was a rollover
		_lastreadtime = 0;
	}
	if (!firstreading && ((currenttime - _lastreadtime) < 2000))
	{
		return true; // return last correct measurement
		//delay(2000 - (currenttime - _lastreadtime));
	}
	firstreading = false;
	/*
	  Serial.print("Currtime: "); Serial.print(currenttime);
	  Serial.print(" Lasttime: "); Serial.print(_lastreadtime);
	*/
	_lastreadtime = millis();

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// now pull it low for ~20 milliseconds
	pinMode(dhtPin, OUTPUT);
	digitalWrite(dhtPin, LOW);
	delay(20);
	cli();
	digitalWrite(dhtPin, HIGH);
	delayMicroseconds(40);
	pinMode(dhtPin, INPUT);

	// read in timings
	for (i = 0; i < MAXTIMINGS; i++)
	{
		counter = 0;
		while (digitalRead(dhtPin) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
			{
				break;
			}
		}
		laststate = digitalRead(dhtPin);
		if (counter == 255) break;
		// ignore first 3 transitions
		if ((i >= 4) && (i % 2 == 0))
		{
			// shove each bit into the storage bytes
			data[j / 8] <<= 1;
			if (counter > dhtCount)
				data[j / 8] |= 1;
			j++;
		}
	}
	sei();
	// check we read 40 bits and that the checksum matches
	if ((j >= 40) &&
		(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
	{
		return true;
	}
	return false;
}

void BlunoShield::dhtBegin(void)
{
	// set up the pins!
	pinMode(dhtPin, INPUT);
	digitalWrite(dhtPin, HIGH);
	_lastreadtime = 0;
	firstreading = true;
}

float BlunoShield::readTemperature(bool S /*= false*/)
{
	float f;

	if (dhtRead())
	{
		switch (dhtType)
		{
		case DHT11:
			f = data[2];
			if (S)
				f = c2f(f);

			return f;
		case DHT22:
		case DHT21:
			f = data[2] & 0x7F;
			f *= 256;
			f += data[3];
			f /= 10;
			if (data[2] & 0x80)
				f *= -1;
			if (S)
				f = c2f(f);

			return f;
		}
	}
	
	Serial.println(F("Read fail"));
	
	return NAN;
}

float BlunoShield::readHumidity(void)
{
	float f;
	if (dhtRead())
	{
		switch (dhtType)
		{
		case DHT11:
			f = data[0];
			return f;
		case DHT22:
		case DHT21:
			f = data[0];
			f *= 256;
			f += data[1];
			f /= 10;
			return f;
		}
	}
	
	Serial.print(F("Read fail"));
	
	return NAN;
}

#pragma endregion

#pragma region Update

bool BlunoShield::CheckDHT()
{
	tempChanged = false;

	static ArduinoTimer timer(2 TSECONDS, true);

	if (timer.IsReady())
	{
		int16_t newTemp = (int16_t)readTemperature(true),
				newHumid = readHumidity();

		if (humidity != newHumid)
		{			
			humidity = newHumid;
			tempChanged = true;
		}

		if (temperature != newTemp)
		{			
			temperature = newTemp;
			tempChanged = true;
		}

		timer.Reset();
	}

	return tempChanged;
}

void BlunoShield::UpdateOLED()
{
	if (oledTimer.IsReady())
	{
		//Mandatory first call before rendering to OLED
		oled.firstPage();
		
		//Begin loop on 1st page
		do
		{
			switch (drawMode)
			{
				case eDM_TempAndHumidity:
				{
					drawTempAndHumidity();
				}
					break;

				case eDM_Custom:
				{
					if (currentDrawCallback != NULL)
						currentDrawCallback(oled);
					else
						drawTempAndHumidity();
				}
					break;

				default:
				{
					Serial.println(F("Invalid draw mode selected!"));
				}
			}

		} while (oled.nextPage()); //If there are more pages to draw, keep cycling
	}
}

void BlunoShield::UpdateRelay()
{
	myRelay.Update();
}

void BlunoShield::UpdateLED()
{
	myLED.Update();
}

void BlunoShield::UpdateJoystick()
{
	myJoy.Update();
}

void BlunoShield::UpdateKnob()
{
	myKnob.Update();
}

void BlunoShield::UpdatePlainProtocol()
{
	//Serial.println("Processing input update-pre");
	if (bleSerial.receiveFrame())
	{
		//Serial.println("Processing input update");
		switch (ipMode)
		{
			case eIPM_External:
			{
				if (currentInputCallback)
					currentInputCallback(bleSerial);
			}
			break;

			case eIPM_Internal:
			{
				ProcessPlainProtocol();
			}
			break;

			case eIPM_Both:
			{
				if (currentInputCallback != NULL)
					currentInputCallback(bleSerial);

				ProcessPlainProtocol();
			}
		}
	}
}

void BlunoShield::ProcessPlainProtocol()
{
	//if (bleSerial.receiveCommand())
	{
		GetOLED().ProcessInput(bleSerial);
		GetLED().ProcessInput(bleSerial);
		GetBuzzer().ProcessInput(bleSerial);
		GetRelay().ProcessInput(bleSerial);
		GetJoystick().ProcessInput(bleSerial);
		GetKnob().ProcessInput(bleSerial);
	}
}

#pragma endregion

#pragma region Draw Methods

uint8_t BlunoShield::drawKnobMeter(u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h)
{
	return drawMeter(GetKnobValue(), 1023, x, y, w, h);
}

uint8_t BlunoShield::drawMeter(uint16_t val, uint16_t max, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h)
{
	GetOLED().drawFrame(x, y, w, h);
	uint8_t width = val / ((float)max) * (w - 2);
	GetOLED().drawBox(x + 1, y + 1, width, h - 2);
	return width - (x + 1);
}

void BlunoShield::drawTempAndHumidity(void)           //Display pOled variable characters
{
	//Serial.println("Draw screen");
	oled.setFont(u8g_font_helvB24);
	oled.disableCursor();
	oled.drawRFrame(0, 0, 127, 34, 5);
	oled.setPrintPos(3, 29);	//set the print position
	oled.print(temperature);
	oled.write(0xB0);			//degree symbol

	oled.setPrintPos(61, 29);
	oled.print(humidity);		//show the humidity on pOled
	oled.print(F("%"));

	oled.setFont(u8g_font_helvB10);
	oled.setPrintPos(0, 60);
	oled.print(messageText);
	//drawKnobMeter(0, 35, 127, 63 - 34);
}

#pragma endregion

#pragma endregion
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#pragma region Built-In Menus
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#pragma region Relay Settings

#pragma region Callbacks

void SetRelayState(bool on)
{
	Serial.print(F("Turning relay ")); Serial.println(on ? F("on") : F("off"));
	BlunoShield* pShield = BlunoShield::pInstance;
	if (pShield != NULL)
		pShield->GetRelay().TurnOn(on);
}

void SetRelayName(String newName)
{
	//myRelay.GetName
}

void SetRelayTimerLength(uint16_t newTimerLength)
{
	Serial.print(F("Setting new timer length: "));
	Serial.println(newTimerLength);
	BlunoShield* pShield = BlunoShield::pInstance;
	if (pShield != NULL)
		pShield->GetRelay().timerOn.SetTimerLength(newTimerLength);
}

void SetRelayMode(uint8_t newMode)
{
	Serial.println(F("Setting mode..."));
}

#pragma endregion

#pragma region RangedValues

RangedValueB relayValue = RangedValueB("State", SetRelayState);
RangedString relayName = RangedString("Name", SetRelayName);
RangedValueU8 relayMode = RangedValueU8("Mode", SetRelayMode, 0, 3);
RangedValueU16 relayTimerLength = RangedValueU16("Timer Length", SetRelayTimerLength, 0, 60 TMINUTES);

#pragma endregion

#pragma region Menu and suboptions

MenuOption relaySettingsPageOptions[] =
{
	MenuOption("Name", relayName),
	MenuOption("State", relayValue),
	MenuOption("Mode", relayMode),
	MenuOption("Timer Length", relayTimerLength),
	MenuOption("Timer Value", myRelay.timerOn),
};

OLEDPage relaySettingsPage("Relay Settings", MenuOptionCount(relaySettingsPageOptions), relaySettingsPageOptions);

#pragma endregion

#pragma endregion

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#pragma region LED Settings

MenuOption ledSettingsPageOptions[] =
{
	MenuOption("Red", myLED.ledRed.value),
	MenuOption("Green", myLED.ledGreen.value),
	MenuOption("Blue", myLED.ledBlue.value),
	//MenuOption("Flash Delay", myLED.blinkDelay),
	//MenuOption("Brightness", myLED.brightness),
	//MenuOption("Blink Count", myLED.blinkCount),
};

OLEDPage ledSettingsPage("LED Settings", MenuOptionCount(ledSettingsPageOptions), ledSettingsPageOptions);

#pragma endregion

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#pragma region Buzzer Settings

void PlayTestSound()
{
	myBuzzer.PlayZeldaSound();
}

MenuOption buzzerSettingsPageOptions[] =
{
	MenuOption("Test Buzzer", PlayTestSound),
};

OLEDPage buzzerSettingsPage("Buzzer Settings", MenuOptionCount(buzzerSettingsPageOptions), buzzerSettingsPageOptions);

#pragma endregion

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#pragma region Main Settings Page

MenuOption mainSettingsPageOptions[] =
{
	MenuOption("LED Settings", &ledSettingsPage),
	MenuOption("Relay Settings", &relaySettingsPage),
	MenuOption("Buzzer Settings", &buzzerSettingsPage),
};

OLEDPage mainSettingsPage("Shield Settings", MenuOptionCount(mainSettingsPageOptions), mainSettingsPageOptions);
#pragma endregion

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#pragma region Main Menu Page
//The main menu page options
MenuOption mainPageOptions[] =
{
	MenuOption("Shield Settings",	&mainSettingsPage)
};

// The main menu page that is activated when the user
// presses the joystick and "exits" the default screen mode.
OLEDPage mainPage("Main Menu", MenuOptionCount(mainPageOptions), mainPageOptions);

#pragma endregion

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#pragma region Menu

//////////////////////////////////////////////////////////////////////////////
// The whole "singular" menu system for this sketch. THERE SHOULD ONLY EVER BE ONE PER SKETCH
OLEDMenu menu(mainPage, oled);

#pragma endregion

/////////////////////////////////////////////////////////////////////
#pragma endregion
/////////////////////////////////////////////////////////////////////