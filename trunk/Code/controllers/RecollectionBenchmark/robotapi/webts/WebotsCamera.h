#ifndef robotapi_webts_WebotsCamera_h
#define robotapi_webts_WebotsCamera_h

#include <robotapi/ICamera.h>
#include <robotapi/webts/WebotsDevice.h>
#include <webots/Camera.hpp>

namespace robotapi {
namespace webts {

class WebotsCamera : virtual public robotapi::ICamera, public robotapi::webts::WebotsDevice {

public:

    void enable(int ms);

    void disable();

    IImage &getImage();

    int saveImage(std::string filename, int quality);

    // Change parameter to Webots API camera
    WebotsCamera( webots::Camera & cam );
    
private:

	webots::Camera * mycam;

    };

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsCamera_h
