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
	FILE * parametersFile = fopen("realParameters.txt","r+");
	behaviours::BehavioursParameters::Init(parametersFile);

	WorldInfo * wi = new WorldInfo();

    robotapi::real::RealRobot * robot = new robotapi::real::RealRobot(wi);

	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(wi, *robot);

	gc->cleanGarbage();
    return EXIT_SUCCESS;
}
