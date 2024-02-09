// BlunoBuzzer.h

#ifndef _BLUNOBUZZER_h
#define _BLUNOBUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ArduinoDevice.h"

/// <summary>
/// The musical notes, as indexed by number of steps above A0, NOT COMPLETE
/// </summary>
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

/// <summary>
/// Represents the buzzer attatched to the board
/// </summary>
class BlunoBuzzer : public ArduinoDevice
{
	BlunoBuzzer(BlunoBuzzer&& ctrArg);
	BlunoBuzzer(const BlunoBuzzer& ctrArg);

	static const uint8_t buzzerPin = 8;
 protected:
	 bool isPlaying = false;

 public:
	BlunoBuzzer()
		: ArduinoDevice(buzzerPin, "Buzzer")
	{
		//Serial.println("BUZZER");
	}

	/// <summary>
	/// Sets the pin mode and immediately turns it off (just to be safe)
	/// </summary>
	virtual uint8_t Initialize()
	{
		//pinMode(buzzerPin, OUTPUT);
		digitalWrite(buzzerPin, LOW);
		Stop();
		isPlaying = false;
		return 0;
	}

	virtual uint8_t Update()
	{
		return 0;
	}

	/// <summary>
	/// Plays a certain frequency via <c>tone()</c> for a specified duration
	/// </summary>
	/// <param name="freq">The frequency to play</param>
	/// <param name="duration">The duration to play for</param>
	void Play(uint16_t freq, uint32_t duration = 0)
	{
		//isPlaying = true;
		tone(buzzerPin, freq, duration);
	}

	/// <summary>
	/// Plays a certain musical note, for a specified duration
	/// </summary>
	/// <param name="note">The note to play, see <see cref="EMusicalNotes"/></param>
	/// <param name="noteLength">The duration to play for</param>
	void Play(EMusicalNotes note, uint16_t noteLength = 200)
	{
		//isPlaying = true;
		tone(buzzerPin, convertNoteToFrequency(note), noteLength/* * tempoScale*/);
	}

	/// <summary>
	/// Stop any sound from coming out of the device
	/// </summary>
	void Stop()
	{
		noTone(buzzerPin);
	}

	/// <summary>
	/// Whether or not the buzzer is playing. DO NOT USE right now, not accurate at all
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() const
	{
		return isPlaying;
	}

	/// <summary>
	/// Plays (supposedly) the sound from the original Legend Of Zelda when you uncover a secret.
	/// </summary>
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
	double convertNoteToFrequency(EMusicalNotes note)
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

