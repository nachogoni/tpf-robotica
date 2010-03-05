// Class automatically generated by Dev-C++ New Class wizard

#include <handlers/DistanceSensorBoardPacketHandler.h> // class's header file
#include <packets/DistanceSensorPacket.h>
#include <math.h>

namespace handlers {

// class constructor
DistanceSensorBoardPacketHandler::DistanceSensorBoardPacketHandler(PacketServer * ps, char groupid, char boardid)
{
	this->groupid = groupid;
	this->boardid = boardid;
	this->ps = ps;
	this->dsValue[0] = 0;
	this->dsValue[1] = 0;
	this->dsValue[2] = 0;
	this->dsValue[3] = 0;
	this->dsValue[4] = 0;
}

// class destructor
DistanceSensorBoardPacketHandler::~DistanceSensorBoardPacketHandler()
{
	// insert your code here
}

void DistanceSensorBoardPacketHandler::handlePacket(Packet * p){
	packets::DistanceSensorPacket * dcmp = new packets::DistanceSensorPacket(groupid,boardid);
	dcmp->analysePacket(p);
	
	if ( p->getCommand() == CMD_GET_ALL_VALUES ){
		short * value = dcmp->getSensorValues();
		// TODO convert from short to double
		// Lock Mutex
		this->dsValue[0] = value[0];
		this->dsValue[1] = value[1];
		this->dsValue[2] = value[2];
		this->dsValue[3] = value[3];
		this->dsValue[4] = value[4];
		// Release Mutex
	}
}

void DistanceSensorBoardPacketHandler::enable(int dsId){
	packets::DistanceSensorPacket * p = new packets::DistanceSensorPacket(groupid,boardid);
	// TODO convert from double to char
	p->enableSensor(dsId);
	p->prepareToSend();
	this->ps->sendPacket(p);
}

void DistanceSensorBoardPacketHandler::disable(int dsId){
	packets::DistanceSensorPacket * p = new packets::DistanceSensorPacket(groupid,boardid);
	// TODO convert from double to char
	p->disableSensor(dsId);
	p->prepareToSend();
	this->ps->sendPacket(p);
}

double DistanceSensorBoardPacketHandler::getValue(int dsId){
	// TODO Put timestamps to prevent flooding
   	packets::DistanceSensorPacket * p = new packets::DistanceSensorPacket(groupid,boardid);
	p->getSensor();
	p->prepareToSend();
	this->ps->sendPacket(p);
	return this->dsValue[dsId];
}

}