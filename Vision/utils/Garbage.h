#ifndef utils_Garbage_h
#define utils_Garbage_h

#include "Rectangle.h"

namespace utils {
class GarbageRecognition;
} /* End of namespace utils */

namespace utils {

class Garbage {

 public:

    virtual double angleTo();

    virtual double distanceTo();

    virtual Rectangle boundingBox();

    Garbage();

 public:
    double angle;
    double distance;
    Rectangle mbr;

 public:

    GarbageRecognition *myGarbageRecognition;

    Rectangle *myRectangle;
};

} /* End of namespace utils */

#endif // utils_Garbage_h
