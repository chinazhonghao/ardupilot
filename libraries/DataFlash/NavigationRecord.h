#ifndef NAVIGATIONRECORD
#define NAVIGATIONRECORD

#include "stdio.h"
#include <AP_Common/AP_Common.h>

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

	float get_vx() { return _vx; }
	float get_vy() { return _vy; }
	float get_vz() { return _vz; }

	float get_x() { return _x; }
	float get_y() { return _y; }
	float get_z() { return _z; }
	
	int32_t get_lat() { return _lat; }
	int32_t get_lng() { return _lng; }
	int32_t get_alt() { return _alt; }
	float get_gx() { return _gx; }
	float get_gy() { return _gy; }
	float get_gz() { return _gz; }

	void set_roll(int16_t temp);
	void set_pitch(int16_t temp);
	void set_yaw(uint16_t temp);
	void set_parsed(bool temp);

	void update();

	struct PACKED floatData
	{
		float vx;
		float vy;
		float vz;
		float x;
		float y;
		float z;
	};

	union PACKED
	{
		struct floatData data;
		uint8_t bytes[sizeof(struct floatData)];
	}_buffer;

private:
	void analysisMSG(uint8_t temp);

	int16_t _roll;
	int16_t _pitch;
	uint16_t _yaw;

	float _vx;
	float _vy;
	float _vz;

	float _x;
	float _y;
	float _z;

	int32_t _lat;
	int32_t _lng;
	int32_t _alt;
	float _gx;
	float _gy;
	float _gz;

	int16_t _t_roll;
	int16_t _t_pitch;
	uint16_t _t_yaw;
	bool _t_parsed;
	uint8_t _t_count;

	uint8_t _step;
};

#endif
