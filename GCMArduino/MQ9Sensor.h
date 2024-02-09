// MQ9Sensor.h

#ifndef _MQ9SENSOR_h
#define _MQ9SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MQSensorBase.h"

class MQ9Sensor : public MQSensorBase
{
public:
	MQ9Sensor(uint8_t _analogPin, uint8_t _digitalPin)
		: MQSensorBase(_analogPin, _digitalPin)
	{
	}

	MQ9Sensor(uint8_t _analogPin, bool calibrateSensor = false)
		: MQSensorBase(_analogPin, calibrateSensor) 
	{
	}

	uint8_t Initialize()
	{
		return MQSensorBase::Initialize();
	}

	uint8_t Update()
	{
		return MQSensorBase::Update();
	}

	virtual void ProcessInput(PlainProtocol& input)
	{
		MQSensorBase::ProcessInput(input);
	}
};

//extern MQ9Sensor MQ9Sensor;

#endif

