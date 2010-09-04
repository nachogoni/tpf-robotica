#include <cstdlib>
#include <iostream>

#include <robotapi/real/RealRobot.h>
#include <behaviours/BehavioursParameters.h>
#include <utilsVision/GarbageRecognition.h>
#include <WorldInfo.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#define MAX_SPEED 100
#define TURN_SPD 60.0
#define METER_SPD 200.0

using namespace std;

robotapi::real::RealRobot * dwrobot;
robotapi::IDifferentialWheels * wdw;
robotapi::ICamera * realcamera;
utils::IGarbageRecognition * igr;
WorldInfo * wi;

void turnLeft(double qspeed);
void turnRight(double qspeed);
void doOneMeter(double qspeed);
void doClockWise(double speed, int cant);
void doCounterClockWise(double speed, int cant);

int main(int argc, char *argv[])
{
	FILE * parametersFile = fopen("realParameters.txt","r+");
	behaviours::BehavioursParameters::Init(parametersFile);
	wi = new WorldInfo();

	dwrobot = new robotapi::real::RealRobot(wi);

	wdw = &(dwrobot->getDifferentialWheels("dw0"));
	wdw->enableEncoders(32);


	realcamera = &(dwrobot->getCamera("camera0"));
	realcamera->enable(32);
	igr = new utils::GarbageRecognition(wi);
	igr->setCamera(*realcamera);

	while(1){
		//do whatever you want with gr, dwrobot and wdw...
		wdw->setSpeed(0,0);
		dwrobot->step(32);
	}

	return EXIT_SUCCESS;
}

