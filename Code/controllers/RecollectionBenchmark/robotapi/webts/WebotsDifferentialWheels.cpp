#include "WebotsDifferentialWheels.h"

namespace robotapi {
namespace webts {

    std::string dwname;
   	IWbDeviceTag dwtag;

//    WebotsDifferentialWheels::WebotsDifferentialWheels(webots::DifferentialWheels & wdw) : WebotsDevice ( wdw ){
    WebotsDifferentialWheels::WebotsDifferentialWheels(WorldInfo * wi, webots::DifferentialWheels & wdw)
														: DifferentialWheelsWOdometry(wi){
		dw = &wdw;
		dwname = "dw1";
		dwtag = *new WebotsDeviceTag(3);
	}

    void WebotsDifferentialWheels::setSpeed(double left, double right){
		dw->setSpeed(left,right);
		this->leftSpeed = left;
		this->rightSpeed = right;
	}

	double WebotsDifferentialWheels::getLeftSpeed(){
		return this->leftSpeed;
	}

	double WebotsDifferentialWheels::getRightSpeed(){
		return this->rightSpeed;
	}

    void WebotsDifferentialWheels::enableEncoders(int ms){
		dw->enableEncoders(ms);
	}

    void WebotsDifferentialWheels::disableEncoders(){
		dw->disableEncoders();
	}

    double WebotsDifferentialWheels::getLeftEncoder(){
		return dw->getLeftEncoder();
	}

    double WebotsDifferentialWheels::getRightEncoder(){
		return dw->getRightEncoder();
	}

    void WebotsDifferentialWheels::moveWheels(double left, double right){

	}

    void WebotsDifferentialWheels::moveLeftWheel(double left){

	}

    void WebotsDifferentialWheels::moveRightWheel(double right){

	}

    void WebotsDifferentialWheels::moveLeftWheel(double left, double speed){

	}

    void WebotsDifferentialWheels::moveRightWheel(double right, double speed){

	}

    double WebotsDifferentialWheels::getLeftMotorConsumption(){
		return 1.2;
	}

    double WebotsDifferentialWheels::getRightMotorConsumption(){
		return 1.2;
	}

	bool WebotsDifferentialWheels::isAlarmPresent(){
		return false;
	}

	bool WebotsDifferentialWheels::motorIsOff(){
		return false;
	}
	
	bool WebotsDifferentialWheels::isAlarmPresent(bool left){
		return false;
	}

	bool WebotsDifferentialWheels::motorIsOff(bool left){
		return false;
	}

    std::string WebotsDifferentialWheels::getName(){
		return dwname;
	}

    IWbDeviceTag & WebotsDifferentialWheels::getTag(){
		return dwtag;
	}

	void WebotsDifferentialWheels::refresh(){
		return ;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
