// UltrasonicSensor.h

#ifndef _ULTRASONICSENSOR_h
#define _ULTRASONICSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ArduinoDevice.h"
#include "OLEDMenu.h"

#define SampleCount		50

#define ZeroDegCInK		273.15

class UltrasonicSensor : public ArduinoDevice
{
public:
	enum EDistanceUnits
	{
		eDU_Millimeters,
		eDU_Centimeters,
		eDU_Meters,

		eDU_Inches,
		eDU_Feet,
		eDU_Yards
	};
private:
	UltrasonicSensor();									//private default ctor
	UltrasonicSensor(UltrasonicSensor&& ctrArg);		//private move ctor
	UltrasonicSensor(const UltrasonicSensor& ctrArg);	//private copy ctor

protected:
	uint8_t trigPin;
	uint8_t echoPin;
	
	EDistanceUnits currentDistanceUnits = eDU_Centimeters;

	float currentAirTemperatureInF;

	long duration;
	float distance;

	//static const double ZeroCInK = 273.15;

public:
	/// <summary>
	/// Creates an ultrasonic sensor given the two data pins
	/// </summary>
	/// <param name="_trigPin">The pin of the "trig" wire</param>
	/// <param name="_echoPin">The pin of the "echo" wire</param>
	UltrasonicSensor(uint8_t _trigPin, uint8_t _echoPin, bool _enabled = true)
		: ArduinoDevice("USSensor"), trigPin(_trigPin), echoPin(_echoPin)
	{
		//enabled = _enabled;
	}

	/// <summary>
	/// Initializes the sensor by setting the pin modes
	/// </summary>
	/// <returns>0 if successful</returns>
	virtual uint8_t Initialize()
	{
		//if (!enabled)
		//	return 1;

		pinMode(trigPin, OUTPUT);
		pinMode(echoPin, INPUT);

		return 0;
	}

	/// <summary>
	/// Updates the sensor by sending out the trigger pulse and then listening
	/// for the echo pulse. Calculates the distance based on the duration.
	/// </summary>
	/// <returns>0 if successful</returns>
	virtual uint8_t Update()
	{
		//if (!enabled)
		//	return 1;

		duration = 0;

		for (int sampleIndex = 0; sampleIndex < SampleCount; sampleIndex++)
		{
			ClearTrigPin();

			SendTriggerPulse();

			duration += ListenForEcho();
		}

		duration /= SampleCount;

		CalculateDistance();

		return 0;
	}

	virtual void ProcessInput(PlainProtocol& pp)
	{

	}

	void SetCurrentTemperature(float temp)
	{
		currentAirTemperatureInF = temp;
	}

	void ClearTrigPin()
	{
		//Clears the trig pin
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
	}

	void SendTriggerPulse()
	{
		//set the trig pin to high for 10 µs
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
	}

	unsigned long ListenForEcho()
	{
		//Reads the echo pin, returns the soundwave travel time in µs
		/*duration =*/ return pulseIn(echoPin, HIGH);
	}

	double ConvertFToK(float degreesF)
	{
		return ((degreesF - 32) * 5.0 / 9.0) + ZeroDegCInK;
	}

	EDistanceUnits GetDistanceUnits() const
	{
		return currentDistanceUnits;
	}

	String GetDistanceUnitsLabel() const
	{
		switch (currentDistanceUnits)
		{
			case eDU_Millimeters:
			{
				return "mm";
			}
			break;

			case eDU_Centimeters:
			{
				return "cm";
			}
			break;

			case eDU_Meters:
			{
				return "m";
			}
			break;

			case eDU_Inches:
			{
				return "in";
			}
			break;

			case eDU_Feet:
			{
				return "ft";
			}
			break;

			case eDU_Yards:
			{
				return "yd";
			}
			break;
		}
	}

	void CycleDistanceUnits(bool up)
	{
		switch (currentDistanceUnits)
		{
			case eDU_Millimeters:
			{
				currentDistanceUnits = up ? eDU_Yards : eDU_Centimeters;
			}
			break;

			case eDU_Centimeters:
			{
				currentDistanceUnits = up ? eDU_Millimeters : eDU_Meters;
			}
			break;

			case eDU_Meters:
			{
				currentDistanceUnits = up ? eDU_Centimeters : eDU_Inches;
			}
			break;

			case eDU_Inches:
			{
				currentDistanceUnits = up ? eDU_Meters : eDU_Feet;
			}
			break;

			case eDU_Feet:
			{
				currentDistanceUnits = up ? eDU_Inches : eDU_Yards;
			}
			break;

			case eDU_Yards:
			{
				currentDistanceUnits = up ? eDU_Feet : eDU_Millimeters;
			}
			break;
		}

		Serial.print("Units: "); Serial.println(currentDistanceUnits);
	}

	void CalculateDistance(/*bool useTemp*/)
	{
		double degreesK = ConvertFToK(currentAirTemperatureInF);
		
		double speedOfSoundInKnots = 643.855 * sqrt(degreesK / ZeroDegCInK);

		//double speedOfSoundCMPMS = speedOfSoundInKnots * 0.0514444;
		double speedOfSoundMPS = speedOfSoundInKnots * 0.51444;// 340;
		double unitConversion = 1;

		switch (currentDistanceUnits)
		{
			case eDU_Millimeters:
			{
				unitConversion = (speedOfSoundMPS / 1000.0);
			}
			break;

			case eDU_Centimeters:
			{
				unitConversion = (speedOfSoundMPS / 10000.0);
			}
			break;

			case eDU_Meters:
			{
				unitConversion = (speedOfSoundMPS / 1000000.0);
			}
			break;

			case eDU_Inches:
			{
				unitConversion = (speedOfSoundMPS / 25400.0);
			}
			break;

			case eDU_Feet:
			{
				unitConversion = (speedOfSoundMPS / 304800.0);
			}
			break;

			case eDU_Yards:
			{
				unitConversion = (speedOfSoundMPS / 914400.0);
			}
			break;
		}

		//Calculate the distance
		distance = duration * unitConversion / 2.0;
	}

	float GetDistance() const
	{
		return distance;
	}
};

//OLEDPage ultrasonicSensorPage()

#endif

