#ifndef behaviours_Wander_h
#define behaviours_Wander_h

#include "AbstractBehaviour.h"


namespace behaviours {

class Wander : public AbstractBehaviour {

public:
	Wander();

    void sense();

    std::string toString();
    
    void action();

public:
	std::string s;

	};

} /* End of namespace behaviours */

#endif // behaviours_Wander_h
