// BlunoBuzzer.h

#ifndef _BLUNOBUZZER_h
#define _BLUNOBUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Buzzer.h"
#include "InputCommandListener.h"

enum EMusicalNotes
{
	noteA0, noteASharp0, noteBFlat0 = noteASharp0,
	noteB0,
	noteC1, noteCSharp1, noteDFlat1 = noteCSharp1,
	noteD1, noteDSharp1, noteEFlat1 = noteDSharp1,
	noteE1,
	noteF1, noteFSharp1, noteGFlat1 = noteFSharp1,
	noteG1, noteGSharp1, noteAFlat1 = noteGSharp1,

	noteA1, noteASharp1, noteBFlat1 = noteASharp1,
	noteB1,
	noteC2, noteCSharp2, noteDFlat2 = noteCSharp2,
	noteD2, noteDSharp2, noteEFlat2 = noteDSharp2,
	noteE2,
	noteF2, noteFSharp2, noteGFlat2 = noteFSharp2,
	noteG2, noteGSharp2, noteAFlat2 = noteGSharp2,

	noteA2, noteASharp2,
	noteB2,
	noteC3, noteCSharp3,
	noteD3, noteDSharp3,
	noteE3,
	noteF3, noteFSharp3,
	noteG3, noteGSharp3,

	noteA3, noteASharp3,
	noteB3,
	noteC4, noteCSharp4,
	noteD4, noteDSharp4,
	noteE4,
	noteF4, noteFSharp4,
	noteG4, noteGSharp4,

	noteA4, noteASharp4,
	noteB4,
	noteC5, noteCSharp5,
	noteD5, noteDSharp5,
	noteE5,
	noteF5, noteFSharp5,
	noteG5, noteGSharp5,

	noteA5, noteASharp5,
	noteB5,
	noteC6, noteCSharp6,
	noteD6, noteDSharp6,
	noteE6,
	noteF6, noteFSharp6,
	noteG6, noteGSharp6,

	noteA6, noteASharp6,
	noteB6,
	noteC7, noteCSharp7,
	noteD7, noteDSharp7,
	noteE7,
	noteF7, noteFSharp7,
	noteG7, noteGSharp7,

	noteA7, noteASharp7,
	noteB7,
	noteC8
};

class BlunoBuzzer : public Buzzer, public IInputCommandListener
{
 protected:
	 bool isPlaying = false;

 public:
	void Init()
	{
		pinMode(buzzerPin, OUTPUT);
		digitalWrite(buzzerPin, LOW);
		noTone(buzzerPin);
		isPlaying = false;
	}

	void Play(uint16_t freq, uint32_t duration = 0)
	{
		//isPlaying = true;
		tone(buzzerPin, freq, duration);
	}

	void Play(EMusicalNotes note, uint16_t noteLength = 200)
	{
		//isPlaying = true;
		tone(buzzerPin, playNote(note), noteLength/* * tempoScale*/);
	}

	void Stop()
	{
		noTone(buzzerPin);
	}

	bool IsPlaying() const
	{
		return isPlaying;
	}

	void PlayZeldaSound()
	{
		uint8_t noteLength = 150;
		uint16_t delayTime = 175;
		Play(EMusicalNotes::noteG5, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteFSharp5, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteDSharp5, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteA4, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteGSharp4, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteE5, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteGSharp5, noteLength);
		delay(delayTime);
		Stop();
		Play(EMusicalNotes::noteC6, noteLength);
		delay(delayTime);
	}

private:
	double playNote(EMusicalNotes note)
	{
		static const double powerA = 1.059463094359;
		static const double OneTwelvth = 1.0 / 12.0;
		return 27.5 * pow(2, ((uint16_t)note) * OneTwelvth);
	}

public:
	virtual void ProcessInput(PlainProtocol& input)
	{
		if (input == "Sound")
		{
			uint8_t stepsAboveA0 = input.read();

			if (stepsAboveA0 >= noteA0 && stepsAboveA0 <= noteC8)
			{
				uint16_t length = input.read();
				if (length > 0 && length <= 2000)
					Play(stepsAboveA0, length);
			}
		}
	}
};

#endif

