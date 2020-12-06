// ArduinoDevice.h

#ifndef _ARDUINODEVICE_h
#define _ARDUINODEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "InputCommandListener.h"

#define USE_INIT_INSTEAD_OF_BEGIN
#define USE_UPDATE_INSTEAD_OF_LOOP

/// <summary>
/// An interface for defining objects that require initialization
/// and/or updating (every frame, or on a timer)
/// </summary>
/// <remarks>I chose <c>uint8_t</c> over int or other types because, except in
/// very specific and extreme cases, I can't imagine a function needing to
/// return MORE than 256 different answers.</remarks>
class IArduinoDevice : public IInputCommandListener
{
public:
#ifdef USE_INIT_INSTEAD_OF_BEGIN
	virtual uint8_t Initialize() = 0;
#else
	virtual uint8_t begin() = 0;
#endif

#ifdef USE_UPDATE_INSTEAD_OF_LOOP
	virtual uint8_t Update() = 0;
#else
	virtual uint8_t loop() = 0;
#endif
};

//#define ARDUINO_DEVICE_INIT_FUNCTION	\
//#ifdef USE_INIT_INSTEAD_OF_BEGIN		\
//	virtual uint8_t Initialize()		\
//#else									\
//	virtual uint8_t begin()				\
//#endif
//
//class TEST : public IArduinoDevice
//{
//	ARDUINO_DEVICE_INIT_FUNCTION
//	{
//
//	}
//};
#endif

