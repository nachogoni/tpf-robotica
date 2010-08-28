#include <cstdlib>
#include <iostream>

#include <robotapi/webts/WebotsRobot.h>
#include <robotapi/webts/WebotsDifferentialWheels.h>
#include <webots/DifferentialWheels.hpp>

#include <GarbageCleaner.h>
#include <WorldInfo.h>
#include <behaviours/BehavioursParameters.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>

// define this to save to disk slots visited
// SAVE_CHANGED

int main(int argc, char *argv[])
{
	FILE * parametersFile = fopen("webotsParameters.txt","r+");
  behaviours::BehavioursParameters::Init(parametersFile);

	WorldInfo * wi = new WorldInfo();

	webots::DifferentialWheels * wdw = new webots::DifferentialWheels();

    robotapi::webts::WebotsRobot * robot = new robotapi::webts::WebotsRobot(wi, *wdw);

//    robotapi::real::RealRobot * robot = new robotapi::real::RealRobot(wi);

	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(wi, *robot);



	gc->cleanGarbage();
/*
  printf("HOLA\n");
  system("PAUSE");
  */
    return EXIT_SUCCESS;
}
