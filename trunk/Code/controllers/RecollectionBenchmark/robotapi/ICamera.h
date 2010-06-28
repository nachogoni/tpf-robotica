#ifndef robotapi_ICamera_h
#define robotapi_ICamera_h

#include "IDevice.h"
#include "IImage.h"
#include <string>

namespace utils {
class GarbageRecognition;
} /* End of namespace utils */

namespace robotapi {

class ICamera : virtual public IDevice {

 public:

    virtual void enable(int ms)  = 0;

    virtual void disable()  = 0;

    virtual IImage &getImage()  = 0;

    virtual int saveImage(std::string filename, int quality)  = 0;

    // virtual destructor for interface
    virtual ~ICamera() { }

    utils::GarbageRecognition *myGarbageRecognition;

    IImage *myIImage;
};

} /* End of namespace robotapi */

#endif // robotapi_ICamera_h
