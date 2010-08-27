#include "AvoidObstacle.h"
#include "GarbageCleaner.h"
#include <sstream>

//#define RANGE (1023/4)
#define RANGE (1024/8)

namespace behaviours {
/*
	double braitenberg_coefficients [8][2] =
	  { {150, -35}, {100, -15}, {80, -10}, {-10, -10},
	    {-10, -10}, {-10, 80}, {-30, 100}, {-20, 150} };
*/
	double braitenberg_coefficients [8][2] =
	  { {-0.9, 0.9}, {-0.85, 0.85}, {-0.2, 0.2}, {0.6, 0.6},
	    {0.5, 0.5}, {0.35, -0.35}, {0.8, -0.8}, {0.6, -0.6} };

	AvoidObstacle::AvoidObstacle(robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & dss) : AbstractBehaviour("Avoid Obstacle"){
		this->wheels = wheels;
		this->dss = &dss;
	}

	AvoidObstacle::~AvoidObstacle(){}

    void AvoidObstacle::sense(){
		int j;
		double thresh;
		for (j = 0; j < GET_PARAM(DISTANCE_SENSORS); j++){
			thresh = GET_PARAM(THRESHOLD_OBSTACLE);
			if ( j == 0 || j == GET_PARAM(DISTANCE_SENSORS)-1 )
				thresh = GET_PARAM(FRONT_THRESHOLD_OBSTACLE);

			if ( (*this->dss).at(j)->getValue() > thresh ){
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
				speed[i] += braitenberg_coefficients[j][i] * (*this->dss).at(j)->getValue();
			}
		}
		wheels->setSpeed(speed[0]*GET_PARAM(SPEED_FACTOR),speed[1]*GET_PARAM(SPEED_FACTOR));
	}


} /* End of namespace behaviours */
