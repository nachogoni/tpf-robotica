#ifndef behaviours_GoToGarbage_h
#define behaviours_GoToGarbage_h

#include "AbstractBehaviour.h"
#include <utils/GarbageRecognition.h>

#define GO_TO_GARBAGE_ANGLE_TOLE 0.1
#define GO_TO_GARBAGE_MAX_SPD 100
#define GO_TO_GARBAGE_MIN_SPD 10

namespace behaviours {

class GoToGarbage : public AbstractBehaviour
{
	public:
		// class constructor
		GoToGarbage(utils::GarbageRecognition * gr, robotapi::IDifferentialWheels * wheels );
		// class destructor
		~GoToGarbage();

		void sense();

	    void action();

	private:
        utils::GarbageRecognition * gr;
   		utils::Garbage * currentGarbage;
		robotapi::IDifferentialWheels * wheels;
		
		double calculateSpeed(double distanceToGarbage);
};

} /* End of namespace behaviours */

#endif // behaviours_GoToGarbage_h
