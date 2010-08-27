#include "FindLine.h"
#include "GarbageCleaner.h"
#include <math.h>

namespace behaviours {

	FindLine::FindLine(WorldInfo * wi, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Find Line"){
		this->wheels = wheels;
		this->fss = &fss;
		this->wi = wi;
	}

	void FindLine::sense(){
        for (int j = 0; j < GET_PARAM(FLOOR_SENSORS); j++){
			printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
		}
	}

    void FindLine::action(){

		double currentOrientation = wheels->getOrientation();
		double destinyAngle = GET_PARAM(HALF_PI);
		double lspd = GET_PARAM(FIND_LINE_BASE_SPD);
		double rspd = GET_PARAM(FIND_LINE_BASE_SPD);
		
		if ( fabs(destinyAngle - currentOrientation) >= GET_PARAM(FIND_LINE_ANGLE_TOLE) ){
			if ( currentOrientation > GET_PARAM(HALF_PI) && currentOrientation < GET_PARAM(THREE_HALF_PI) ){
			    rspd *= -1;
			}else{
			    lspd *= -1;
			}
		}else{
			rspd *= 2;
			lspd *= 2;
		}
	    wheels->setSpeed(lspd,rspd);
	}

} /* End of namespace behaviours */
