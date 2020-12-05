// LEDToggleSwitch.h

#ifndef _LEDTOGGLESWITCH_h
#define _LEDTOGGLESWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SwitchDevice.h"

#define DEFAULT_LEDTOGGLESWITCH_MODE	true

class LEDToggleSwitch : public SwitchDevice	//SwitchDevice controls LED illumination on switch
{
 protected:
	 uint8_t pinInput;

 public:
	 LEDToggleSwitch()
	 {	 }

	 LEDToggleSwitch(uint8_t pinLED, uint8_t _pinInput)
		 : SwitchDevice(pinLED),
			pinInput(_pinInput)
	 {
		 pinMode(pinInput, INPUT);
	 }

	 LEDToggleSwitch(uint8_t pinLED, uint8_t _pinInput, const char* _name,
					bool onByDefault = DEFAULT_LEDTOGGLESWITCH_MODE,
					ESwitchDeviceMode deviceMode = ESwitchDeviceMode::eSDM_ManualOnly)
		 : SwitchDevice(pinLED, _name, onByDefault, deviceMode),
			pinInput(_pinInput)
	 {
		 pinMode(pinInput, INPUT);
	 }
	 
	 virtual void Init()
	 {
		 SwitchDevice::Init();
	 }

	 virtual void Update()
	 {
		 digitalWrite(pinNumber, isOn ? HIGH : LOW);
		 //SwitchDevice::Update();
	 }

	 bool SwitchIsFlippedUp() const
	 {
		 return digitalRead(pinInput) == HIGH;
	 }
};

//extern LEDToggleSwitch LEDToggleSwitch;

#endif

