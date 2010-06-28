#ifndef robotapi_real_RealImage_h
#define robotapi_real_RealImage_h

#include <robotapi/IImage.h>



namespace robotapi {
namespace real {

class RealImage : virtual public robotapi::IImage {

	public:
		int getWidth();

		int getHeight();

		int getDepth();

		IplImage * toIPL();
	};

} /* End of namespace robotapi::real */
} /* End of namespace robotapi */

#endif // robotapi_real_RealImage_h
