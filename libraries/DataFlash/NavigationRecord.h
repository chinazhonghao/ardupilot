#ifndef NAVIGATIONRECORD
#define NAVIGATIONRECORD

#include "stdio.h"
#include <AP_Common/AP_Common.h>

struct PACKED NAVIGATION_MSG
{
	int16_t roll;
	int16_t pitch;
	uint16_t yaw;
	int16_t vx;
	int16_t vy;
	int16_t vz;
	float px;
	float py;
	int16_t pz;
};

// define the structure to store the data
class NavigationMSG
{
public:
	NavigationMSG();
	~NavigationMSG();

	int16_t get_roll() { return _roll; }
	int16_t get_pitch() { return _pitch; }
	uint16_t get_yaw() { return _yaw; }
	bool is_attitude_parsed() { return _attitude_parsed; }
	bool is_pos_parsed() { return _pos_parsed; }

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

	void set_roll(int16_t temp) { _roll = temp; }
	void set_pitch(int16_t temp) { _pitch = temp; }
	void set_yaw(uint16_t temp) { _yaw = temp; }
	void set_attitude_parsed(bool temp) { _attitude_parsed = temp; }
	void set_pos_parsed(bool temp) { _pos_parsed = temp; }

	void update();

	union PACKED
	{
		struct NAVIGATION_MSG data;
		uint8_t bytes[sizeof(struct NAVIGATION_MSG)];
	}_buffer;

	enum NAVIGATION_PROTOCOL_BYTES
	{
		PREAMBLE1 = 0xB5,
		PREAMBLE2 = 0x62,
		CLASS_NAV = 0x01,
		CLASS_SENSOR = 0x02,
		MSG_NAV_MSG = 0xFE
	};

private:
	void analysisMSG(uint8_t temp);
	bool _parse_navigation();

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

	bool _attitude_parsed;
	bool _pos_parsed;

	uint8_t _step;
	uint8_t _class;
	uint8_t _msg_id;
	uint8_t _ck_a;
	uint8_t _ck_b;
	uint16_t _payload_length;
	uint16_t _payload_counter;
};

#endif
