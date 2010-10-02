#ifndef behaviours_GoToGarbage_h
#define behaviours_GoToGarbage_h

#include "AbstractBehaviour.h"
#include <vision/IGarbageRecognition.h>

namespace behaviours {

class GoToGarbage : public AbstractBehaviour
{
	public:
		// class constructor
		GoToGarbage(vision::IGarbageRecognition * gr, robotapi::IDifferentialWheels * wheels );
		// class destructor
		~GoToGarbage();

		void sense();

	    void action();

	private:
        vision::IGarbageRecognition * gr;
   		vision::Garbage * currentGarbage;
		robotapi::IDifferentialWheels * wheels;
		
		double calculateSpeed(double distanceToGarbage);
};

} /* End of namespace behaviours */

#endif // behaviours_GoToGarbage_h
