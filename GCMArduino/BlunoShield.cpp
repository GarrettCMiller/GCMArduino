// 
// 
// 

#include "BlunoShield.h"
#include "ArduinoTimer.h"
#include "OLEDMenu.h"

BlunoShield blunoShield;

BlunoShield* BlunoShield::pInstance = NULL;

///////////////////////////////////////////////////////////////////////

PlainProtocol bleSerial(Serial, 115200);

//BlunoAccessory constructor,for setting the pOled
OLED oled;

//Buzzer constructor,for setting the buzzer
BlunoBuzzer myBuzzer;

//Relay constructor,for setting the Relay
SwitchDevice myRelay(relayPin, "Relay");

//Joystick constructor,for setting the joyStick
Joystick myJoy;

//Knob constructor,for setting the knob
Knob myKnob;

//Rgb constructor,for setting the RGB
LED_RGB myLED;

//DHT constructor,for setting the temperature, humidity
//DHT dht(DHTPIN, DHTTYPE);
//SimpleDHT11 dht;

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

ArduinoTimer oledTimer(50, true);

///////////////////////////////////////////////////////////////////////
void BlunoShield::Init()
{
	InitOLED();

	InitDHT();

	InitRelay();

	InitLED();

	InitBuzzer();

	InitJoystick();

	InitKnob();
}

void BlunoShield::Update()
{
	CheckDHT();

	UpdateOLED();
	
	UpdateRelay();

	UpdateKnob();

	UpdateJoystick();

	UpdatePlainProtocol();

	UpdateLED();
}

float BlunoShield::c2f(float tempC) const
{
	return tempC * 9.0f / 5.0f + 32.0f;
}

void BlunoShield::InitDHT()
{
	Serial.println(F("Initializing on-board DHT sensor..."));

	dhtBegin();

	Serial.println(F("Successfully initialized DHT sensor!"));
}

void BlunoShield::InitRelay()
{
	Serial.println(F("Initializing on-board relay..."));

	myRelay.Initialize();

	Serial.println(F("Successfully initialized relay!"));
}

void BlunoShield::InitLED()
{
	Serial.println(F("Initializing on-board LED..."));

	myLED.Initialize();
	myLED.setColor(0, 0, 0);
	myLED.turnOff();

	Serial.println(F("Successfully initialized LED!"));
}

void BlunoShield::InitOLED()
{
	oled.Initialize();
}

void BlunoShield::InitBuzzer()
{
	GetBuzzer().Initialize();
}

void BlunoShield::InitJoystick()
{
	myJoy.Initialize();
}

void BlunoShield::InitKnob()
{
	myKnob.Initialize();
}

void BlunoShield::InitMenuPages(OLEDMenu& menu)
{
	relaySettingsPage.Init(menu);
	ledSettingsPage.Init(menu);
	//buzzerSettingsPage.Init(menu);
	mainSettingsPage.Init(menu);
}

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

void BlunoShield::drawTempAndHumidity(void)           //Display pOled variable characters
{
	Serial.println("Draw screen");
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
		oled.firstPage();
		
		do
		{
			switch (drawMode)
			{
				case eDM_TempAndHumidity:
					drawTempAndHumidity();
					break;

				case eDM_Custom:
					if (currentDrawCallback != NULL)
						currentDrawCallback(oled);
					else
						drawTempAndHumidity();
					break;

				default:
					Serial.println(F("Invalid draw mode selected!"));
			}

		} while (oled.nextPage());
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
	//if (bleSerial.available())
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
	if (bleSerial.available())
	{
		GetOLED().ProcessInput(bleSerial);
		GetLED().ProcessInput(bleSerial);
		GetBuzzer().ProcessInput(bleSerial);
		GetRelay().ProcessInput(bleSerial);
		GetJoystick().ProcessInput(bleSerial);
		GetKnob().ProcessInput(bleSerial);
	}
}

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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void SetRelayState(bool on)
{
	BlunoShield* pShield = BlunoShield::pInstance;
	if (pShield != NULL)
		pShield->GetRelay().TurnOn(on);
}
void SetRelayName(String newName)
{
	//myRelay.GetName
}

RangedValueB relayValue = RangedValueB("Relay State", SetRelayState);
RangedString relayName = RangedString("Relay Name", SetRelayName);

MenuOption relaySettingsPageOptions[] =
{
	MenuOption("Name", relayName),
	MenuOption("State", relayValue),
};

OLEDPage relaySettingsPage("Relay Settings", MenuOptionCount(relaySettingsPageOptions), relaySettingsPageOptions);

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
MenuOption ledSettingsPageOptions[] =
{
	MenuOption("Red", myLED.valueRed),
	MenuOption("Green", myLED.valueGreen),
	MenuOption("Blue", myLED.valueBlue),
	MenuOption("Flash Delay", myLED.blinkDelay),
	MenuOption("Brightness", myLED.brightness),
	MenuOption("Blink Count", myLED.blinkCount),
};

OLEDPage ledSettingsPage("LED Settings", MenuOptionCount(ledSettingsPageOptions), ledSettingsPageOptions);

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//void PlayTestSound()
//{
//	myBuzzer.PlayZeldaSound();
//}
//
//MenuOption buzzerSettingsPageOptions[] =
//{
//	MenuOption("Test Buzzer", PlayTestSound),
//};

//OLEDPage buzzerSettingsPage(MenuOptionCount(buzzerSettingsPageOptions), buzzerSettingsPageOptions);

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

MenuOption mainSettingsPageOptions[] =
{
	MenuOption("LED Settings", &ledSettingsPage),
	MenuOption("Relay Settings", &relaySettingsPage),
	//MenuOption("Buzzer Settings", &buzzerSettingsPage),
};

OLEDPage mainSettingsPage("Shield Settings", MenuOptionCount(mainSettingsPageOptions), mainSettingsPageOptions);