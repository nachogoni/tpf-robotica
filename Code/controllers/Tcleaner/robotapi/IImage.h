#ifndef robotapi_IImage_h
#define robotapi_IImage_h

#include <highgui.h>

namespace robotapi {
class ICamera;
} /* End of namespace robotapi */

namespace robotapi {

class IImage {

 public:

    virtual int getWidth()  = 0;

    virtual int getHeight()  = 0;

    virtual int getDepth()  = 0;

	virtual IplImage * toIPL()  = 0;

public:
    // virtual destructor for interface 
    virtual ~IImage() { }

 public:

    ICamera *myICamera;
};

} /* End of namespace robotapi */

#endif // robotapi_IImage_h
