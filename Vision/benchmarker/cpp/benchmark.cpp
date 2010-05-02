
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "xmlParser.h"
#include "Result.h"
#include "visionInterface.h"
#include "GarbageAdapter.h"

using namespace benchmark;

Result *
compareFrameXmlWithFrame(IplImage* frame,Frame* frameXml);

void
makeGlobalResults(list<Result*>  resultList);

Result*
getFrameResults(IplImage * frame);

GarbageAdapter * ga;

typedef struct {
	int hit;
	int falseDetections;
	int nObjects;
} stats;
	

int main(int argc, char** argv)
{	    
	CvCapture* capture;
	
	//frame from video
	IplImage * videoFrameImg;
	
	if (argc < 3) {
        fprintf(stderr, "Usage: %s filename.xml\n video", argv[0]);
        return 1;
    }
    
    if( (capture=cvCreateFileCapture( argv[2]))== 0 ){
		perror("Invalid capture");
		return 1;
	}
    
    GarbageAdapter * ga= new GarbageAdapter();
    //std::list<Frame*> framesList=parseXmlObjects(argv[1]);
    videoInfo vinfo=parseXmlObjects(argv[1]);
    std::list<Frame*> framesList=vinfo.framesList;
    int maxFrameNumber=vinfo.numberOfFrames;
    int nextTestframe=1;
    int videoFrameNumber=1;
    bool continueVideo=true;
    
    std::list<Frame*>::iterator frameXml = framesList.begin();
    std::list<Result*> resultList;
    
    
    while((videoFrameImg=cvQueryFrame(capture))!=NULL && videoFrameNumber<maxFrameNumber)
    {
		Result* result=NULL;
		
		
		if(videoFrameNumber==nextTestframe){
			printf("Comparando frame %d\n", videoFrameNumber);
			//compare FrameXml with FrameImg
			result=compareFrameXmlWithFrame(videoFrameImg,*frameXml);
			
			
			++frameXml;
			if(frameXml != framesList.end()) 
				nextTestframe=(*frameXml)->frameNumber;
		}
		else{
			printf("no test info for frame %d\n",videoFrameNumber);
			result=getFrameResults(videoFrameImg);
		}
	
		if(result!=NULL)
			resultList.push_back(result);
		videoFrameNumber++;
		

		
	}
	
	//cvReleaseImage( &videoFrameImg );
	cvReleaseCapture( &capture ); 
	makeGlobalResults(resultList);
    
    
}

Result*
getFrameResults(IplImage * frame)
{
	Result * res= new Result();
	std::list<Cobject*> objects=ga->recognizeObjects(frame);
	
	for (std::list<Cobject*>::iterator it = objects.begin(); it != objects.end(); it++){
		res->addMiss((*it));
	}
	
	return res;
		
}
Result*
compareFrameXmlWithFrame(IplImage* frame,Frame* frameXml)
{
	bool found=false;
	std::list<Cobject*> objects=ga->recognizeObjects(frame);
	std::list<Cobject*> objectsXml=frameXml->getObjects();
	std::list<int> missContours;
	Result *aResult=new Result(frameXml);
	
	for (std::list<Cobject*>::iterator itXml = objectsXml.begin(); itXml != objectsXml.end(); itXml++){
		for (std::list<Cobject*>::iterator itVid = objects.begin(); itVid != objects.end(); itVid++){
			if( (*itVid)->isSimilar(*itXml)){
				if(!found){
					printf("found object %d index\n",(*itXml)->index);
					aResult->addFound((*itXml)->index);
					found=true;
					
					//could check if other detections also are similar to the one in the xml
					//then it would be proper to keep the one which is more similar
				}
			}else{
				missContours.push_back((*itVid)->index);
			}
		}
	}
	
	
	
	return aResult;
}

void
makeGlobalResults(list<Result*>  resultList){
	stats allFramesStats;
	stats testedFrames;
	
	allFramesStats.nObjects=0;
	allFramesStats.falseDetections=0;
	allFramesStats.hit=0;
	
	for (std::list<Result*>::iterator itRes = resultList.begin(); itRes != resultList.end(); itRes++){
		allFramesStats.hit+=(*itRes)->detectedObjects();
		allFramesStats.falseDetections+=(*itRes)->falsePositives;
		allFramesStats.nObjects+=(*itRes)->nObjects;
	}
	
	printf(" Total number of objects to be recognized %d\n",allFramesStats.nObjects);
	printf(" number of hits %d\n",allFramesStats.hit);
	printf(" number of detected objects that didn't exist %d\n",allFramesStats.falseDetections);
	printf(" False positive detection  %g\n",(allFramesStats.hit + allFramesStats.falseDetections)/(double) allFramesStats.nObjects);
	double falseNegativeDetectionProb=allFramesStats.hit==0?(allFramesStats.falseDetections>0?1:0):
		allFramesStats.falseDetections/( (double)allFramesStats.hit+allFramesStats.falseDetections);
	printf(" False negative detection  %g\n",falseNegativeDetectionProb);
}
