/*
 * ZDGPS.h
 *
 *  Created on: 2016年4月17日
 *      Author: hao
 */

#ifndef LIBRARIES_AP_DGPS_ZDGPS_H_
#define LIBRARIES_AP_DGPS_ZDGPS_H_

#include <AP_HAL/AP_HAL.h>
#include <inttypes.h>

#include <AP_SerialManager/AP_SerialManager.h>

//默认的接收缓冲区长度
#define MAXRECEIVEDATA 64

class Z_DGPS
{
public:
	Z_DGPS(const AP_SerialManager &serialManager);
//    Z_DGPS();
	~Z_DGPS();

	void init();
    void setSerialCom(AP_HAL::UARTDriver* p);

	//北、南、西、东
	enum SemispherePosition
	{
		N = 0, S = 1, W = 2, E = 3
	};

	//度，分，秒
	typedef struct Z_Location
	{
		uint8_t dd;
		float mm;
	} Z_Location;

	typedef struct Z_NEPosition
	{
		SemispherePosition N_Pos;
		SemispherePosition E_Pos;
	} Z_NEPosition;

	typedef struct Z_DGPS_Status
	{
		uint8_t state;
		uint8_t num_sats;
		float altitude;
		float horizontal_accuracy;
		Z_Location Lattitude;
		Z_Location Longitude;
		Z_NEPosition NEPos;
	} Z_DGPS_Status;

	void setData(uint8_t *data, const uint8_t len);

	void update();
    //get ground altitude
    void calibrate();
    void getData();

	//状态获取接口
	float getAltitude()
	{
		return _status.altitude;
	}
	uint8_t getDGPSStatus()
	{
		return _status.state;
	}
	uint8_t getNumSats()
	{
		return _status.num_sats;
	}
	Z_Location getLattitude()
	{
		return _status.Lattitude;
	}
	Z_Location getLongitude()
	{
		return _status.Longitude;
	}
	Z_NEPosition getNEPos()
	{
		return _status.NEPos;
	}
	bool getDataIsOK()
	{
		return _dataIsOK;
	}

private:
	void _analyseData();
	void _analyseData(uint8_t data);
    void _copy2DGPSStatus();

	//读取数据帧格式的状态
	enum DataPosition
	{
		HEAD = 0,
		COMMA,
        SATSTATE,
		SATNUMBER,
		ALTITUDE,
		LATTITUDE,
		NS,
		LONGITUDE,
		WE,
		CHECK
	};

	uint8_t _receiveData[MAXRECEIVEDATA];
	uint8_t _dataLen;
	Z_DGPS_Status _status;
	//记录读取到的数据位置
	enum DataPosition _nextMessage;

	bool _dataIsOK;
	uint32_t _tempNum;
	uint8_t _floatLen;
    bool _isFloat;
	uint8_t _tempLen;
    
    float _groundAlt;

    uint8_t _tempState;
    uint8_t _tempNumSats;
    float _tempAlt;
    Z_Location _tempLatitude;
    Z_Location _tempLongitude;
    Z_NEPosition _tempNEPos;

	AP_HAL::UARTDriver* _serialCom;

	const AP_SerialManager& _serialManager;
};

#endif /* LIBRARIES_AP_DGPS_ZDGPS_H_ */
