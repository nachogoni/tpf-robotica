
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

Result*
getFrameResults(IplImage * frame);



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
    
    std::list<Frame*> framesList=parseXmlObjects(argv[1]);
    printf("frames %d",framesList.size());
    
    int nextTestframe=1;
    int videoFrameNumber=1;
    bool continueVideo=true;
    
    std::list<Frame*>::iterator frameXml = framesList.begin();
    std::list<Result*> resultList;
    
    
    while(videoFrameImg=cvQueryFrame(capture))
    {
		Result* result;
		
		
		if(videoFrameNumber==nextTestframe){
			printf("Comparando frame %d\n", videoFrameNumber);
			result=compareFrameXmlWithFrame(videoFrameImg,*frameXml);
			//compare FrameXml with FrameImg
			
			++frameXml;
			if(frameXml != framesList.end()) 
				nextTestframe=(*frameXml)->frameNumber;
		}
		else{
			printf("no test info for frame %d\n",videoFrameNumber);
			result=getFrameResults(videoFrameImg);
		}
		
		resultList.push_back(result);
		videoFrameNumber++;
		
	}
    
    
}

Result*
getFrameResults(IplImage * frame)
{
	GarbageAdapter * ga= new GarbageAdapter();
	std::list<Cobject*> objects=ga->recognizeObjects(frame);
	
	return new Result();		
}
Result*
compareFrameXmlWithFrame(IplImage* frame,Frame* frameXml)
{
	bool found=false;
	GarbageAdapter * ga= new GarbageAdapter();
	std::list<Cobject*> objects=ga->recognizeObjects(frame);
	std::list<Cobject*> objectsXml=frameXml->getObjects();
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
			}
		}
	}
	
	
	
	return new Result(frameXml);
}
