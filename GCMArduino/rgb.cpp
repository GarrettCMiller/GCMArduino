/*
This file is separate RGB control

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
#include <rgb.h>


void Rgb::setRGBLed(int red, int green, int blue){
	analogWrite(redLedPin,255 - red);
	analogWrite(greenLedPin,255 - green);
	analogWrite(blueLedPin,255 - blue);
}
