// MQSensorBase.h

#ifndef _MQSENSORBASE_h
#define _MQSENSORBASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ArduinoDevice.h"

#define CALIBRATE_MQ_SENSOR_SAMPLES 100

class MQSensorBase : public IArduinoDevice
{
	bool sensorCalibrationMode;

	uint8_t analogPin;
	uint8_t digitalPin;

	float sensorVolt;
	float RS_air; //  Rs in clean air 
	float R0;  // R0 in 1000 ppm LPG 
	float sensorValue;
	float RS_gas;
	float ratio;

public:
	MQSensorBase(uint8_t _analogPin, uint8_t _digitalPin)
		: analogPin(_analogPin), digitalPin(_digitalPin), sensorCalibrationMode(false)
	{
	}

	MQSensorBase(uint8_t _analogPin, bool calibrateSensor = false)
		: analogPin(_analogPin), digitalPin(-1), sensorCalibrationMode(calibrateSensor)
	{
	}

	virtual uint8_t Initialize()
	{
		return 0;
	}

	virtual uint8_t Update()
	{
		if (sensorCalibrationMode)
			CalibrateSensor();
		else
			ReadSensor();

		return 0;
	}

	virtual void CalibrateSensor()
	{
		sensorValue = 0.0f;

		//Average   
		for (int x = 0; x < CALIBRATE_MQ_SENSOR_SAMPLES; x++)
			sensorValue = sensorValue + analogRead(analogPin);

		sensorValue = sensorValue / /*(float)*/CALIBRATE_MQ_SENSOR_SAMPLES;

		sensorVolt = (sensorValue / 1024) * 5.0f;
		RS_air = (5.0f - sensorVolt) / sensorVolt; // Depend on RL on your module 
		R0 = RS_air / 9.9f; // According to MQ9 datasheet table 
	}

	virtual void ReadSensor()
	{
		int sensorValue = analogRead(analogPin);
		sensorVolt = (sensorValue / 1024.0f) * 5.0f;
		RS_gas = (5.0f - sensorVolt) / sensorVolt; // Depend on RL on your module 
		ratio = RS_gas / R0; // ratio = RS/R0 
	}

	virtual void ProcessInput(PlainProtocol& input)
	{

	}
};

//extern MQSensorBase MQSensorBase;

#endif

