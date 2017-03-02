#include "NavigationRecord.h"

#include "DataFlash_Backend.h"
#include "DFMessageWriter.h"

extern const AP_HAL::HAL& hal;

extern uint8_t nav_buffer[512];
extern uint16_t nav_buffer_header;
extern uint16_t nav_buffer_tail;

NavigationMSG::NavigationMSG()
{
	_roll = 0;
	_pitch = 0;
	_yaw = 0;
	_vx = 0.0;
	_vy = 0.0;
	_vz = 0.0;
	_x = 0.0;
	_y = 0.0;
	_z = 0.0;
	_lat = 0;
	_lng = 0;
	_alt = 0;
	_gx = 0;
	_gy = 0;
	_gz = 0;
	_attitude_parsed = false;
	_pos_parsed = false;
	_step = 0x00;
	_class = 0x00;
	_msg_id = 0x00;
	_ck_a = 0x00;
	_ck_b = 0x00;
	_payload_length = 0x00;
	_payload_counter = 0x00;
}

NavigationMSG::~NavigationMSG()
{}

// get data from the nav_buffer queue
void NavigationMSG::update()
{
	while(nav_buffer_header != nav_buffer_tail)
	{
		analysisMSG(nav_buffer[nav_buffer_header]);
		//hal.uartE->printf("s%x,d%x ",_step,nav_buffer[nav_buffer_header]);
		nav_buffer_header++;
		if(nav_buffer_header >= 512)
		{
			nav_buffer_header = 0;
		}
	}
	return;
}

void NavigationMSG::analysisMSG(uint8_t temp)
{
reset:
	switch(_step)
	{
	case 0x01:
		if(PREAMBLE2 == temp)
		{
			_step++;
			break;
		}
		_step = 0;
		// no break
	case 0x00:
		if(PREAMBLE1 == temp)
		{
			_step++;
		}
		break;
	case 0x02:
		_step++;
		_class = temp;
		_ck_a = temp;
		_ck_b = _ck_a;
		break;
	case 0x03:
		_step++;
		_msg_id = temp;
		_ck_a += temp;
		_ck_b += _ck_a;
		break;
	case 0x04:
		_step++;
		_ck_a += temp;
		_ck_b += _ck_a;
		_payload_length = temp;
		break;
	case 0x05:
		_step++;
		_ck_a += temp;
		_ck_b += _ck_a;
		_payload_length += (uint16_t)(temp<<8);
		if(_payload_length > sizeof(_buffer))
		{
			_payload_length = 0x00;
			_step = 0x00;
			goto reset;
		}
		_payload_counter = 0x00;
		break;
	case 0x06:
		_ck_a += temp;
		_ck_b += _ck_a;
		if(_payload_counter < sizeof(_buffer))
		{
			_buffer.bytes[_payload_counter] = temp;
		}
		if(++_payload_counter == _payload_length)
		{
			_step++;
		}
		break;
	case 0x07:
		_step++;
		if(_ck_a != temp)
		{
			_step = 0x00;
			goto reset;
		}
		break;
	case 0x08:
		_step = 0x00;
		if(_ck_b != temp)
		{
			break;
		}
		if(_parse_navigation())
		{
			_attitude_parsed = true;
			_pos_parsed = true;
		}
		break;
	}
	return;
}

bool NavigationMSG::_parse_navigation()
{
	if(_class == CLASS_NAV)
	{
		if(_msg_id == MSG_NAV_MSG)
		{
			_roll = _buffer.data.roll;
			_pitch = _buffer.data.pitch;
			_yaw = _buffer.data.yaw;
			_vx = _buffer.data.vx / 100.0f;
			_vy = _buffer.data.vy / 100.0f;
			_vz = _buffer.data.vz / 100.0f;
			_x = _buffer.data.px / 100.0;
			_y = _buffer.data.py / 100.0;
			_z = _buffer.data.pz / 100.0;
			return true;
		}
	}
	return false;
}
