#ifndef GarbageCleaner_h
#define GarbageCleaner_h

#include <list>

#include <behaviours/AbstractBehaviour.h>
#include <robotapi/IRobot.h>
#include <WorldInfo.h>

#define FOCUS_GOTO_COLLECT_DISPOSE_GARBAGE 1
#define GO_TO_RECHARGE 1

#define DISTANCE_SENSORS 8
#define FLOOR_SENSORS 3
#define TIME_STEP 32

#define MAX_BEHAVIOURS 32

class GarbageCleaner {

 public:

    GarbageCleaner(WorldInfo * wi, robotapi::IRobot &robot);

    void cleanGarbage();

    robotapi::IRobot &myIRobot;

	void stepWasDone();
    /**
     * @element-type AbstractBehaviour
     */
    std::list< behaviours::AbstractBehaviour* > myAbstractBehaviours;
    
	void printStats();
 private:
	void initializeBehaviours(WorldInfo * wi);
	void initializeSensors();
	int stats [MAX_BEHAVIOURS];

};

#endif // GarbageCleaner_h
