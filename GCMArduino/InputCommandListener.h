// InputCommandListener.h

#ifndef _INPUTCOMMANDLISTENER_h
#define _INPUTCOMMANDLISTENER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "PlainProtocol.h"
//#include "RangedValue.h"

class IInputCommandListener
{
public:
	virtual void ProcessInput(PlainProtocol& input) = 0;
};

#endif

