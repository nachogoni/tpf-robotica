#ifndef GarbageCleaner_h
#define GarbageCleaner_h

#include <vector>

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
    std::vector< behaviours::AbstractBehaviour* > myAbstractBehaviour;
};

#endif // GarbageCleaner_h
