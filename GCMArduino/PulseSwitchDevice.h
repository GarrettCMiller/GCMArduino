// PulseSwitchDevice.h

#ifndef _PULSESWITCHDEVICE_h
#define _PULSESWITCHDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"

/// <summary>
/// A SwitchDevice that gets turned on and off by a single quick pulse
/// on the relay rather than a sustained HIGH
/// </summary>
class PulseSwitchDevice : public SwitchDevice
{
 protected:
	 bool deviceOn;

 public:
	 PulseSwitchDevice(uint8_t _pin, const char* _name)
		 : SwitchDevice(_pin, _name, SwitchDevice::eSDM_EventOff)
	 {

	 }

	 virtual uint8_t Initialize()
	 {
		 return 0;
	 }

	 virtual uint8_t Update()
	 {
		 return 0;
	 }
};

//extern PulseSwitchDevice PulseSwitchDevice;

#endif

