#include "NavigationRecord.h"

extern uint8_t nav_buffer[256];
extern uint8_t nav_buffer_header;
extern uint8_t nav_buffer_tail;

NavigationMSG::NavigationMSG()
{}

NavigationMSG::~NavigationMSG()
{}

int16_t NavigationMSG::get_roll()
{
	return _roll;
}

int16_t NavigationMSG::get_pitch()
{
	return _pitch;
}

uint16_t NavigationMSG::get_yaw()
{
	return _yaw;
}

void NavigationMSG::set_roll(int16_t temp)
{
	_roll = temp;
	return;
}

void NavigationMSG::set_pitch(int16_t temp)
{
	_pitch = temp;
	return;
}

void NavigationMSG::set_yaw(uint16_t temp)
{
	_yaw = temp;
	return;
}

// get data from the nav_buffer queue
void NavigationMSG::update()
{
	while(nav_buffer_header != nav_buffer_tail)
	{
		
	}
}

void NavigationMSG::analysisMSG(uint8_t temp)
{
	bool parsed = false;

	switch(_step)
	{
	case 0:
		if(temp == 0xFF)
		{
			
		}
	}

	return;
}
