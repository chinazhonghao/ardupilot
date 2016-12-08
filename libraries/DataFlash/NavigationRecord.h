#ifndef NAVIGATIONRECORD
#define NAVIGATIONRECORD

#include "stdio.h"

// define the structure to store the data
class NavigationMSG
{
public:
	NavigationMSG();
	~NavigationMSG();

	int16_t get_roll();
	int16_t get_pitch();
	uint16_t get_yaw();

	void set_roll(int16_t temp);
	void set_pitch(int16_t temp);
	void set_yaw(uint16_t temp);

private:
	int16_t roll;
	int16_t pitch;
	uint16_t yaw;
};

#endif
