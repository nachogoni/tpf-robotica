// Class automatically generated by Dev-C++ New Class wizard

#include "DCMotorBoardPacketHandler.h" // class's header file
#include "DCMotorPacket.h"
#include <math.h>

// class constructor
DCMotorBoardPacketHandler::DCMotorBoardPacketHandler(PacketServer * ps, char groupid, char boardid)
{
	this->groupid = groupid;
	this->boardid = boardid;
	this->ps = ps;
}

// class destructor
DCMotorBoardPacketHandler::~DCMotorBoardPacketHandler()
{
	// insert your code here
}

void DCMotorBoardPacketHandler::handlePacket(Packet * p){
}

void DCMotorBoardPacketHandler::setSpeed(double value){
	DCMotorPacket * p = new DCMotorPacket(groupid,boardid);
	// TODO convert from double to short
	short aux = (short) floor(value);
	p->setDCSpeed(true, aux);
	p->prepareToSend();
	this->ps->sendPacket(p);
}

void DCMotorBoardPacketHandler::enableEncoder(){
	DCMotorPacket * p = new DCMotorPacket(groupid,boardid);
	p->setInit();
	p->prepareToSend();
	this->ps->sendPacket(p);
}

double DCMotorBoardPacketHandler::getEncoder(){
	return 2.0;
}

void DCMotorBoardPacketHandler::moveWheel(double value){
	DCMotorPacket * p = new DCMotorPacket(groupid,boardid);
	p->setInit();
	p->prepareToSend();
	this->ps->sendPacket(p);
}

void DCMotorBoardPacketHandler::moveWheel(double counts, double speed){

}

double DCMotorBoardPacketHandler::getMotorConsumption(){
	return 2.0;
}

double DCMotorBoardPacketHandler::getAverageConsumption(){
	return 2.0;
}
