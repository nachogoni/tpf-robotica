#include <iostream>
#include <cv.h>
#include <vector>
#include <list>
#include <highgui.h>
#include "xmlParser.h"
#include "Result.h"

#include "GarbageAdapter.h"

using namespace benchmark;


Result *
compareFrameXmlWithFrame(IplImage* frame,Frame* frameXml);

void
makeGlobalResults(list<Result*>  resultList);

Result*
getFrameResults(IplImage * frame);

void
drawCompare(std::list<Cobject*>,std::list<Cobject*>);


GarbageAdapter * ga;

#define _RED  cvScalar (0, 0, 255, 0)
#define _GREEN cvScalar (0, 255, 0, 0)

typedef struct {
	int hit;
	int falseDetections;
	int nObjects;
} stats;

//frame from video
IplImage * videoFrameImg;
IplImage * compareImg;
