#ifndef behaviours_Wander_h
#define behaviours_Wander_h

#include "AbstractBehaviour.h"
#include <vector>
#include <utils/ArenaGridSlot.h>
#include "WorldInfo.h"

#define WANDER_SPD 75.0

namespace behaviours {

class Wander : public AbstractBehaviour {

	public:
		Wander(WorldInfo * wi, robotapi::IDifferentialWheels * wheels);
	
		~Wander();
	
	    void sense();
	
		void action();

	private:
		WorldInfo * wi;
		robotapi::IDifferentialWheels* wheels;

		int getOldestSlot(std::vector<utils::ArenaGridSlot *> nb);
	};

} /* End of namespace behaviours */

#endif // behaviours_Wander_h
