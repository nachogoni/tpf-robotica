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
		utils::ArenaGridSlot * currentSlot = this->ag->getSlotAt(df->getPosition());
		if ( currentSlot != NULL ){
			std::list<utils::ArenaGridSlot *> seenSlots = this->getSlotsSeen(currentSlot);
			printf("Current Slot: %g - %g --> Timestamp: %ld\n",currentSlot->getX(),currentSlot->getZ(),currentSlot->getTimeStamp());
			this->saveChanges(seenSlots);
		}
/*
		printf("Current Position : %g %g %g\n",df->getPosition()->getX(),df->getPosition()->getY(),df->getOrientation());
		printf("Robot Battery : %g - PC Battery : %g\n",robotBattery->getValue(),pcBattery->getValue());
		printf("Current Touch Sensor value : %d\n",this->tb->getValue());
		*/
		return ;
	}
    
	void WebotsRobot::saveChanges(std::list<utils::ArenaGridSlot *> ags){
		std::list<utils::ArenaGridSlot *>::iterator it;
		time_t ts = time(NULL);
		FILE * pFile;
		pFile = fopen ("changes.tmp","w");
		if (pFile!=NULL){
			for ( it=ags.begin() ; it != ags.end() ; it++ ){
				(*it)->setTimeStamp(ts);
				fprintf(pFile,"(%d,%d)",this->ag->getSlotXIdx(*it),this->ag->getSlotZIdx(*it));
				printf("(%d,%d)",this->ag->getSlotXIdx(*it),this->ag->getSlotZIdx(*it));
			}
		}
		fclose (pFile);

		return;
	}

	std::list<utils::ArenaGridSlot *> WebotsRobot::getSlotsSeen(utils::MyPoint * position, double angle, utils::ArenaGridSlot * currentSlot){
		std::list<utils::ArenaGridSlot *> out;
		out.push_back(currentSlot);
		utils::Rectangle2D * rect = new utils::Rectangle2D( position,
										this->wi->getMinimumDistance(),
										this->wi->getMaximumDistance(),
										angle, this->wi->getCameraFOVH());

		
		utils::MyPoint * minP = rect->getMinPoint();
		utils::MyPoint * maxP = rect->getMaxPoint();
		utils::ArenaGridSlot * minSlot = this->ag->getSlotAt(minP);
		utils::ArenaGridSlot * maxSlot = this->ag->getSlotAt(maxP);
		int minI = this->ag->getSlotXIdx(minSlot);
		int maxI = this->ag->getSlotXIdx(maxSlot);
		int minJ = this->ag->getSlotZIdx(minSlot);
		int maxJ = this->ag->getSlotZIdx(maxSlot);

		for( int i = minI ; i < maxI ; i++ ){
			for( int j = minJ ; j < maxJ ; j++ ){
				utils::ArenaGridSlot * current = this->ag->getSlotAt(i,j);
				if ( current != NULL ){
					utils::MyPoint * cp = new utils::MyPoint(current->getX(),current->getZ());
					if ( rect->containsPoint(cp) )
						out.push_back(cp);
					else
						delete cp;
				}
			}
		}
		if ( minP != NULL )
			delete minP;
		if ( maxP != NULL )
			delete maxP;
		return out;
	}


} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
