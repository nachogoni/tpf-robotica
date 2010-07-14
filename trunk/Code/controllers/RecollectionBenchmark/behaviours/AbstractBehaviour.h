#ifndef behaviours_AbstractBehaviour_h
#define behaviours_AbstractBehaviour_h

#include <string>
#include <robotapi/IRobot.h>
#include <robotapi/webts/WebotsRobot.h>

#define LINE_UP_THRESHOLD 350
#define LINE_DOWN_THRESHOLD 175

class GarbageCleaner;

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
	void setGarbageCleaner(GarbageCleaner * garbageCleaner);
};

} /* End of namespace behaviours */

#endif // behaviours_AbstractBehaviour_h
