#include "RealRobot.h"

#include <robotapi/real/RealCamera.h>
#include <robotapi/real/RealDistanceSensor.h>
#include <robotapi/real/RealServo.h>
#include <robotapi/real/RealBattery.h>
#include <robotapi/real/RealDifferentialWheels.h>
#include <robotapi/real/RealTrashBin.h>
#include <robotapi/real/RealPCBattery.h>
#include <robotapi/real/RealDevice.h>

#define DC_GROUP 0x01
#define DC_LEFT_ID 0x00
#define DC_RIGHT_ID 0x01

#define SERVO_GROUP 0x02
#define SERVO_BOARD 0x00
#define SERVO_ONE_ID 0x00
#define SERVO_TWO_ID 0x01
#define SERVO_THREE_ID 0x02

#define DISTANCE_SENSOR_GROUP 0x03
#define DISTANCE_SENSOR_BOARD_ONE 0x00
#define DISTANCE_SENSOR_BOARD_TWO 0x01
#define DISTANCE_SENSOR_ONE_ID 0x00
#define DISTANCE_SENSOR_TWO_ID 0x01
#define DISTANCE_SENSOR_THREE_ID 0x02
#define DISTANCE_SENSOR_FOUR_ID 0x03
#define DISTANCE_SENSOR_FIVE_ID 0x04

#define BATTERY_GROUP 0x06
#define BATTERY_BOARD 0x00

#define TRASHBIN_GROUP 0x07
#define TRASHBIN_BOARD 0x00

namespace robotapi {
namespace real {

RealRobot::RealRobot(WorldInfo * wi){

	protocol::PacketServer * ps = new protocol::PacketServer();
	this->initBatteries(ps);
	this->initServos(ps);
	this->initWheels(ps);
	this->initTrashBins(ps);
	this->initDistanceSensors(ps);
	this->initCameras();
	this->wi = wi;
}

void RealRobot::initWheels(protocol::PacketServer * ps){
	protocol::handlers::DCMotorBoardPacketHandler * dcmbphleft = new protocol::handlers::DCMotorBoardPacketHandler(ps,DC_GROUP,DC_LEFT_ID);
	protocol::handlers::DCMotorBoardPacketHandler * dcmbphright = new protocol::handlers::DCMotorBoardPacketHandler(ps,DC_GROUP,DC_RIGHT_ID);
   	RealDifferentialWheels * rdw = new RealDifferentialWheels(this->wi,dcmbphleft,dcmbphright,"dw0");

	this->wheels.insert( std::pair<std::string, IDifferentialWheels *>(rdw->getName(),rdw) );
	ps->registerHandler(dcmbphleft,DC_GROUP,DC_LEFT_ID);
	ps->registerHandler(dcmbphright,DC_GROUP,DC_RIGHT_ID);
}

void RealRobot::initBatteries(protocol::PacketServer * ps){
	protocol::handlers::BatteryBoardPacketHandler * bbph = new protocol::handlers::BatteryBoardPacketHandler(ps,BATTERY_GROUP,BATTERY_BOARD);
   	RealBattery * rb = new RealBattery(bbph,"b0");
	this->batteries.insert( std::pair<std::string, IBattery *>(rb->getName(),rb) );
	ps->registerHandler(bbph,BATTERY_GROUP,BATTERY_BOARD);
	
	IBattery * b = new RealPCBattery("b1");
	this->batteries.insert( std::pair<std::string, IBattery *>(b->getName(),b) );
}

void RealRobot::initDistanceSensors(protocol::PacketServer * ps){
	protocol::handlers::DistanceSensorBoardPacketHandler * dsbph = new protocol::handlers::DistanceSensorBoardPacketHandler(ps,DISTANCE_SENSOR_GROUP,DISTANCE_SENSOR_BOARD_ONE);
	this->initDistanceSensor(ps, dsbph, "ds0", DISTANCE_SENSOR_ONE_ID);
	this->initDistanceSensor(ps, dsbph, "ds1", DISTANCE_SENSOR_TWO_ID);
	this->initDistanceSensor(ps, dsbph, "ds2", DISTANCE_SENSOR_THREE_ID);
	this->initDistanceSensor(ps, dsbph, "ds3", DISTANCE_SENSOR_FOUR_ID);
	this->initDistanceSensor(ps, dsbph, "ds4", DISTANCE_SENSOR_FIVE_ID);
	dsbph = new protocol::handlers::DistanceSensorBoardPacketHandler(ps,DISTANCE_SENSOR_GROUP,DISTANCE_SENSOR_BOARD_TWO);
	this->initDistanceSensor(ps, dsbph, "ds5", DISTANCE_SENSOR_ONE_ID);
	this->initDistanceSensor(ps, dsbph, "ds6", DISTANCE_SENSOR_TWO_ID);
	this->initDistanceSensor(ps, dsbph, "ds7", DISTANCE_SENSOR_THREE_ID);
	this->initDistanceSensor(ps, dsbph, "ds8", DISTANCE_SENSOR_FOUR_ID);
	this->initDistanceSensor(ps, dsbph, "ds9", DISTANCE_SENSOR_FIVE_ID);
	// IF THERE ARE MORE THAN 10, CREATE ANOTHER BOARDHANDLER WITH SAME GROUP AND ANOTHER BOARDID
	ps->registerHandler(dsbph,DISTANCE_SENSOR_GROUP,DISTANCE_SENSOR_BOARD_ONE);
	ps->registerHandler(dsbph,DISTANCE_SENSOR_GROUP,DISTANCE_SENSOR_BOARD_TWO);
}

void RealRobot::initDistanceSensor(protocol::PacketServer * ps, protocol::handlers::DistanceSensorBoardPacketHandler * dsbph, std::string name, int id){
	RealDistanceSensor * ds = new RealDistanceSensor(dsbph,id,name);
	this->distanceSensors.insert( std::pair<std::string, IDistanceSensor *>(ds->getName(),ds) );
}

void RealRobot::initCameras(){
}

void RealRobot::initTrashBins(protocol::PacketServer * ps){
	protocol::handlers::TrashBinBoardPacketHandler * tbph = new protocol::handlers::TrashBinBoardPacketHandler(ps,TRASHBIN_GROUP,TRASHBIN_BOARD);
   	RealTrashBin * tb = new RealTrashBin(tbph,"trashbin0");
	this->trashBins.insert( std::pair<std::string, ITrashBin *>(tb->getName(),tb) );
	ps->registerHandler(tbph,TRASHBIN_GROUP,TRASHBIN_BOARD);
}

void RealRobot::initServos(protocol::PacketServer * ps){
	protocol::handlers::ServoBoardPacketHandler * sbph = new protocol::handlers::ServoBoardPacketHandler(ps,SERVO_GROUP,SERVO_BOARD);
	this->initServo(ps, sbph, "servo0", SERVO_ONE_ID);
	this->initServo(ps, sbph, "servo1", SERVO_TWO_ID);
	this->initServo(ps, sbph, "servo2", SERVO_THREE_ID);
	ps->registerHandler(sbph,SERVO_GROUP,SERVO_BOARD);
}

void RealRobot::initServo(protocol::PacketServer * ps, protocol::handlers::ServoBoardPacketHandler * sbph, std::string name, int id){
	RealServo * rs = new RealServo(sbph,id,name);
	this->servos.insert( std::pair<std::string, IServo *>(rs->getName(),rs) );
}

std::string RealRobot::getName(){
	return "T-Cleaner";
}

double RealRobot::getTime(){
	return 0.0;
}

int RealRobot::getMode(){
	return 0;
}

bool RealRobot::getSynchronization(){
	return true;
}

double RealRobot::getBasicTimeStep(){
	return 16;
}

ICamera & RealRobot::getCamera(std::string name){
	return *this->cameras[name];
};

IDistanceSensor & RealRobot::getDistanceSensor(std::string name){
	return *this->distanceSensors[name];
}

IServo & RealRobot::getServo(std::string name){
	return *this->servos[name];
}

IDevice & RealRobot::getDevice(std::string name){
	IDevice * device = new RealDevice(&name);
    return *device;
}

IDifferentialWheels & RealRobot::getDifferentialWheels(std::string name){
	return *this->wheels[name];
}

IBattery & RealRobot::getBattery(std::string name){
	return *this->batteries[name];
}

ITrashBin & RealRobot::getTrashBin(std::string name){
	return *this->trashBins[name];
}

void RealRobot::step(int ms){
	return;
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
