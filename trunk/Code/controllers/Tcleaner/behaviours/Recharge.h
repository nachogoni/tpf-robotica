#ifndef behaviours_Recharge_h
#define behaviours_Recharge_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <vector>

namespace behaviours {

class Recharge : public AbstractBehaviour {

	public:

        Recharge(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels, WorldInfo * wi, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, std::vector<robotapi::IDistanceSensor*> & fss);

		void sense();
		
		void action();
		
	private:
	    robotapi::IBattery * robotBattery;
		robotapi::IBattery * pcBattery;
		robotapi::IRobot * robot;
		robotapi::IDifferentialWheels * wheels;
		std::vector<robotapi::IDistanceSensor*> * fss;
		WorldInfo * wi;
	};

} /* End of namespace behaviours */

#endif // behaviours_Recharge_h
