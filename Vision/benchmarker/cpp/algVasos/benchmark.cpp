

#include "benchmark.h"
#include <vector>

int focusedFrames;
int guesses;
int guessNot;

using namespace benchmark;

PredictionStats predStats;

int main(int argc, char** argv)
{	    
	CvCapture* capture;
	cvNamedWindow("compare",CV_WINDOW_AUTOSIZE);
	
	initializePredictionStats(&predStats);
	
	if (argc < 3) {
        fprintf(stderr, "Usage: %s filename.xml\n video", argv[0]);
        return 1;
    }
    
    if( (capture=cvCreateFileCapture( argv[2]))== 0 ){
		perror("Invalid capture");
		return 1;
	}
    
    ga= new GarbageAdapter();
    videoInfo vinfo=parseXmlObjects(argv[1]);
    std::list<Frame*> framesList=vinfo.framesList;
    int maxFrameNumber=vinfo.numberOfFrames;
    int nextTestframe=1,h=0,frameStart=0;
    int videoFrameNumber=1;
    bool continueVideo=true;
    
    std::list<Frame*>::iterator frameXml = framesList.begin();
    nextTestframe=(*frameXml)->frameNumber;
    
    std::list<Result*> resultList;
    
    while(h!=frameStart){
		cvQueryFrame(capture);
		h++;
	}
		
    
    
    while((videoFrameImg=cvQueryFrame(capture))!=NULL && videoFrameNumber< maxFrameNumber)
    {
		Result* result=NULL;
		compareImg=cvCloneImage (videoFrameImg);
		
		
		if(videoFrameNumber==nextTestframe){
			//compare FrameXml with FrameImg
			printf(" comparando frame %d\n",videoFrameNumber);
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
				
		cvReleaseImage( &compareImg );
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
		calcPredStats((*it),false);
	}
	
	return res;
		
}
Result*
compareFrameXmlWithFrame(IplImage* frame,Frame* frameXml)
{
	bool found=false;
	int hit=0,miss=0;
	std::list<Cobject*> objects=ga->recognizeObjects(frame);
	std::list<Cobject*> objectsXml=frameXml->getObjects();
	std::list<int> foundVideoContours;
	Result *aResult=new Result(frameXml);
	vector<int> missCount (objects.size(),0);
	
	drawCompare(objects,objectsXml);
	
	
	printf("(%d vid-%d xml)\n",objects.size(),objectsXml.size());
	for (std::list<Cobject*>::iterator itXml = objectsXml.begin(); itXml != objectsXml.end(); itXml++){
		found=false;
		for (std::list<Cobject*>::iterator itVid = objects.begin(); itVid != objects.end(); itVid++){		
			if((*itVid)->isSimilar(*itXml)){
				if(!found){
					//~ printf("found object %d index %d\n",(*itXml)->index,(*itVid)->index);
					aResult->addFound((*itXml)->index);
					found=true;		
					hit++;
					//could check if other detections also are similar to the one in the xml
					//then it would be proper to keep the one which is more similar
				}
			}else{
					//printf("%d no le pego a %d\n",(*itVid)->index,(*itXml)->index);
					missCount[(*itVid)->index]++;
			}
		}
		
	}
	
	for (std::list<Cobject*>::iterator itVid = objects.begin(); itVid != objects.end(); itVid++)
	{
		if(missCount[(*itVid)->index]==objectsXml.size()){
			//~ printf("index %d, no encontro nada \n",(*itVid)->index);
			aResult->addMiss((*itVid));
			miss++;
			calcPredStats((*itVid),false);
		}
		else
			calcPredStats((*itVid),true);
	}
	
	printf("	hits:%d, miss:%d\n",hit,miss);
	
	
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
	printf("--Prediction stats-- \n");
	printf("Number of objects guessed: %d\n",predStats.predicted);
	printf("Number of objects guessed and hit: %d\n",predStats.predictedAndHit);
	printf("Number of objects found by vision system: %d\n",predStats.visualized);
	printf("Number of objects found by vision system and hit: %d\n",predStats.visionAndHit);
	printf("--Windowing stats -- \n");
	printf("Number of objects focused: %d\n",predStats.focused);
	printf("Number of objects focused and hit: %d\n",predStats.focusedAndHit);
	printf("Number of not focused objects: %d\n",predStats.notFocused);
	printf("Number of not focused objects and hit: %d\n",predStats.notFocusedAndHit);
	printf("Number of focused frames: %d\n",focusedFrames);
	printf("--Detection stats-- \n");
	printf(" Total number of objects to be recognized %d\n",allFramesStats.nObjects);
	printf(" number of hits %d\n",allFramesStats.hit);
	printf(" number of detected objects that didn't exist %d\n",allFramesStats.falseDetections);
	printf(" False negative detection  %g\n",1 - (allFramesStats.hit)/(double) allFramesStats.nObjects);
	double falseNegativeDetectionProb=allFramesStats.hit==0?(allFramesStats.falseDetections>0?1:0):
		allFramesStats.falseDetections/( (double)allFramesStats.hit + allFramesStats.falseDetections);
	printf(" False positive detection  %g\n",falseNegativeDetectionProb);
}

void drawCompare(std::list<Cobject*> objects,std::list<Cobject*> objectsXml){
	Cobject * boundingRect;
	
	for (std::list<Cobject*>::iterator itXml = objectsXml.begin(); itXml != objectsXml.end(); itXml++){
		boundingRect=(*itXml);
		cvRectangle(compareImg,cvPoint(boundingRect->x,boundingRect->y),
								cvPoint(boundingRect->x+boundingRect->w,
								boundingRect->y+boundingRect->h),
								_GREEN,1,8,0);
	}
	for (std::list<Cobject*>::iterator itVid = objects.begin(); itVid != objects.end(); itVid++){
		boundingRect=(*itVid);
		cvRectangle(compareImg,cvPoint(boundingRect->x,boundingRect->y),
								cvPoint(boundingRect->x+boundingRect->w,
								boundingRect->y+boundingRect->h),
								_RED,1,8,0);
		std::vector<int> centroid=(*itVid)->getCentroid();
		
		cvCircle(compareImg,cvPoint(centroid[0],centroid[1]), 5, _RED, 1);

		
	}
	cvShowImage("compare",compareImg);
	cvWaitKey(1000/20);
	//~ cvWaitKey(0);
}

void initializePredictionStats(PredictionStats  * stats){

	guessNot=0;
	guesses=0;
	focusedFrames=0;
	stats->predicted=0;
	stats->visualized=0;
	stats->predictedAndHit=0;
	stats->visionAndHit=0;
	stats->predictedAndVisualizedAndHit=0;
	stats->predictedAndVisualized=0;
	stats->focused=0;
	stats->focusedAndHit=0;
	stats->notFocused=0;
	stats->notFocusedAndHit=0;
}
void calcPredStats(Cobject* obj,bool found){
	if(obj->isPredicted()){
			predStats.predicted++;
		}
	if(found && obj->isPredicted() && !obj->isVisualized()){
			predStats.predictedAndHit++;
		}
	if(!obj->isPredicted() && obj->isVisualized())
		predStats.visualized++;
	
	if(found && !obj->isPredicted() && obj->isVisualized())
		predStats.visionAndHit++;
	
	if(obj->isVisualized() && obj->isPredicted()){
		predStats.predictedAndVisualized++;
	}
	if(obj->isFocused())
		predStats.focused++;
	if(obj->isFocused() && found)
		predStats.focusedAndHit++;
	if(!obj->isFocused())
		predStats.notFocused++;
	if(!obj->isFocused() && found)
		predStats.notFocusedAndHit++;
		
	//~ if(found){
		//~ if(obj->isPredicted() && !obj->isVisualized()){
			//~ predStats.predicted++;
			//~ predStats.predictedAndHit++;
		//~ }
		//~ if(obj->isVisualized() && !obj->isPredicted()){
			//~ predStats.visualized++;
			//~ predStats.visionAndHit++;
		//~ }
		//~ if(obj->isPredicted() && obj->isVisualized()){
			//~ predStats.predictedAndVisualized++;
			//~ predStats.predictedAndVisualizedAndHit++;
		//~ }
		//~ if(obj->isFocused()){
			//~ predStats.focused++;
			//~ predStats.focusedAndHit++;
		//~ }
	//~ }else{
		//~ if(obj->isPredicted() && !obj->isVisualized()){
			//~ predStats.predicted++;
		//~ }
		//~ if(obj->isVisualized() && !obj->isPredicted()){
			//~ predStats.visualized++;
		//~ }
		//~ if(obj->isPredicted() && obj->isVisualized()){
			//~ predStats.predictedAndVisualized++;
		//~ }
		//~ if(obj->isFocused()){
			//~ predStats.focused++;
		//~ }
		//~ 
	//~ }
		
}
