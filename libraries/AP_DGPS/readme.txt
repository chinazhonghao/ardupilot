Introduction for using DGPS model
	
	1.add SerialProtocol(enum tyep) in AP_SerialManager/AP_SerialMananger.h
		named SerialProtocol_DGPS = 10
	2.modify the badu in AP_SerialManager/AP_SerialManager.cpp
		using the telem2 uartD in the program with baud 115200
		and SerialProtocal named SerialProtocal_DGPS
	3.add a variable name DGPS in Copter.h initialized with SerialManager to
		get the uartE pointer
	4.define function in copter.h named read_DGPS for reading dgps_alt defined in copter.h
	5.in update_altitude function call read_DGPS function to get the DGPS altitude