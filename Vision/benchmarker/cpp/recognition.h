#include <stdio.h>
#include "Cobject.h"
#include <opencv/cv.h>


//returns null terminated array of recognized objects
benchmark::Cobject * recognizeObjects(IplImage * img);
