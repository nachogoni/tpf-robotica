#include "RealServo.h"

namespace robotapi {
namespace real {

RealServo::RealServo(protocol::handlers::ServoBoardPacketHandler * sbph, int sensorNumber){
	this->boardHandler = sbph;
	this->id = sensorNumber;
}

void RealServo::setAcceleration(double force){
	return;
}

void RealServo::setVelocity(double vel){
	this->boardHandler->setSpeed(id, vel);
}

void RealServo::enablePosition(int ms){
	return;
}

void RealServo::disablePosition(){
	return;
}

void RealServo::setForce(double force){
	this->boardHandler->setForce(id,force);
}

void RealServo::setMotorForce(double motor_force){
	return;
}

void RealServo::enableMotorForceFeedback(int ms){
	return;
}

void RealServo::disableMotorForceFeedback(){
	return;
}

double RealServo::getMotorForceFeedback(){
	return 0.0;
}

double RealServo::getPosition(){
	return this->boardHandler->getPosition(id);
}

void RealServo::setPosition(double position){
	this->boardHandler->setPosition(id,position);
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
