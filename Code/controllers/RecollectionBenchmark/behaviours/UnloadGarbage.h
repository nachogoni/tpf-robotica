#ifndef behaviours_UnloadGarbage_h
#define behaviours_UnloadGarbage_h

#include "AbstractBehaviour.h"

#define UNLOAD_TIME_STEP 32
#define UNLOAD_ORIENTATION_TOLE 0.1

#define UNLOAD_BACKWARD_STEPS 75
#define UNLOAD_BACKWARD_SPD 100.0

#define UNLOAD_FORWARD_STEPS 45
#define UNLOAD_FORWARD_SPD 100.0

#define UNLOAD_BASE_X -0.847015
#define UNLOAD_BASE_Z 0.102655
#define UNLOAD_BASE_ANGLE (3.0*PI/2.0)

#define UNLOAD_WAIT_STEPS 80
#define UNLOAD_E_PUCK_DIAMETER 0.052

#define UNLOAD_BASE_SPD 50.0
#define UNLOAD_SPD_FACTOR 0.7


namespace behaviours {

class UnloadGarbage : public AbstractBehaviour {

	public:
		UnloadGarbage(robotapi::ITrashBin * tb, robotapi::IServo * gate, robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss);

		~UnloadGarbage();

		void sense();

	    void action();

	private:
		void positionSelf();
		void goDistance(double distance);
		void alignWithLine();
		void followLine(bool backwards, double distance);
    	void followLineSpd(bool backwards);

		robotapi::IServo * gate;
		robotapi::ITrashBin * trashbin;
		robotapi::IRobot * robot;
		robotapi::IDifferentialWheels * wheels;
		std::vector<robotapi::IDistanceSensor*> * fss;
		int timesEntered;
	};

} /* End of namespace behaviours */

#endif // behaviours_UnloadGarbage_h
