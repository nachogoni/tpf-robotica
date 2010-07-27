// Class automatically generated by Dev-C++ New Class wizard

#include <protocol/packets/DistanceSensorPacket.h> // class's header file

namespace protocol {
namespace packets {


bool isBitSet(int val,int index){
	if(val & ( 0x01 << index)){
		return true;
	}
	
	return false;
}

// class constructor
DistanceSensorPacket::DistanceSensorPacket(char groupid, char boardid) : protocol::packets::BoardPacket(groupid, boardid)
{
}

// class destructor
DistanceSensorPacket::~DistanceSensorPacket()
{
	// insert your code here
}

void DistanceSensorPacket::on(char sensorId){
	this->addData(sensorId);
	this->setCommand(CMD_ON);
}

void DistanceSensorPacket::off(char sensorId){
	this->addData(sensorId);
	this->setCommand(CMD_OFF);
}

void DistanceSensorPacket::setMask(char sensorMask){
	this->addData(sensorMask);
	this->setCommand(CMD_SET_STATUS);
}

void DistanceSensorPacket::getMask(){
	this->setCommand(CMD_GET_STATUS);
}

void DistanceSensorPacket::getValue(char sensorIds){
	this->addData(sensorIds);
	this->setCommand(CMD_GET_VALUE);
}

void DistanceSensorPacket::getOneValue(char sensorIds){
	this->addData(sensorIds);
	this->setCommand(CMD_GET_ONE_VALUE);
}

void DistanceSensorPacket::alarmOn(char data){
	this->addData(data);
	this->setCommand(CMD_ALARM_ON);
}


//get values
short DistanceSensorPacket::getSensorValue(){
	return this->getShortData();
}

short * DistanceSensorPacket::getSensorValues(){
	// TODO
	short * values = new short[6];

	for (int i = 0; i < 6; i++)
		values[i] = 0;

	char mask = this->getCharData();
	
	// Get mask for data values
	for (int i = 0; i < 6; i++) {
		if (isBitSet(mask, i)) {
			values[i] = this->getShortData();
		}
	}

	return values;

/*
 * 	short * values = new short[6];

	for (int i = 0; i < 6; i++)
		values[i] = 0;

	int dataLength = this->getDataLength();
	char * data = this->getData();
	
	if (dataLength < 1)
		return values;

	char mask = data[0];
	
	// Skip mask
	data++;
	
	// Get mask for data values
	for (int i = 0; i < 6; i++) {
		if (isBitSet(mask, i)) {
			values[i] = *((short*)data);
			// Go to next value...
			data += 2;
		}
	}

	return values;

*/
}

short DistanceSensorPacket::getShotSensorValue(){
	return this->getSensorValue();
}

short * DistanceSensorPacket::getShotSensorValues(){
	return this->getSensorValues();
}

}
}
