#ifndef behaviours_AbstractBehaviour_h
#define behaviours_AbstractBehaviour_h

#include <string>
#include <robotapi/IRobot.h>
#include <behaviours/BehavioursParameters.h>

namespace behaviours {

class AbstractBehaviour {

 public:

    virtual void sense() = 0;

    void act();

    std::string toString();

	AbstractBehaviour(char * name);
	
	virtual ~AbstractBehaviour()  = 0;

    virtual void action()  = 0;

    bool isMyAction();

	void setStimulusPresent();

	static void resetStimulusPresent();

	bool inLine(double value);
 public:
    int behaviour_id;
   	std::string s;
};

} /* End of namespace behaviours */

#endif // behaviours_AbstractBehaviour_h
