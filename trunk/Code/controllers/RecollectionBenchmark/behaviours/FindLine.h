#ifndef behaviours_FindLine_h
#define behaviours_FindLine_h

#include "AbstractBehaviour.h"
#include "WorldInfo.h"
#include <vector>

namespace behaviours {

class FindLine : public AbstractBehaviour {

	public:

		FindLine(WorldInfo * wi, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss);
		
		void sense();

	    void action();

	private:
		std::vector<robotapi::IDistanceSensor*> * fss;
		robotapi::IDifferentialWheels * wheels;
		WorldInfo * wi;
	};

} /* End of namespace behaviours */

#endif // behaviours_FindLine_h
