#include "RealDifferentialWheels.h"

namespace robotapi {
namespace real {

RealDifferentialWheels::RealDifferentialWheels(protocol::handlers::DCMotorBoardPacketHandler * dcmbphl,
												protocol::handlers::DCMotorBoardPacketHandler * dcmbphr){
		this->leftBoard = dcmbphl;
		this->rightBoard = dcmbphr;
}

void RealDifferentialWheels::setSpeed(double left, double right){
	this->leftBoard->setSpeed(left);
	this->rightBoard->setSpeed(right);
}

void RealDifferentialWheels::enableEncoders(int ms){
	return;
}

void RealDifferentialWheels::disableEncoders(){
	return;
}

double RealDifferentialWheels::getLeftEncoder(){
	return this->leftBoard->getEncoder();
}

double RealDifferentialWheels::getRightEncoder(){
	return this->rightBoard->getEncoder();
}

void RealDifferentialWheels::moveWheels(double left, double right){
	this->moveLeftWheel(left);
	this->moveRightWheel(right);
}

void RealDifferentialWheels::moveLeftWheel(double left){
	this->leftBoard->moveWheel(left);
}

void RealDifferentialWheels::moveRightWheel(double right){
	this->rightBoard->moveWheel(right);
}

void RealDifferentialWheels::moveLeftWheel(double left, double speed){
	this->leftBoard->moveWheel(left,speed);
}

void RealDifferentialWheels::moveRightWheel(double right, double speed){
	this->rightBoard->moveWheel(right,speed);
}

double RealDifferentialWheels::getLeftMotorConsumption(){
	return this->leftBoard->getMotorConsumption();
}

double RealDifferentialWheels::getRightMotorConsumption(){
	return this->rightBoard->getMotorConsumption();
}

bool RealDifferentialWheels::isAlarmPresent(){
	// TODO use DCMotorBoardPacketHandler Method
	return false;
}

bool RealDifferentialWheels::isMotorOff(){
	// TODO use DCMotorBoardPacketHandler Method
	return false;
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
