#include "NavigationRecord.h"

extern uint8_t nav_buffer[256];
extern uint8_t nav_buffer_header;
extern uint8_t nav_buffer_tail;

NavigationMSG::NavigationMSG()
{
	_roll = 0;
	_pitch = 0;
	_yaw = 0;
	_t_roll = 0;
	_t_pitch = 0;
	_t_yaw = 0;
	_t_count = false;
	_t_parsed = false;
}

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

bool NavigationMSG::get_parsed()
{
	return _t_parsed;
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
		analysisMSG(nav_buffer[nav_buffer_header++]);
	}
	return;
}

void NavigationMSG::set_parsed(bool temp)
{
	_t_parsed = temp;
	return;
}

void NavigationMSG::analysisMSG(uint8_t temp)
{
	switch(_step)
	{
	case 0:
		if(temp == 0xFF)
		{
			_step = 0xFF;
		}
		else
		{
			_step = 0x00;
		}
		break;
	case 0xFF:
		if(temp == ',')
		{
			_step = 0x01;
			_t_count = 0;
		}
		else
		{
			_step = 0x00;
			_t_count = 0;
		}
		break;
	case 0x01:
		if(temp==',' && _t_count==2)
		{
			_t_count = 0;
			_step = 0x02;
		}
		else
		{
			_t_count++;
			_t_roll = _t_roll * 256 + (int16_t)temp; 
		}
		break;
	case 0x02:
		if(temp==',' && _t_count==2)
		{
			_step = 0x03;
			_t_count = 0;
		}
		else
		{
			_t_count++;
			_t_pitch = _t_pitch * 256 + (int16_t)temp;
		}
		break;
	case 0x03:
		if(temp==',' && _t_count==2)
		{
			_t_count = 0;
			_step = 0x04;
		}
		else
		{
			_t_count++;
			_t_yaw = _t_yaw * 256 + (uint16_t)temp;
		}
		break;
	case 0x04:
		if(temp==0xFA)
		{
			_t_parsed = true;
			_roll = _t_roll;
			_pitch = _t_pitch;
			_yaw = _t_yaw;
			_t_count = 0;
		}
		else
		{
			_t_parsed = false;
		}
		_step = 0x00;
		break;
	}
	return;
}
