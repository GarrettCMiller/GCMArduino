// SoilSensor.h

#ifndef _SOILSENSOR_h
#define _SOILSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_seesaw.h>
#include "ArduinoDevice.h"
#include "BlunoShield.h"
#include "TemperatureSensor.h"

class SoilSensor : public IArduinoDevice
{
	Adafruit_seesaw soilSensor;
	uint16_t soilMoisture;
	int8_t localTemperture;
	uint8_t i2C_address;
 protected:


 public:
	SoilSensor(uint8_t address = 0x36) : i2C_address(address)
	{
	}

	virtual uint8_t Initialize()
	{
		if (!soilSensor.begin(i2C_address))
		{
			Serial.println("ERROR! seesaw not found");
			return -1;
		}
		
		Serial.print("seesaw started! version: ");
		Serial.println(soilSensor.getVersion(), HEX);
		
		return 0;
	}

	virtual uint8_t Update()
	{
		soilMoisture = soilSensor.touchRead(0);
		localTemperture = c2f(soilSensor.getTemp());
		return 0;
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
	}

	uint16_t GetSoilMoisture() const
	{
		return soilMoisture;
	}

	float GetSoilMoisturePct() const
	{
		return soilMoisture / 1024.0f * 100.0f;
	}

	int8_t GetLocalTemperature() const
	{
		return localTemperture;
	}

	void drawDefaultSensorData(OLED& oled)
	{
		/*static ArduinoTimer timer(5 TSECONDS, true);

		if (GetSoilMoisture() > 1000 && timer.IsReady())
			blunoShield.GetBuzzer().PlayZeldaSound();*/

		oled.setFont(u8g_font_helvB24);
		oled.disableCursor();
		oled.drawRFrame(0, 15, 127, 34, 5);
		oled.setPrintPos(3, 44);	//set the print position
		oled.print(GetSoilMoisturePct());
		oled.print(F("%"));

		oled.setFont(u8g_font_helvB10);
		oled.setPrintPos(0, 63);
		oled.print(GetLocalTemperature());		//show the humidity on pOled
		oled.write(0xB0);			//degree symbol

		oled.setFont(u8g_font_helvB10);
		oled.setPrintPos(0, 12);
		oled.print("Soil sensor");
	}
};

#endif

