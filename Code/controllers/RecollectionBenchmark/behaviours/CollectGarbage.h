#ifndef behaviours_CollectGarbage_h
#define behaviours_CollectGarbage_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <utils/IGarbageRecognition.h>

namespace behaviours {

class CollectGarbage : public AbstractBehaviour {

	public:

		CollectGarbage(utils::IGarbageRecognition * gr, robotapi::IRobot * robot, robotapi::ITrashBin * tb, robotapi::IDifferentialWheels * wheels, WorldInfo * wi, robotapi::IServo * shovel, robotapi::IServo * container);
		~CollectGarbage();

		void sense();

	    void action();

	private:
		robotapi::IServo * shovel;
		robotapi::IServo * cont;
		robotapi::IRobot * robot;
		robotapi::IDifferentialWheels * wheels;
		WorldInfo * wi;
		robotapi::ITrashBin * trashBin;
		utils::IGarbageRecognition * gr;
   		utils::Garbage * currentGarbage;
	};

} /* End of namespace behaviours */

#endif // behaviours_CollectGarbage_h
