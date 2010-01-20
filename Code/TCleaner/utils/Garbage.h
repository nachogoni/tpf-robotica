#ifndef utils_Garbage_h
#define utils_Garbage_h

namespace utils {
class GarbageRecognition;
class Rectangle;
} /* End of namespace utils */

namespace utils {

class Garbage {

 public:

    virtual double angleTo();

    virtual double distanceTo();

    virtual Rectangle boundingBox();

};

} /* End of namespace utils */

#endif // utils_Garbage_h
