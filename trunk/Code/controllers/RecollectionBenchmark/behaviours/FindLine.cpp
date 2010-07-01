#include "FindLine.h"
#include "GarbageCleaner.h"
#include <math.h>

#define ANGLE_TOLE 0.07
#define BASE_SPD 50

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
		double destinyAngle = PI/2;
		
		if ( fabs(destinyAngle - currentOrientation) < ANGLE_TOLE )
		    wheels->setSpeed(BASE_SPD,BASE_SPD);
		else{
			if ( currentOrientation > HALF_PI && currentOrientation < THREE_HALF_PI )
			    wheels->setSpeed(BASE_SPD,-BASE_SPD);
			else
			    wheels->setSpeed(-BASE_SPD,BASE_SPD);
		}
		/*
		utils::MyPoint * linepos = new utils::MyPoint(l->getX(),l->getY());

		utils::MyVector * direction = new utils::MyVector(wheels->getOrientation());
		utils::MyVector * vectorToLine = new utils::MyVector(wheels->getPosition(),linepos);
		printf("line position: ( %g ; %g )\n",l->getX(),l->getY());
		printf("angle to line: %g\n",wheels->getOrientation()+direction->angleTo(vectorToLine));
		*/
	}

} /* End of namespace behaviours */
