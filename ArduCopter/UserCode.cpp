/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "Copter.h"

uint8_t nav_buffer[512];
uint16_t nav_buffer_header = 0x00;
uint16_t nav_buffer_tail = 0x00;

#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
}
#endif

#ifdef USERHOOK_200HZLOOP
void Copter::userhook_200Hz()
{
	uint16_t data_num = hal.uartD->available();
	//hal.uartE->printf("num%d\n",data_num);
	for(uint16_t i=0; i<data_num;i++)
	{
		nav_buffer[nav_buffer_tail] = hal.uartD->read();
		//hal.uartE->printf("%x ",nav_buffer[nav_buffer_tail]);
		nav_buffer_tail++;
		if(nav_buffer_tail >= 512)
		{
			nav_buffer_tail = 0;
		}
	}
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
	nav_msg.update();
	// record some AHRS data
	DataFlash.Log_Write_NAVIGATION(ahrs, nav_msg);
	DataFlash.Log_Write_NAVIGATIONVP(ahrs, nav_msg);
	//hal.uartE->printf("vx%.3f,vy%.3f,vz%.3f,x%.3f,y%.3f,z%.3f\n", nav_msg.get_vx(),nav_msg.get_vy(),nav_msg.get_vz(),nav_msg.get_x(),nav_msg.get_y(),nav_msg.get_z());
	//hal.uartE->printf("r%d,p%d,y%.2f\n",nav_msg.get_roll(),nav_msg.get_pitch(),ahrs.yaw*57.3);
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif
