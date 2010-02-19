#ifndef GarbageCleaner_h
#define GarbageCleaner_h

#include <list>

#include "behaviours/AbstractBehaviour.h"
#include "robotapi/IRobot.h"

#define DISTANCE_SENSORS 8
#define FLOOR_SENSORS 4
#define TIME_STEP 32

class GarbageCleaner {

 public:

    GarbageCleaner(robotapi::IRobot &robot);

    void cleanGarbage();

    robotapi::IRobot &myIRobot;

    /**
     * @element-type AbstractBehaviour
     */
    std::list< behaviours::AbstractBehaviour* > myAbstractBehaviours;
    
 private:
	void initializeBehaviours();
	void initializeSensors();

};

#endif // GarbageCleaner_h
