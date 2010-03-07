#include <cstdlib>
#include <iostream>

#include <robotapi/IWbDeviceTag.h>
#include <robotapi/IDevice.h>

#include <robotapi/Webts/WebotsDeviceTag.h>
#include <robotapi/Webts/WebotsDevice.h>
#include <robotapi/Webts/WebotsRobot.h>
#include <robotapi/Webts/WebotsDifferentialWheels.h>
#include <webots/DifferentialWheels.hpp>
#include <GarbageCleaner.h>
#include <cv.h>
#include <highgui.h>

using namespace std;

int main(int argc, char *argv[])
{

	webots::DifferentialWheels * wdw = new webots::DifferentialWheels();
    robotapi::webts::WebotsRobot * robot = new robotapi::webts::WebotsRobot(*wdw);

	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(*robot);
	gc->cleanGarbage();
  
    return EXIT_SUCCESS;
}
