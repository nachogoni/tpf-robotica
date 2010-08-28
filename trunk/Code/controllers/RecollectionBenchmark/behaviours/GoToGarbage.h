#ifndef behaviours_GoToGarbage_h
#define behaviours_GoToGarbage_h

#include "AbstractBehaviour.h"
#include <utils/IGarbageRecognition.h>

namespace behaviours {

class GoToGarbage : public AbstractBehaviour
{
	public:
		// class constructor
		GoToGarbage(utils::IGarbageRecognition * gr, robotapi::IDifferentialWheels * wheels );
		// class destructor
		~GoToGarbage();

		void sense();

	    void action();

	private:
        utils::IGarbageRecognition * gr;
   		utils::Garbage * currentGarbage;
		robotapi::IDifferentialWheels * wheels;
		
		double calculateSpeed(double distanceToGarbage);
};

} /* End of namespace behaviours */

#endif // behaviours_GoToGarbage_h
