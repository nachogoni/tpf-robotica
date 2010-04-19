#include "AvoidObstacle.h"
#include "GarbageCleaner.h"
#include <sstream>

//#define RANGE (1023/4)
#define RANGE (1024/8)

namespace behaviours {

	double braitenberg_coefficients [8][2] =
	  { {150, -35}, {100, -15}, {80, -10}, {-10, -10},
	    {-10, -10}, {-10, 80}, {-30, 100}, {-20, 150} };

	AvoidObstacle::AvoidObstacle(robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & dss) : AbstractBehaviour("Avoid Obstacle"){
		this->wheels = wheels;
		this->dss = &dss;
	}

	AvoidObstacle::~AvoidObstacle(){}

    void AvoidObstacle::sense(){
		int j;

		for (j = 0; j < DISTANCE_SENSORS; j++){
			if ( (*this->dss).at(j)->getValue() > THRESHOLD_OBSTACLE ){
				setStimulusPresent();
				// TODO Set obstacle side
			}
		}
	}

    void AvoidObstacle::action(){
		int j,i;
		double speed [2] = {0,0};
		for (i = 0; i < 2; i++) {
			speed[i] = 0.0;
			for (j = 0; j < 8; j++) {
				speed[i] += braitenberg_coefficients[j][i] * (1.0 - ((*this->dss).at(j)->getValue() / RANGE));
			}
		}
		wheels->setSpeed(speed[0],speed[1]);
	}


} /* End of namespace behaviours */
