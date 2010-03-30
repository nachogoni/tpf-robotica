#include <cstdlib>
#include <iostream>

#include <robotapi/IWbDeviceTag.h>
#include <robotapi/IDevice.h>

#include <robotapi/webts/WebotsRobot.h>
#include <robotapi/webts/WebotsDifferentialWheels.h>
#include <webots/DifferentialWheels.hpp>
#include <robotapi/real/RealRobot.h>
#include <GarbageCleaner.h>
#include <WorldInfo.h>
#include <cv.h>
#include <highgui.h>

using namespace std;

int main(int argc, char *argv[])
{

	WorldInfo * wi = new WorldInfo();

	webots::DifferentialWheels * wdw = new webots::DifferentialWheels();
    robotapi::webts::WebotsRobot * robot = new robotapi::webts::WebotsRobot(wi, *wdw);

//    robotapi::real::RealRobot * robot = new robotapi::real::RealRobot(wi);
	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(wi, *robot);
	gc->cleanGarbage();
  
    return EXIT_SUCCESS;
}
