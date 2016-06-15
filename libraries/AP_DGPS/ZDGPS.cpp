/*
 * ZDGPS.cpp
 *
 *  Created on: 2016Äê4ÔÂ17ÈÕ
 *      Author: hao
 */

#include "ZDGPS.h"

extern const AP_HAL::HAL& hal;

Z_DGPS::Z_DGPS(const AP_SerialManager &serialManager):_serialManager(serialManager)
{
	// TODO Auto-generated constructor stub
	_nextMessage = DataPosition::HEAD;
	_dataLen = 0;
	_dataIsOK = false;
	_tempNum = 0;
	_floatLen = 0;
	_isFloat = false;
	_tempLen = 0;
    _groundAlt = 0;
	_serialCom = NULL;
}

Z_DGPS::~Z_DGPS()
{
	// TODO Auto-generated destructor stub
}

void Z_DGPS::setSerialCom(AP_HAL::UARTDriver* p)
{
    _serialCom = p;
    return;
}

void Z_DGPS::init()
{
	_serialCom = _serialManager.find_serial(AP_SerialManager::SerialProtocol_DGPS,0);
	return;
}

void Z_DGPS::setData(uint8_t *data, const uint8_t len)
{
	if (len <= MAXRECEIVEDATA)
	{
		memcpy(_receiveData, data, len * sizeof(uint8_t));
		_dataLen = len;
		_analyseData();
	}
	else
	{
		//printf("too many datas\n");
	}
	return;
}

void Z_DGPS::update()
{
    getData();
    //hal.uartE->printf("\nalt=%.3f\n",_tempAlt);
    if(_dataIsOK)
    {
        _copy2DGPSStatus();
        //hal.uartE->printf("\nalt=%.3f\n",_tempAlt);
        _status.altitude = _tempAlt - _groundAlt;
    }
    return;
}

void Z_DGPS::getData()
{
	_dataIsOK = false;
	if (_serialCom != nullptr)
	{
		int16_t length = _serialCom->available();
		uint8_t tempData = 0;
        //hal.uartE->printf("DGPS\n");
		for (int16_t i = 0; i < length; i++)
		{
			tempData = _serialCom->read();
            //hal.uartE->printf("%x!",tempData); 
			_analyseData(tempData);
		}
	}
	return;
}

void Z_DGPS::calibrate()
{
    uint8_t tempNum = 0;
    _groundAlt = 0;
    hal.uartE->print("DGPS calibrate start");
    /*for(uint8_t i=0;i<3;i++)
    {
        getData();
        if(_dataIsOK)
        {
            _groundAlt += _tempAlt;
            tempNum++;
        }
    }*/
    while(true)
    {
        getData();
        if(_dataIsOK)
        {
            _groundAlt += _tempAlt;
            tempNum++;
            if(tempNum>5)
            {
                break;
            }
        }
    }
    if(tempNum!=0)
    {
        _groundAlt /= tempNum;
        hal.uartE->printf("groundAL=%f",_groundAlt);
        hal.uartE->print("DGPS calibrate OK");
    }
    else
    {
        hal.uartE->print("DGPS calibrate is wrong!");
    }
    return;
}

void Z_DGPS::_copy2DGPSStatus()
{
    _status.state = _tempState;
    _status.num_sats = _tempNumSats;
    _status.altitude = _tempAlt;
    _status.Lattitude = _tempLatitude;
    _status.Longitude = _tempLongitude;
    _status.NEPos = _tempNEPos;
    return;
}

void Z_DGPS::_analyseData()
{
	for (uint8_t i = 0; i < _dataLen; i++)
	{
		_analyseData(_receiveData[i]);
	}
	return;
}

void Z_DGPS::_analyseData(uint8_t tempData)
{
	switch (_nextMessage)
	{
	case DataPosition::HEAD:
		if (tempData == 0xFF)
		{
			_nextMessage = DataPosition::COMMA;
		}
		else
		{
			_nextMessage = DataPosition::HEAD;
		}
		break;
	case DataPosition::COMMA:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::SATSTATE;
            _tempNum = 0;
		}
		else
		{
			_nextMessage = DataPosition::HEAD;
		}
		break;
	case DataPosition::SATSTATE:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::SATNUMBER;
			_tempState = _tempNum;
			_tempNum = 0;
		}
		else
		{
            if(tempData>='0' && tempData<='9')
            {
                tempData = tempData - '0';
                _tempNum = _tempNum * 10 + tempData;
            }
		}
		break;
	case DataPosition::SATNUMBER:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::ALTITUDE;
			_tempNumSats = _tempNum;
			_tempNum = 0;
            _isFloat = false;
            _floatLen = 0;
		}
		else
		{
            if(tempData>='0' && tempData<='9')
            {
                tempData = tempData - '0';
                _tempNum = _tempNum * 10 + tempData;
            }
		}
		break;
	case DataPosition::ALTITUDE:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::LATTITUDE;
			_tempAlt = (float) _tempNum;
			for (uint8_t i = 0; i < _floatLen; i++)
			{
				_tempAlt /= 10.0;
			}
			_isFloat = false;
            _floatLen = 0;
			_tempNum = 0;
		}
		else
		{
			if (tempData == 0x2E)
			{
				_isFloat = true;
				_floatLen = 0;
				return;
			}
			if (_isFloat)
			{
				_floatLen++;
			}
            if(tempData>='0' && tempData<='9')
            {
                tempData = tempData - '0';
                _tempNum = _tempNum * 10 + tempData;
            }
            else
            {
                _tempNum *= 10;
            }
		}
		break;
	case DataPosition::LATTITUDE:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::NS;
            _tempLatitude.mm = _tempNum;
			for (uint8_t i = 0; i < _floatLen; i++)
			{
			    _tempLatitude.mm /= 10.0;
			}
			_tempNum = 0;
			_isFloat = false;
			_tempLen = 0;
		}
		else
		{
			if (_tempLen >= 2)
			{
				if (tempData == 0x2E || _tempLen == 2)
				{
					_floatLen = 0;
					_isFloat = true;
					return;
				}
                if(tempData>='0' && tempData<='9')
                {
                    tempData = tempData - '0';
                    _tempNum = _tempNum * 10 + tempData;
                }
				if (_tempNum != 0 && _isFloat)
				{
					_floatLen++;
				}
			}
			else
			{
				_tempLen++;
                if(tempData>='0' && tempData<='9')
                {
                    tempData = tempData - '0';
                    _tempNum = _tempNum * 10 + tempData;
                }
				if (_tempLen == 2)
				{
					_tempLatitude.dd = _tempNum;
					_tempNum = 0;
				}
			}
		}
		break;
	case DataPosition::NS:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::LONGITUDE;
		}
		else
		{
			if (tempData == 0x4E)
			{
				_tempNEPos.N_Pos = SemispherePosition::N;
			}
			else
			{
			    _tempNEPos.N_Pos = SemispherePosition::S;
			}
		}
		break;
	case DataPosition::LONGITUDE:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::WE;
			_tempLongitude.mm = _tempNum;
			for (uint8_t i = 0; i < _floatLen; i++)
			{
				_tempLongitude.mm /= 10.0;
			}
			_tempNum = 0;
			_isFloat = false;
			_tempLen = 0;
		}
		else
		{
			if (_tempLen >= 3)
			{
				if (tempData == 0x2E || _tempLen == 3)
				{
					_floatLen = 0;
					_isFloat = true;
					return;
				}
                if(tempData>='0' && tempData<='9')
                {
                    tempData = tempData - '0';
                    _tempNum = _tempNum * 10 + tempData;
                }
				if (_tempNum != 0 && _isFloat)
				{
					_floatLen++;
				}
			}
			else
			{
				_tempLen++;
                if(tempData>='0' && tempData<='9')
                {
                    tempData = tempData - '0';
                    _tempNum = _tempNum * 10 + tempData;
                }
				if (_tempLen == 3)
				{
					_tempLongitude.dd = _tempNum;
					_tempNum = 0;
				}
			}
		}
		break;
	case DataPosition::WE:
		if (tempData == 0x2C)
		{
			_nextMessage = DataPosition::CHECK;
		}
		else
		{
			if (tempData == 0x45)
			{
				_tempNEPos.E_Pos = SemispherePosition::E;
			}
			else
			{
				_tempNEPos.E_Pos = SemispherePosition::W;
			}
		}
		break;
	case DataPosition::CHECK:
		if (tempData == 0xFA)
		{
			_dataIsOK = true;
		}
		else
		{
			_dataIsOK = false;
		}
		_nextMessage = DataPosition::HEAD;
		break;
	default:
		_nextMessage = DataPosition::HEAD;
	}
	return;
}
