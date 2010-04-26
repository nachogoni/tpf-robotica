#ifndef behaviours_Wander_h
#define behaviours_Wander_h

#include "AbstractBehaviour.h"

#define WANDER_SPD 75.0

namespace behaviours {

class Wander : public AbstractBehaviour {

public:
	Wander::Wander(robotapi::IDifferentialWheels * wheels);

	Wander::~Wander();

    void sense();

	void action();

private:

	robotapi::IDifferentialWheels* wheels;
	};

} /* End of namespace behaviours */

#endif // behaviours_Wander_h