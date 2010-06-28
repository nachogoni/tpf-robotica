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

		IplImage * toIPL();

		WebotsImage(const unsigned char * img, int w, int h);

	private:
		const unsigned char * img;
		int w;
		int h;
		CvSize srcSize;
	};
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */

#endif // robotapi_webts_WebotsImage_h
