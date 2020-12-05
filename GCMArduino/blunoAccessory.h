
/* blunoAccessory library 

written by DFrobot Roy
*/

#ifndef blunoAccessory_H
#define blunoAccessory_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21
#define dhtPin 2
#define dhtType DHT11
#define dhtCount 6

#define joystickPin 0
#define knobPin 1
#define relayPin 11
#define buzzerPin 8
#define redLedPin 9
#define greenLedPin 10
#define blueLedPin 3

#define RES 6    //LED reset pin PIN6
#define DC 7     //LED DC pin PIN3

//enum EMusicalNotes
//{
//  noteA0,   noteASharp0,
//  noteB0,
//  noteC1,   noteCSharp1,
//  noteD1,   noteDSharp1,
//  noteE1,
//  noteF1,   noteFSharp1,
//  noteG1,   noteGSharp1,
//
//  noteA1,   noteASharp1,
//  noteB1,
//  noteC2,   noteCSharp2,
//  noteD2,   noteDSharp2,
//  noteE2,
//  noteF2,   noteFSharp2,
//  noteG2,   noteGSharp2,
//
//  noteA2,   noteASharp2,
//  noteB2,
//  noteC3,   noteCSharp3,
//  noteD3,   noteDSharp3,
//  noteE3,
//  noteF3,   noteFSharp3,
//  noteG3,   noteGSharp3,
//
//  noteA3,   noteASharp3,
//  noteB3,
//  noteC4,   noteCSharp4,
//  noteD4,   noteDSharp4,
//  noteE4,
//  noteF4,   noteFSharp4,
//  noteG4,   noteGSharp4,
//
//  noteA4,   noteASharp4,
//  noteB4,
//  noteC5,   noteCSharp5,
//  noteD5,   noteDSharp5,
//  noteE5,
//  noteF5,   noteFSharp5,
//  noteG5,   noteGSharp5,
//
//  noteA5,   noteASharp5,
//  noteB5,
//  noteC6,   noteCSharp6,
//  noteD6,   noteDSharp6,
//  noteE6,
//  noteF6,   noteFSharp6,
//  noteG6,   noteGSharp6,
//
//  noteA6,   noteASharp6,
//  noteB6,
//  noteC7,   noteCSharp7,
//  noteD7,   noteDSharp7,
//  noteE7,
//  noteF7,   noteFSharp7,
//  noteG7,   noteGSharp7,
//
//  noteA7,   noteASharp7,
//  noteB7,
//  noteC8
//};

class blunoAccessory
{
 private:
  uint8_t data[6];
  bool joystickChange;
  bool dhtRead(void);
  void ledBegin(void);
  void dhtBegin(void); 
  float convertCtoF(float);
  unsigned long _lastreadtime;
  bool firstreading;

 public:
  uint16_t notes[6] = {110, 220, 440, 880, 880 << 1, 880 << 2};
 public:
  void play(uint8_t note);
  //void play(EMusicalNotes note);
  void begin(void);
  float readTemperature(bool S=false);
  float readHumidity(void);
  int readKnob(void);
  int readJoystick(void);
  void setRelay(bool value=true);
  void setBuzzer(bool value=true);
  void playTone();
  void setRGBLed(int red=255, int green=255, int blue=255);
  bool joystickAvailable(void);
};
#endif
