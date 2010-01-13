#ifndef utils_Garbage_h
#define utils_Garbage_h


#include "Rectangle.h"





class Garbage {

 public:

     double angleTo();

     double distanceTo();

     Rectangle* boundingBox();

     Garbage(Rectangle * hola);

 public:
    double angle;
    double distance;
    Rectangle * mbr;

};



#endif // utils_Garbage_h
