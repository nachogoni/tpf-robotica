#ifndef behaviours_Wander_h
#define behaviours_Wander_h

#include "AbstractBehaviour.h"
#include <vector>
#include <utils/ArenaGridSlot.h>
#include "WorldInfo.h"



namespace behaviours {

class Wander : public AbstractBehaviour {

	public:
		Wander(WorldInfo * wi, robotapi::IDifferentialWheels * wheels);
	
		~Wander();
	
	    void sense();
	
		void action();

		void comingFromBase();

	private:
		WorldInfo * wi;
		robotapi::IDifferentialWheels* wheels;

		bool isComingFromBase;
		bool leftWasLast;
		double turnFactor;
		int steps;

		utils::ArenaGridSlot * getOldestSlot(std::vector<utils::ArenaGridSlot *> nb);
	};

} /* End of namespace behaviours */

#endif // behaviours_Wander_h
