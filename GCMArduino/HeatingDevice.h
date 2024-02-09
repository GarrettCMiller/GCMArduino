// HeatingDevice.h

#ifndef _HEATINGDEVICE_h
#define _HEATINGDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"
#include "TemperatureSensor.h"

class HeatingDevice : public SwitchDevice
{
 protected:
	 float maxTemp;
	 float minTemp;	//necessary?
	 ITemperatureSensor* pTempSensor;

 public:
	 HeatingDevice(uint8_t pin,
					const char* name = "Heater",
					unsigned long timerLength = 2 TMINUTES)
		 : SwitchDevice(pin, name, timerLength), pTempSensor(NULL)
	 {
	 }

	 HeatingDevice(uint8_t pin,
					ITemperatureSensor* _pTempSensor,
					float _minTemp, float _maxTemp,
					const char* name = "Heater",
					unsigned long timerLength = 2 TMINUTES)
		 : SwitchDevice(pin, name, timerLength), pTempSensor(_pTempSensor),
			minTemp(_minTemp), maxTemp(_maxTemp)
	 {
	 }

	 virtual uint8_t Initialize()
	 {
		 if (pTempSensor)
			 pTempSensor->Initialize();

		 return SwitchDevice::Initialize();
	 }

	 virtual uint8_t Update()
	 {
		 if (pTempSensor)
		 {
			 //Serial.println("Heater updating...");
			 float currentTemp = pTempSensor->GetTemperature();

			 Serial.print("Heater updating. Temp: "); Serial.println(currentTemp);
			 if (currentTemp < minTemp && !IsOn())
			 {
				 Serial.println("Heater on...");
				 //turn heater on
				 TurnOn();
			 }
			 else if (currentTemp > maxTemp && IsOn())
			 {
				 Serial.println("Heater off...");
				 //turn heater off
				 TurnOn(false);
			 }
		 }

		 return SwitchDevice::Update();
	 }

	 bool HasTemperatureSensor() const
	 {
		 return pTempSensor != NULL;
	 }

	 float GetMinTemp() const
	 {
		 return minTemp;
	 }

	 float GetMaxTemp() const
	 {
		 return maxTemp;
	 }
};

//extern HeatingDevice heatingDevice;

#endif

