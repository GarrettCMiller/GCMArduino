// Stack.h

#ifndef _STACK_h
#define _STACK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//#define DEBUG_STACK

#define MAX_STACK_SIZE	8

#ifdef DEBUG_STACK
class OLEDPage;
#endif //  DEBUG_STACK

template <class _Type>
class Stack
{
	_Type stack[MAX_STACK_SIZE];
	uint8_t pos;

public:
	Stack<_Type>()
	{
#ifdef  DEBUG_STACK
		//Serial.println("Stack constructor");
#endif //  DEBUG_STACK
		memset(stack, 0, sizeof(_Type) * MAX_STACK_SIZE);
		pos = 0;
	}

	void Push(_Type obj)
	{
#ifdef  DEBUG_STACK
		if (Serial)
		{
			Serial.print("Pre-push:");
			Serial.println(pos);

			if (pos > MAX_STACK_SIZE)
				Serial.println("BAD INDEX!");
		}
#endif
		stack[pos++] = obj;
#ifdef  DEBUG_STACK
		if (Serial)
		{
			Serial.print("Pushed object #"); Serial.print(pos); Serial.println(" onto the stack.");
		}
		//Serial.print("\tObject: (parent of "); Serial.println(((OLEDPage*)(obj))->GetOption(0));
#endif //  DEBUG_STACK
	}

	uint8_t Count() const
	{
		return pos;
	}

	_Type Pop()
	{
#ifdef  DEBUG_STACK
		if (pos == 0)
			Serial.println("Stack is empty!");
		Serial.print("Popping object "); Serial.print(pos); Serial.println(" from the stack.");
#endif //  DEBUG_STACK
		return stack[--pos];
	}

	_Type Peek() const
	{
#ifdef  DEBUG_STACK
		if (pos <= 0)
		{
			Serial.println("Trying to peek on an empty stack!");
			return NULL;
		}
#endif //  DEBUG_STACK
		return stack[pos - 1];
	}
};

#ifdef DEBUG_STACK
#undef DEBUG_STACK
#endif // DEBUG_STACK


#endif

