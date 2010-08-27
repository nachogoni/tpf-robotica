#ifndef behaviours_Recharge_h
#define behaviours_Recharge_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <vector>



namespace behaviours {

class Recharge : public AbstractBehaviour {

	public:

        Recharge(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, std::vector<robotapi::IDistanceSensor*> & fss);

		void sense();
		
		void action();
	
	private:
		void positionSelf();
		void alignWithLine();
		void followLine(bool backwards, double distance);
    	void followLineSpd(bool backwards);
		void goDistance(double distance);

		bool isCharging();

		double lastRobotValue;
		double lastPCValue;
	    robotapi::IBattery * robotBattery;
		robotapi::IBattery * pcBattery;
		robotapi::IRobot * robot;
		robotapi::IDifferentialWheels * wheels;
		std::vector<robotapi::IDistanceSensor*> * fss;
	};

} /* End of namespace behaviours */

#endif // behaviours_Recharge_h
