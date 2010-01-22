#include "WebotsDifferentialWheels.h"

namespace robotapi {
namespace webts {

	webots::DifferentialWheels * dw;

    std::string dwname;
   	IWbDeviceTag dwtag;

//    WebotsDifferentialWheels::WebotsDifferentialWheels(webots::DifferentialWheels & wdw) : WebotsDevice ( wdw ){
    WebotsDifferentialWheels::WebotsDifferentialWheels(webots::DifferentialWheels & wdw){
		dw = &wdw;
		dwname = "dw1";
		dwtag = *new WebotsDeviceTag(3);
	}

    void WebotsDifferentialWheels::setSpeed(double left, double right){
		dw->setSpeed(left,right);
	}

    void WebotsDifferentialWheels::enableEncoders(int ms){
		dw->enableEncoders(ms);
	}

    void WebotsDifferentialWheels::disableEncoders(){
		dw->disableEncoders();
	}

    double WebotsDifferentialWheels::getLeftEncoder(){
		return 1.2;
	}

    double WebotsDifferentialWheels::getRightEncoder(){
		return 1.2;
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

    double WebotsDifferentialWheels::getMotorConsumption(){
		return 1.2;
	}

    double WebotsDifferentialWheels::getAverageConsumption(){
		return 1.2;
	}

    std::string WebotsDifferentialWheels::getName(){
		return dwname;
	}

    IWbDeviceTag & WebotsDifferentialWheels::getTag(){
		return dwtag;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
