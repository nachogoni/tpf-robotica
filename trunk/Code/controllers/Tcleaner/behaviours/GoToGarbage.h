#ifndef behaviours_GoToGarbage_h
#define behaviours_GoToGarbage_h

#include "AbstractBehaviour.h"
#include <utils/GarbageRecognition.h>

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
};

} /* End of namespace behaviours */

#endif // behaviours_GoToGarbage_h
