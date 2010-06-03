#include <cstdlib>
#include <iostream>

#include <robotapi/IWbDeviceTag.h>
#include <robotapi/IDevice.h>

#include <robotapi/real/RealRobot.h>
#include <GarbageCleaner.h>
#include <WorldInfo.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>

using namespace std;

int main(int argc, char *argv[])
{

	WorldInfo * wi = new WorldInfo();

	robotapi::real::RealRobot * robot = new robotapi::real::RealRobot(wi);

	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(wi, *robot);

	gc->cleanGarbage();
/*
  printf("HOLA\n");
  system("PAUSE");
  */
    return EXIT_SUCCESS;
}
