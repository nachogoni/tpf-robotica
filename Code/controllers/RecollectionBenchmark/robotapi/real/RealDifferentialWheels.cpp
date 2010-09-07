#include "RealDifferentialWheels.h"

namespace robotapi {
namespace real {

RealDifferentialWheels::RealDifferentialWheels(WorldInfo * wi, protocol::handlers::DCMotorBoardPacketHandler * dcmbphl,
												protocol::handlers::DCMotorBoardPacketHandler * dcmbphr,
												std::string name ) : DifferentialWheelsWOdometry( wi )
												 , RealDevice( &name ) {
		this->leftBoard = dcmbphl;
		this->rightBoard = dcmbphr;
}

RealDifferentialWheels::~RealDifferentialWheels(){
}

void RealDifferentialWheels::setSpeed(double left, double right){
	this->leftBoard->setSpeed(left);
	this->rightBoard->setSpeed(right);
}

void RealDifferentialWheels::enableEncoders(int ms){
	return;
}

void RealDifferentialWheels::disableEncoders(){
	this->leftBoard->setEncoder(0);
	this->rightBoard->setEncoder(0);
}

double RealDifferentialWheels::getLeftEncoder(){
	return this->leftBoard->getEncoder();
}

double RealDifferentialWheels::getRightEncoder(){
	return this->rightBoard->getEncoder();
}

double RealDifferentialWheels::getLeftSpeed(){
	return this->leftBoard->getSpeed();
}

double RealDifferentialWheels::getRightSpeed(){
	return this->rightBoard->getSpeed();
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
	return this->isAlarmPresent(true) || this->isAlarmPresent(false);
}

bool RealDifferentialWheels::isAlarmPresent(bool left){
	if ( left )
		return this->leftBoard->isAlarmPresent();

	return this->rightBoard->isAlarmPresent();
}

bool RealDifferentialWheels::motorIsOff(){
	return this->motorIsOff(true) || this->motorIsOff(false);
}

bool RealDifferentialWheels::motorIsOff(bool left){
	if ( left )
		return this->leftBoard->motorIsOff();

	return this->rightBoard->motorIsOff();
}

void RealDifferentialWheels::refresh(){
	
	this->leftBoard->getEncoder(true);
	this->rightBoard->getEncoder(true);
	
}

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */
