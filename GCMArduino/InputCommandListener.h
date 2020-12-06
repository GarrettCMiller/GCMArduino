// InputCommandListener.h

#ifndef _INPUTCOMMANDLISTENER_h
#define _INPUTCOMMANDLISTENER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "PlainProtocol.h"

/// <summary>
/// An interface that describes a function that listens to and/or processes input received from
/// (and possibly writes to) a <c>PlainProtocol</c> reference.
/// </summary>
class IInputCommandListener
{
public:
	/// <summary>
	/// The abstract (and only) function required by a child class by this class.
	/// </summary>
	/// <param name="input">A reference to the PlainProtocol object in use by the Bluno-board</param>
	virtual void ProcessInput(PlainProtocol& input) = 0;
};

#endif

