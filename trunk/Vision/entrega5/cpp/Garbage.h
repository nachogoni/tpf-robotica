#ifndef utils_Garbage_h
#define utils_Garbage_h


#include "MinimalBoundingRectangle.h"





class Garbage {

 public:

     double angleTo();

     double distanceTo();

     MinimalBoundingRectangle::MinimalBoundingRectangle * boundingBox();

     Garbage(MinimalBoundingRectangle * hola);

 public:
    double angle;
    double distance;
    MinimalBoundingRectangle::MinimalBoundingRectangle * mbr;

};



#endif // utils_Garbage_h
