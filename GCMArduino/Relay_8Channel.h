// Relay_8Channel.h

#ifndef _RELAY_8CHANNEL_h
#define _RELAY_8CHANNEL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SwitchDevice.h>

class Relay_8Channel
{
	static const int firstPinNumber = 24;

 protected:
	 SwitchDevice* switchDevice[8] = 
	 {
		 new SwitchDevice("Relay 1", firstPinNumber + 0),
		 new SwitchDevice("Relay 2", firstPinNumber + 1),
		 new SwitchDevice("Relay 3", firstPinNumber + 2),
		 new SwitchDevice("Relay 4", firstPinNumber + 3),
		 new SwitchDevice("Relay 5", firstPinNumber + 4),
		 new SwitchDevice("Relay 6", firstPinNumber + 5),
		 new SwitchDevice("Relay 7", firstPinNumber + 6),
		 new SwitchDevice("Relay 8", firstPinNumber + 7),
	 };

 public:
	uint8_t Initialize();
	uint8_t Update();
};

//extern Relay_8ChannelClass Relay_8Channel;

#endif

