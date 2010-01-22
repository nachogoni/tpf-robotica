#ifndef GarbageCleaner_h
#define GarbageCleaner_h

#include <list>

#include "behaviours/AbstractBehaviour.h"
#include "robotapi/IRobot.h"


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
};

#endif // GarbageCleaner_h
