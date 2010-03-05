// Class automatically generated by Dev-C++ New Class wizard

#ifndef DISTANCESENSORPACKET_H
#define DISTANCESENSORPACKET_H

#include <packets/BoardPacket.h> // inheriting class's header file

#define CMD_ENABLE 0x40
#define CMD_DISABLE 0x41
#define CMD_SET_ALL 0x42
#define CMD_GET_VALUE 0x43
#define CMD_GET_ALL_VALUES 0x44
#define CMD_GET_SHOT_VALUE 0x45
#define CMD_GET_SHOT_ALL_VALUES 0x46

namespace packets {

/**
 * No description
 */
class DistanceSensorPacket : public packets::BoardPacket
{
	public:
		// class constructor
		DistanceSensorPacket(char groupid, char boardid);
		// class destructor
		~DistanceSensorPacket();

		// Commands to be sent
		void enableSensor(char sensorId);
		void disableSensor(char sensorId);
		void setAllSensors(char sensorIds);
		void getSensor(char sensorId);
		void getSensor();

		void getShotSensor(char sensorId);
		void getShotSensor();
		
		// Get data from package
		short getSensorValue();
		short * getSensorValues();
		short getShotSensorValue();
		short * getShotSensorValues();
};

}

#endif // DISTANCESENSORPACKET_H