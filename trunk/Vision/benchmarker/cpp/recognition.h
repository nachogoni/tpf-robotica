#include <stdio.h>
#include "Cobject.h"
#include <list>
#include <opencv/cv.h>



//returns null terminated array of recognized objects
list<benchmark::Cobject*> recognizeObjects(IplImage * img);


