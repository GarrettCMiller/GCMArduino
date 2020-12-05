// 
// 
// 

#include "Relay_8Channel.h"

void Relay_8Channel::Init()
{
	for (int r = 0; r < 8; r++)
	{
		switchDevice[r]->Init();
	}
}

void Relay_8Channel::Update()
{
	for (int r = 0; r < 8; r++)
	{
		switchDevice[r]->Update();
	}
}

//Relay_8ChannelClass Relay_8Channel;

