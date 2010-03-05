#include "GarbageCleaner.h"
#include <behaviours/AvoidObstacle.h>
#include <behaviours/CollectGarbage.h>
#include <behaviours/FindLine.h>
#include <behaviours/FocusGarbage.h>
#include <behaviours/GoToBase.h>
#include <behaviours/GoToDisposal.h>
#include <behaviours/Recharge.h>
#include <behaviours/UnloadGarbage.h>
#include <behaviours/Wander.h>
#include <robotapi/IDistanceSensor.h>
#include <robotapi/IServo.h>
#include <robotapi/IDifferentialWheels.h>
#include <vector>
#include <sstream>
#include <iostream>

std::vector<robotapi::IDistanceSensor*> dss;
std::vector<robotapi::IDistanceSensor*> fss;
robotapi::IServo* servoFront;
robotapi::IServo* servoBottom;
robotapi::IServo* servoRear;
robotapi::IDifferentialWheels* wheels;
robotapi::ICamera* camera;

GarbageCleaner::GarbageCleaner(robotapi::IRobot & robot) : myIRobot(robot)
// don't delete the following line as it's needed to preserve source code of this autogenerated element
// section -64--88-1-100--1230920c:125cb7ecd6f:-8000:0000000000000DB6 begin
{
	initializeBehaviours();
	initializeSensors();
}
// section -64--88-1-100--1230920c:125cb7ecd6f:-8000:0000000000000DB6 end
// don't delete the previous line as it's needed to preserve source code of this autogenerated element

void GarbageCleaner::initializeSensors(){
	for( int i=0 ; i < DISTANCE_SENSORS ; i++ ){
		std::stringstream sstr;
		sstr << "ps" << i;
		robotapi::IDistanceSensor * ids = &myIRobot.getDistanceSensor(sstr.str());
		ids->enable(TIME_STEP);
		dss.push_back(ids);
	}
/*	for( int i=0 ; i < FLOOR_SENSORS ; i++ ){
		std::stringstream sstr;
		sstr << "fs" << i;
		robotapi::IDistanceSensor * ifs = &myIRobot.getDistanceSensor(sstr.str());
		ifs->enable(TIME_STEP);
		fss.push_back(ifs);
	}

	servoFront = &myIRobot.getServo("s_f");
	servoBottom = &myIRobot.getServo("s_b");
	servoRear = &myIRobot.getServo("s_r");
	wheels = &myIRobot.getDifferentialWheels("df");
	wheels->enableEncoders(TIME_STEP);

	camera = &myIRobot.getCamera("camera");
	camera->enable(TIME_STEP);
*/
	
}

void GarbageCleaner::initializeBehaviours(){
    behaviours::AbstractBehaviour * w = new behaviours::Wander();
    myAbstractBehaviours.push_back(w);
    w = new behaviours::Wander();
    myAbstractBehaviours.push_back(w);
    w = new behaviours::Wander();
    myAbstractBehaviours.push_back(w);
    w = new behaviours::Wander();
    myAbstractBehaviours.push_back(w);
}

void GarbageCleaner::cleanGarbage()
// don't delete the following line as it's needed to preserve source code of this autogenerated element
// section -64--88-1-100--1230920c:125cb7ecd6f:-8000:0000000000000DBA begin
{
	std::list<behaviours::AbstractBehaviour*>::iterator it;

	std::cout << "mylist contains:";
	for ( it=myAbstractBehaviours.begin() ; it != myAbstractBehaviours.end(); it++ )
		std::cout << "\n" << (*it)->toString();

}
// section -64--88-1-100--1230920c:125cb7ecd6f:-8000:0000000000000DBA end
// don't delete the previous line as it's needed to preserve source code of this autogenerated element