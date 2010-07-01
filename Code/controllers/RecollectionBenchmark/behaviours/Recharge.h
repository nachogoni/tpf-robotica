#ifndef behaviours_Recharge_h
#define behaviours_Recharge_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <vector>

#define RECHARGE_TIME_STEP 32
#define RECHARGE_ORIENTATION_TOLE 0.1

#define RECHARGE_BACKWARD_STEPS 40
#define RECHARGE_BACKWARD_SPD 100.0

#define RECHARGE_FORWARD_SPD 100.0

#define RECHARGE_BASE_X -0.847015
#define RECHARGE_BASE_Z 0.102655
#define RECHARGE_BASE_ANGLE (3.0*PI/2.0)

#define RECHARGE_BASE_SPD 50.0
#define RECHARGE_SPD_FACTOR 0.7
#define RECHARGE_E_PUCK_DIAMETER 0.052

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
