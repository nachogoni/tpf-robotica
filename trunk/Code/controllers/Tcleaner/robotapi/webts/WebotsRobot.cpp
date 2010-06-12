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
#include <utils/Rectangle2D.h>
#include <GarbageCleaner.h>

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
		printf("STEPPING");
		if ( currentSlot != NULL ){
			std::list<utils::ArenaGridSlot *> seenSlots = this->getSlotsSeen(df->getPosition(), df->getOrientation(), currentSlot);
			printf("Current Slot: %g - %g --> Timestamp: %ld\n",currentSlot->getX(),currentSlot->getZ(),currentSlot->getTimeStamp());
			this->saveChanges(seenSlots);
		}
/*
		printf("Current Position : %g %g %g\n",df->getPosition()->getX(),df->getPosition()->getY(),df->getOrientation());
		printf("Robot Battery : %g - PC Battery : %g\n",robotBattery->getValue(),pcBattery->getValue());
		printf("Current Touch Sensor value : %d\n",this->tb->getValue());
		*/

		// Refresh stats
		if ( gc != NULL )
			gc->stepWasDone();

		return ;
	}
    
	void WebotsRobot::saveChanges(std::list<utils::ArenaGridSlot *> ags){
		std::list<utils::ArenaGridSlot *>::iterator it;
		time_t ts = time(NULL);
		FILE * pFile;
		pFile = fopen ("changes.tmp","w");
		if (pFile!=NULL){
			time_t ts0 = this->ag->getInitialTimeStamp();
			for ( it=ags.begin() ; it != ags.end() ; it++ ){
				if ( (*it)->setTimeStamp(ts) <= ts0 )
					this->ag->setSlotVisited(*it);

				fprintf(pFile,"(%d,%d)",(*it)->getI(),(*it)->getJ());
//				printf("(%d,%d)\n",(*it)->getI(),(*it)->getJ());
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

		//printf("Pos act: (%g,%g)",position->getX(),position->getY());
		//printf("Angle: %g",angle);
		utils::MyPoint * minP = rect->getMinPoint();
		utils::MyPoint * maxP = rect->getMaxPoint();
		//printf("Punto Min: (%g : %g)\nPunto Max: (%g : %g)\n",minP->getX(),minP->getY(),maxP->getX(),maxP->getY());

		utils::ArenaGridSlot * minSlot = this->ag->getSlotAt(minP);
		utils::ArenaGridSlot * maxSlot = this->ag->getSlotAt(maxP);
		int minI = minSlot->getI();
		int maxI = maxSlot->getI();
		int minJ = minSlot->getJ();
		int maxJ = maxSlot->getJ();
		//printf("minI : %d, maxI : %d, minJ : %d, maxJ : %d\n",minI,maxI,minJ,maxJ);
		for( int i = minI ; i < maxI ; i++ ){
			for( int j = minJ ; j < maxJ ; j++ ){
				utils::ArenaGridSlot * current = this->ag->getSlotAt(i,j);
				if ( current != NULL ){
					utils::MyPoint * cp = new utils::MyPoint(current->getX(),current->getZ());
					if ( rect->containsPoint(cp) ){
						out.push_back(current);
					}
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

	void WebotsRobot::setGC(GarbageCleaner * gc){
		this->gc = gc;
	}
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
