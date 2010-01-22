#ifndef robotapi_webts_WebotsImage_h
#define robotapi_webts_WebotsImage_h

#include <robotapi/IImage.h>



namespace robotapi {
namespace webts {

class WebotsImage : virtual public robotapi::IImage {

public:
	int getWidth();

    int getHeight();

    int getDepth();

	WebotsImage(unsigned char * img);

	};
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsImage_h
