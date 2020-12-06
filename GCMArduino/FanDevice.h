// FanDevice.h

#ifndef _FANDEVICE_h
#define _FANDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SwitchDevice.h>

class FanDevice : public SwitchDevice
{
 protected:
	 int minTempToStart = 75;

 public:
	virtual uint8_t Initialize();
	virtual uint8_t Update();
};

//extern FanDeviceClass FanDevice;

#endif

