#include <cstdlib>
#include <iostream>

#include <robotapi/real/RealRobot.h>
#include <GarbageCleaner.h>
#include <WorldInfo.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>

// define this to save to disk slots visited
// SAVE_CHANGED

using namespace std;

int main(int argc, char *argv[])
{

	WorldInfo * wi = new WorldInfo();

//	webots::DifferentialWheels * wdw = new webots::DifferentialWheels();

//	robotapi::webts::WebotsRobot * robot = new robotapi::webts::WebotsRobot(wi, *wdw);

    robotapi::real::RealRobot * robot = new robotapi::real::RealRobot(wi);

	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(wi, *robot);

	gc->cleanGarbage();
/*
  printf("HOLA\n");
  system("PAUSE");
  */
    return EXIT_SUCCESS;
}
