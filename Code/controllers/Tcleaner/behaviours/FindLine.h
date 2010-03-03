#ifndef behaviours_FindLine_h
#define behaviours_FindLine_h

#include "AbstractBehaviour.h"


namespace behaviours {

class FindLine : public AbstractBehaviour {

	public:

		FindLine(robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt, robotapi::IDifferentialWheels * wheels);
		
		void sense();

	    void action();

	private:
		robotapi::IBattery * robotBatt;
		robotapi::IBattery * pcBatt;
		robotapi::IDifferentialWheels * wheels;
	};

} /* End of namespace behaviours */

#endif // behaviours_FindLine_h
