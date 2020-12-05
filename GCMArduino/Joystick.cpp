/*
This file is to run the game operating lever

September-December 2016

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
https://github.com/DFRobot/DFRobot_BlunoAccessory
*/
#include <Joystick.h>

/*********************************************************************
If the joystick's state is changed,it will retrun true
*********************************************************************/
boolean Joystick::joystickAvailable(void){
	
	readJoystick();
	delay(10);
	boolean temp=joystickChange;
	joystickChange=false;
	return temp;
}

/*********************************************************************
Return the state of Joystick
value 1: right
      2: up
      3: left
      4: down
      5: push
*********************************************************************/
int Joystick::readJoystick(void){
	
	int key_value = 0;
	int stickRaw = analogRead(joystickPin);
	delay(10);
	static int keyValueLast = 0;
#if defined	ARDUINO_AVR_UNO || defined ARDUINO_AVR_LEONARDO
  //stickRaw = map(stickRaw, 0, 700, 0, 1023);
#endif
  if(stickRaw < 70)
	  key_value = 1;  //right
  else if(70 <= stickRaw && stickRaw < 235)
	  key_value = 5;  //push
  else if(235 <= stickRaw && stickRaw < 420)
	  key_value = 2;  //up
  else if(420 <= stickRaw && stickRaw < 620)
      key_value=3;  //left
  else if(620 <= stickRaw && stickRaw < 900)
	  key_value = 4;  //dowm
  else
      key_value = 0;
  if (keyValueLast != key_value){joystickChange = true;keyValueLast = key_value;}
  return key_value;
}

