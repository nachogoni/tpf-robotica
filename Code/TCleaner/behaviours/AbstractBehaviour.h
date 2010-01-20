#ifndef behaviours_AbstractBehaviour_h
#define behaviours_AbstractBehaviour_h

#include <string>

class GarbageCleaner;

namespace behaviours {

class AbstractBehaviour {

 public:

    virtual void sense() = 0;

    virtual void act();

    virtual std::string toString() = 0;

 protected:

    virtual void action()  = 0;

    virtual bool isMyAction();

 public:
    int behaviour_id;

 public:

    GarbageCleaner *myGarbageCleaner;
};

} /* End of namespace behaviours */

#endif // behaviours_AbstractBehaviour_h
