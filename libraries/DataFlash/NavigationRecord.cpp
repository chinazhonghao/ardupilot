#include "NavigationRecord.h"

NavigationMSG::NavigationMSG()
{}

NavigationMSG::~NavigationMSG()
{}

int16_t NavigationMSG::get_roll()
{
	return roll;
}

int16_t NavigationMSG::get_pitch()
{
	return pitch;
}

uint16_t NavigationMSG::get_yaw()
{
	return yaw;
}

void NavigationMSG::set_roll(int16_t temp)
{
	roll = temp;
	return;
}

void NavigationMSG::set_pitch(int16_t temp)
{
	pitch = temp;
	return;
}

void NavigationMSG::set_yaw(uint16_t temp)
{
	yaw = temp;
	return;
}
