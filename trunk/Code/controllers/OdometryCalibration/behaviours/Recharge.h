#ifndef behaviours_Recharge_h
#define behaviours_Recharge_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"

namespace behaviours {

class Recharge : public AbstractBehaviour {

	public:

        Recharge(WorldInfo * wi, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt);

		void sense();
		
		void action();
		
	private:
	    robotapi::IBattery * robotBattery;
		robotapi::IBattery * pcBattery;
		WorldInfo * wi;
	};

} /* End of namespace behaviours */

#endif // behaviours_Recharge_h
