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
        for (int j = 0; j < FLOOR_SENSORS; j++){
			printf("Floor sensor %d: %d\n", j, (*this->fss).at(j)->getValue() );
		}
	}

    void FindLine::action(){

		double currentOrientation = wheels->getOrientation();
		double destinyAngle = HALF_PI;
		double lspd = FIND_LINE_BASE_SPD;
		double rspd = FIND_LINE_BASE_SPD;
		
		if ( fabs(destinyAngle - currentOrientation) >= FIND_LINE_ANGLE_TOLE ){
			if ( currentOrientation > HALF_PI && currentOrientation < THREE_HALF_PI ){
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
