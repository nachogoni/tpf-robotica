#include "WebotsImage.h"
#include <webots/Camera.hpp>
#include <stdio.h>

namespace robotapi {
namespace webts {

	int WebotsImage::getWidth(){
		return this->w;
	}

	int WebotsImage::getHeight(){
		return this->h;
	}

    int WebotsImage::getDepth(){
		return 8;
	}

	IplImage * WebotsImage::toIPL(){
        IplImage * rgb = cvCreateImage( srcSize, IPL_DEPTH_8U, 3 );
        unsigned char g, b, r;
//        printf("IMG width : %d - height : %d\n",this->w,this->h);
        for(int i = 0; i < this->h; i ++ ){
            for(int j = 0; j < this->w; j ++ ){
//                IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3); ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=111; // B ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=112; // G ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=113; // R
				// B G R For IPL...

				// Blue
				b = webots::Camera::imageGetBlue(this->img,this->w,j,i);
				((uchar *)(rgb->imageData + i*rgb->widthStep))[j*rgb->nChannels + 0] = b;

				g = webots::Camera::imageGetGreen(this->img,this->w,j,i);
				((uchar *)(rgb->imageData + i*rgb->widthStep))[j*rgb->nChannels + 1] = g;

				r = webots::Camera::imageGetRed(this->img,this->w,j,i);
				((uchar *)(rgb->imageData + i*rgb->widthStep))[j*rgb->nChannels + 2] = r;
//		        printf("Setting rgb : ( %d , %d , %d ) for pixel ( %d , %d )\n",r,g,b,j,i);
			}
		}
		return rgb;
	}

	void WebotsImage::setImage(const unsigned char * img){
		this->img = img;
	}

	WebotsImage::WebotsImage(int w, int h){
        this->srcSize = cvSize(w,h);
		this->w = w;
		this->h = h;
	}

} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
