// 
// 
// 

#include "Relay_8Channel.h"

uint8_t Relay_8Channel::Initialize()
{
	for (int r = 0; r < 8; r++)
	{
		if (switchDevice[r]->Initialize() != 0)
			return -1;
	}

	return 0;
}

uint8_t Relay_8Channel::Update()
{
	for (int r = 0; r < 8; r++)
	{
		switchDevice[r]->Update();
	}

	return 0;
}

//Relay_8ChannelClass Relay_8Channel;

