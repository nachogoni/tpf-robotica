#ifndef robotapi_real_RealCamera_h
#define robotapi_real_RealCamera_h

#include <cv.h>
#include <highgui.h>
#include <robotapi/ICamera.h>



namespace robotapi {
namespace real {

class RealCamera : virtual public robotapi::ICamera {

	public:
		RealCamera(int cameraID);
		void enable(int ms);

		void disable();

		IImage & getImage();

		int saveImage(std::string filename, int quality);
	private:
		CvCapture* capture;
		IImage * iim;
	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealCamera_h
