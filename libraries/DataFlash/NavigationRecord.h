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
	bool get_parsed();

	void set_roll(int16_t temp);
	void set_pitch(int16_t temp);
	void set_yaw(uint16_t temp);
	void set_parsed(bool temp);

	void update();

private:
	void analysisMSG(uint8_t temp);

	int16_t _roll;
	int16_t _pitch;
	uint16_t _yaw;

	int16_t _t_roll;
	int16_t _t_pitch;
	uint16_t _t_yaw;
	bool _t_parsed;

	uint8_t _step;
};

#endif
