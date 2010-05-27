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
#include <utils/ArenaGridSlot.h>

namespace robotapi {
namespace webts {

	webots::Robot * robot;

    WebotsRobot::WebotsRobot( WorldInfo * wi, webots::DifferentialWheels & dw){
		robot = &dw;
		this->wi = wi;
		this->df = new WebotsDifferentialWheels(this->wi, (webots::DifferentialWheels&)*robot);
		IWbDeviceTag * wdt = new WebotsDeviceTag(3);
		std::string s = "HOLA";
		this->pcBattery = new WebotsPCBattery(s,*wdt);
		this->robotBattery = new WebotsBattery(*robot,s,*wdt);
		this->ag = this->wi->getArenaGrid();
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
        return *this->df;
    }

    IBattery & WebotsRobot::getBattery(std::string name){
		if ( name.compare("b1") == 0 )
			return *this->pcBattery;

		return *this->robotBattery;
    }

	ITrashBin & WebotsRobot::getTrashBin(std::string name){
		if ( this->tb == NULL )
	        this->tb = new WebotsTrashBin(*robot->getTouchSensor(name));
        return * (this->tb);
	}

	void WebotsRobot::step(int ms){
		robot->step(ms);
		df->computeOdometry();
		utils::ArenaGridSlot * ags = this->ag->getSlotAt(df->getPosition());
		if ( ags != NULL ){
			printf("Current Slot: %g - %g --> Timestamp: %ld\n",ags->getX(),ags->getZ(),ags->getTimeStamp());
			ags->setTimeStamp();
			this->saveChanges(ags);
		}
/*
		printf("Current Position : %g %g %g\n",df->getPosition()->getX(),df->getPosition()->getY(),df->getOrientation());
		printf("Robot Battery : %g - PC Battery : %g\n",robotBattery->getValue(),pcBattery->getValue());
		printf("Current Touch Sensor value : %d\n",this->tb->getValue());
		*/
		return ;
	}
    
	void WebotsRobot::saveChanges(utils::ArenaGridSlot * ags){
		//TODO open file, save data and close file
		return;
	}
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
