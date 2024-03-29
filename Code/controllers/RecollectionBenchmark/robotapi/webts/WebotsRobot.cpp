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
#include <webots/GPS.hpp>
#include <utils/Rectangle2D.h>
#include <math.h>

namespace robotapi {
namespace webts {

		webots::Robot * robot;
		webots::GPS * gps;
		double lastPosX, lastPosZ;
		double lastGPSX, lastGPSZ;
		double distanceCoveredOdometry, distanceCoveredGPS;

    WebotsRobot::WebotsRobot( WorldInfo * wi, webots::DifferentialWheels & dw){
		robot = &dw;
		this->wi = wi;
		this->df = new WebotsDifferentialWheels(this->wi, (webots::DifferentialWheels&)*robot);
		IWbDeviceTag * wdt = new WebotsDeviceTag(3);
		std::string s = "HOLA";
		this->pcBattery = new WebotsPCBattery(s,*wdt);
		this->robotBattery = new WebotsBattery(*robot,s,*wdt);
		this->ag = this->wi->getArenaGrid();
		gps = robot->getGPS("gps");
		if ( gps != NULL ){
			gps->enable(32);
			lastGPSX = this->wi->getInitialPosition()->getX();
			lastGPSZ = this->wi->getInitialPosition()->getY();
		}
		lastPosX = this->wi->getInitialPosition()->getX();
		lastPosZ = this->wi->getInitialPosition()->getY();
		distanceCoveredOdometry = 0;
		distanceCoveredGPS = 0;
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
//		printf("STEPPING");

		#ifdef REFRESH_SEEN_SLOTS
		if ( currentSlot != NULL ){
			//printf("Current Slot: %g - %g --> Timestamp: %ld\n",currentSlot->getX(),currentSlot->getZ(),currentSlot->getTimeStamp());
			std::list<utils::ArenaGridSlot *> seenSlots = this->getSlotsSeen(df->getPosition(), df->getOrientation(), currentSlot);
			this->saveChanges(seenSlots);
		}
		#endif

		#ifdef CONTROLLER_DEBUG
		printf("Current Position : %g %g %g\n",df->getPosition()->getX(),df->getPosition()->getY(),df->getOrientation());
		#endif
		
		#ifdef OUTPUT_ODOMETRY_ERROR_GPS
		if ( gps != NULL ){
			const double * values = gps->getValues();
			double difX = values[0] - lastGPSX;
			double difZ = values[2] - lastGPSZ;
			double angle = atan2(-difX,-difZ);

			distanceCoveredOdometry += sqrt( (df->getPosition()->getX() - lastPosX ) * (df->getPosition()->getX() - lastPosX ) + (df->getPosition()->getY() - lastPosZ ) * (df->getPosition()->getY() - lastPosZ ) );
			distanceCoveredGPS += sqrt( difX * difX + difZ * difZ );

			printf("GPS Position : %g %g %g\n",values[0],values[2],angle);//(new utils::MyAngle(angle))->getNormalizedValue());
			lastGPSX = values[0];
			lastGPSZ = values[2];
			lastPosX = df->getPosition()->getX();
			lastPosZ = df->getPosition()->getY();

			FILE * pFile;
			pFile = fopen ("odometryError.txt","a+");
			if (pFile!=NULL){
				fprintf(pFile,"%g %g %g %g %g %g %g %g\n",df->getPosition()->getX(),
													values[0],
													df->getPosition()->getY(),
													values[2],
													df->getOrientation(),
													angle,
													distanceCoveredOdometry,
													distanceCoveredGPS);
			}
			fclose(pFile);
		}
		#endif
		
		#ifdef CONTROLLER_DEBUG
		printf("Slots Left: %d - Really Left: %d - Visited : %d\n",this->ag->getSlotsLeft(),this->ag->getSlotsLeftWOExcluded(),this->ag->getSlotsVisited());
		printf("Robot Battery : %g - PC Battery : %g\n",robotBattery->getValue(),pcBattery->getValue());
		#endif

		// Refresh stats
		if ( gc != NULL )
			gc->stepWasDone();
		return ;
	}
    
	void WebotsRobot::saveChanges(std::list<utils::ArenaGridSlot *> ags){
		std::list<utils::ArenaGridSlot *>::iterator it;
		time_t ts = time(NULL);

		#ifdef SAVE_CHANGED
		FILE * pFile;
		pFile = fopen ("changes.tmp","w");
		if (pFile!=NULL){
		#endif
			time_t ts0 = this->ag->getInitialTimeStamp();
			for ( it=ags.begin() ; it != ags.end() ; it++ ){
				if ( (*it)->setTimeStamp(ts) <= ts0 )
					this->ag->setSlotVisited(*it);

				#ifdef SAVE_CHANGED
				fprintf(pFile,"(%d,%d)",(*it)->getI(),(*it)->getJ());
				#endif
//				printf("(%d,%d)\n",(*it)->getI(),(*it)->getJ());
			}
		#ifdef SAVE_CHANGED
		}
		fclose (pFile);
		#endif
		return;
	}

	std::list<utils::ArenaGridSlot *> WebotsRobot::getSlotsSeen(utils::MyPoint * position, double angle, utils::ArenaGridSlot * currentSlot){
		std::list<utils::ArenaGridSlot *> out;
		out.push_back(currentSlot);
		utils::Rectangle2D * rect = new utils::Rectangle2D( position,
										this->wi->getMinimumDistance(),
										this->wi->getMaximumDistance(),
										angle, this->wi->getCameraFOVH());

		if ( rect == NULL )
			return out;

		//printf("Pos act: (%g,%g)",position->getX(),position->getY());
		//printf("Angle: %g",angle);
		utils::MyPoint * minP = rect->getMinPoint();
		utils::MyPoint * maxP = rect->getMaxPoint();

		int minI = this->ag->getIForPoint(minP);
		int maxI = this->ag->getIForPoint(maxP);
		int minJ = this->ag->getJForPoint(minP);
		int maxJ = this->ag->getJForPoint(maxP);
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
		delete rect;

		return out;
	}

	void WebotsRobot::setGC(IGarbageCleaner * gc){
		this->gc = gc;
	}

	void WebotsRobot::shutdown(){
};
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
