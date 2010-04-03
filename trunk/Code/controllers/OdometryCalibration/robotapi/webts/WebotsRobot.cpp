#include "WebotsRobot.h"

#include <robotapi/webts/WebotsCamera.h>
#include <robotapi/webts/WebotsDistanceSensor.h>
#include <robotapi/webts/WebotsServo.h>
#include <robotapi/webts/WebotsBattery.h>
#include <robotapi/webts/WebotsPCBattery.h>
#include <robotapi/webts/WebotsDifferentialWheels.h>
#include <robotapi/webts/WebotsTrashBin.h>
#include <webots/Camera.hpp>
#include <webots/Robot.hpp>

namespace robotapi {
namespace webts {

	webots::Robot * robot;

    WebotsRobot::WebotsRobot( WorldInfo * wi, webots::DifferentialWheels & dw){
		robot = &dw;
		this->wi = wi;
	}

    std::string WebotsRobot::getName(){
        return robot->getName();
    }
    
    double WebotsRobot::getTime(){
        return robot->getTime();
    }

    int WebotsRobot::getMode(){
        return robot->getMode();
    }

    bool WebotsRobot::getSynchronization(){
        return robot->getSynchronization();
    }
    
    double WebotsRobot::getBasicTimeStep(){
        return robot->getBasicTimeStep();
    }
    
    ICamera & WebotsRobot::getCamera(std::string name){
        WebotsCamera * cam = new WebotsCamera(*robot->getCamera(name));
        return * cam;
    }

    IDistanceSensor & WebotsRobot::getDistanceSensor(std::string name){
        WebotsDistanceSensor * ds = new WebotsDistanceSensor(*robot->getDistanceSensor(name));
        return * ds;
    }

    IServo & WebotsRobot::getServo(std::string name){
        WebotsServo * s = new WebotsServo(*robot->getServo(name));
        return * s;
    }

    IDevice & WebotsRobot::getDevice(std::string name){
        IWbDeviceTag * wdt = new WebotsDeviceTag(3);
        WebotsDevice * d = new WebotsDevice(name, *wdt);
        return * d;
    }

    IDifferentialWheels & WebotsRobot::getDifferentialWheels(std::string name){
        WebotsDifferentialWheels * df = new WebotsDifferentialWheels(this->wi->getDistanceBetweenWheels(),
										this->wi->getLeftWheelRadius(), this->wi->getEncoderResolution(),
										this->wi->getInitialPosition()->getX(), this->wi->getInitialPosition()->getY(),
										this->wi->getInitialOrientation()->getNormalizedValue(),
										(webots::DifferentialWheels&)*robot);
        // TODO Save 'em in order to reuse it and don't create another instance
        return * df;
    }

    IBattery & WebotsRobot::getBattery(std::string name){
		IWbDeviceTag * wdt = new WebotsDeviceTag(3);
		IBattery * b;
		if ( name.compare("b1") == 0 )
	        b = new WebotsPCBattery(name,*wdt);
		else
	        b = new WebotsBattery(*robot,name,*wdt);
        return * b;
    }

	ITrashBin & WebotsRobot::getTrashBin(std::string name){
        WebotsTrashBin * tb = new WebotsTrashBin(*robot->getTouchSensor(name));
        return * tb;
	}

	void WebotsRobot::step(int ms){
		robot->step(ms);
		return ;
	}
    
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
