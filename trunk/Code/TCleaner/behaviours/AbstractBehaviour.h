#ifndef behaviours_AbstractBehaviour_h
#define behaviours_AbstractBehaviour_h

#include <string>
#include <robotapi/IRobot.h>


class GarbageCleaner;

namespace behaviours {

class AbstractBehaviour {

 public:

    virtual void sense() = 0;

    virtual void act();

    virtual std::string toString() = 0;

	AbstractBehaviour();

 protected:

    virtual void action()  = 0;

    virtual bool isMyAction();

	virtual void setStimulusPresent();

 public:
    int behaviour_id;
	void setGarbageCleaner(GarbageCleaner * garbageCleaner);
};

} /* End of namespace behaviours */

#endif // behaviours_AbstractBehaviour_h
