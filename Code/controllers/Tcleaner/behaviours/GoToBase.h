#ifndef behaviours_GoToBase_h
#define behaviours_GoToBase_h

#include "AbstractBehaviour.h"


namespace behaviours {

class GoToBase : public AbstractBehaviour {

	public:

		GoToBase(robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, robotapi::IDifferentialWheels * wheels);

		void sense();

	    void action();

	private:
		robotapi::IBattery * robotBatt;
		robotapi::IBattery * pcBatt;
		robotapi::IDifferentialWheels * wheels;



	};

} /* End of namespace behaviours */

#endif // behaviours_GoToBase_h
