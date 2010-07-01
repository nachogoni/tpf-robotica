#ifndef behaviours_CollectGarbage_h
#define behaviours_CollectGarbage_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <utils/GarbageRecognition.h>

#define BOTTOM_SERVO_ANGLE 0.73
#define COLLECT_TIME_STEP 32
#define COLLECT_BASE_SPD 50.0

#define COLLECT_WAIT_STEPS 80
#define COLLECT_WAIT_STEPS_A_LAST 30
#define COLLECT_WAIT_STEPS_LAST 10

#define COLLECT_ANGLE_TOLE 0.1

// Experimental
#define DISTANCE_TOLE 0.0581187

#define SHOVEL_ANGLE -0.1

namespace behaviours {

class CollectGarbage : public AbstractBehaviour {

	public:

		CollectGarbage(utils::GarbageRecognition * gr, robotapi::IRobot * robot, robotapi::ITrashBin * tb, robotapi::IDifferentialWheels * wheels, WorldInfo * wi, robotapi::IServo * shovel);
		~CollectGarbage();

		void sense();

	    void action();

	private:
		robotapi::IServo * shovel;
		robotapi::IRobot * robot;
		robotapi::IDifferentialWheels * wheels;
		WorldInfo * wi;
		robotapi::ITrashBin * trashBin;
		utils::GarbageRecognition * gr;
   		utils::Garbage * currentGarbage;
	};

} /* End of namespace behaviours */

#endif // behaviours_CollectGarbage_h
