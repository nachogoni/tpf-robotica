#ifndef behaviours_RemoveFromStuck_h
#define behaviours_RemoveFromStuck_h

#include "AbstractBehaviour.h"
#include <vector>

#define ACTIVATION_STEPS 2000
#define REMOVE_ANGLE_TOLE 0.05
#define REMOVE_SPD 20
#define REMOVE_STEP 32
#define REMOVE_DESVSTD_ANGLE_THRESHOLD 0.1
#define REMOVE_DESVSTD_DIST_THRESHOLD 0.01

namespace behaviours {

class RemoveFromStuck : public AbstractBehaviour {

public:
	RemoveFromStuck(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels);

	~RemoveFromStuck();
	
    void sense();

    void action();

	double desvStd(double values [], double sum);

private:

	robotapi::IRobot * robot;
	robotapi::IDifferentialWheels * wheels;
	unsigned int steps;
	double angles [ACTIVATION_STEPS];
	double xs [ACTIVATION_STEPS];
	double zs [ACTIVATION_STEPS];
	double sumA;
	double sumX;
	double sumZ;

	};

} /* End of namespace behaviours */

#endif // behaviours_RemoveFromStuck_h
