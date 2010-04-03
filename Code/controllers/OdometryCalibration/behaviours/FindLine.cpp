#include "FindLine.h"
#include "GarbageCleaner.h"

namespace behaviours {

	FindLine::FindLine(WorldInfo * wi, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Find Line"){
		this->wheels = wheels;
		this->fss = &fss;
		this->wi = wi;
	}

	void FindLine::sense(){
        for (int j = 0; j < FLOOR_SENSORS; j++){
			printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
		}
	}

    void FindLine::action(){}

} /* End of namespace behaviours */
