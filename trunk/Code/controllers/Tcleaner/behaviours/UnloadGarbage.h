#ifndef behaviours_UnloadGarbage_h
#define behaviours_UnloadGarbage_h

#include "AbstractBehaviour.h"

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
