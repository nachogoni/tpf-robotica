#ifndef behaviours_GoToBase_h
#define behaviours_GoToBase_h

#include "AbstractBehaviour.h"
#include <vector>

namespace behaviours {

class GoToBase : public AbstractBehaviour {

	public:

		GoToBase(robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss);

		void sense();

	    void action();

	private:
		std::vector<robotapi::IDistanceSensor*> * fss;
		robotapi::IDifferentialWheels * wheels;

	};

} /* End of namespace behaviours */

#endif // behaviours_GoToBase_h
