#ifndef behaviours_RemoveFromStuck_h
#define behaviours_RemoveFromStuck_h

#include "AbstractBehaviour.h"
#include <vector>

#define ACTIVATION_STEPS 2000

namespace behaviours {

class RemoveFromStuck : public AbstractBehaviour {

public:
	RemoveFromStuck(robotapi::IRobot * robot, robotapi::IDifferentialWheels * wheels);

	~RemoveFromStuck();
	
    void sense();

    void action();

private:

	robotapi::IRobot * robot;
	robotapi::IDifferentialWheels * wheels;
	unsigned int steps;

	};

} /* End of namespace behaviours */

#endif // behaviours_RemoveFromStuck_h
