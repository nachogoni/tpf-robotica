#ifndef robotapi_real_RealCamera_h
#define robotapi_real_RealCamera_h

#include <robotapi/ICamera.h>



namespace robotapi {
namespace real {

class RealCamera : virtual public robotapi::ICamera {

	public:
		void enable(int ms);

		void disable();

		IImage & getImage();

		int saveImage(std::string filename, int quality);

	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealCamera_h
