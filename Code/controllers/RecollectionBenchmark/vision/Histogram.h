#ifndef vision_Histogram_h
#define vision_Histogram_h

#include <cv.h>
#include <highgui.h>

namespace vision{

	class Histogram {
		
		public:

			CvHistogram * getHShistogramFromHS(IplImage* hChannel,IplImage* sChannel);

			CvHistogram * getHShistogramFromRGB(IplImage* src);
			
			Histogram(int h_bins,int s_bins);
	
		private:
		    int s_bins;
		    int h_bins;
	
	};

}

#endif
