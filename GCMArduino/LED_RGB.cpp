#include "LED_RGB.h"
#include "OLEDMenu.h"



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//uint8_t LED_RGB::Initialize()
//{
	/*isFlashing = false;

	pinRed = pinRedDefault;
	pinGreen = pinGreenDefault;
	pinBlue = pinBlueDefault;

	pinMode(pinRed, OUTPUT);
	pinMode(pinGreen, OUTPUT);
	pinMode(pinBlue, OUTPUT);*/

	/*valueRed = valueRedDefault;
	valueGreen = valueGreenDefault;
	valueBlue = valueBlueDefault;*/

	//updateColor();
//}

///////////////////////////////////////////////////////////////////////////////

//uint8_t LED_RGB::Update()
//{
//	if (flashDelayTimer.timerLength != blinkDelay)
//		flashDelayTimer.SetTimerLength(blinkDelay);
//
//	if (isFlashing)
//	{
//		if (blinkIndex < blinkCount)
//		{
//			if (flashOnTimer.IsReady())
//			{
//				flashOnTimer.enabled = false;
//				//Serial.println("Flash on timer ready! Turning off...");
//				flashOffTimer.Reset();
//				blinkIndex++;
//				turnOff();
//			}
//			else if (flashOffTimer.IsReady())
//			{
//				flashOffTimer.enabled = false;
//				//Serial.println("Flash off timer ready! Turning on...");
//				flashOnTimer.Reset();				
//				
//				updateColor();
//			}
//		}
//		else
//		{
//			//Serial.println("Finished blinking!");
//			blinkIndex = 0;
//			isFlashing = false;
//
//			turnOff();
//
//			flashDelayTimer.Reset();
//		}
//	}
//	else if (eLEDState == eLS_OnBlink && flashDelayTimer.IsReady())
//	{
//		flash(flashOnTimer.timerLength, flashOffTimer.timerLength, blinkCount);
//	}
//}