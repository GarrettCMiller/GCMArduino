// 
// 
// 

#include "ReptileMister.h"

bool MistingValve::TurnOn(bool on)
{
	mistingDevice->TurnValveOn(*this, on);
	return SwitchDevice::TurnOn(on);
}