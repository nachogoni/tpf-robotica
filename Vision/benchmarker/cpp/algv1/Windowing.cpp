
#include "GarbageHistoric.h"
#include <vector>
#include <list>
#include <stdio.h>
#include <highgui.h>
#include <cv.h>
#include "Garbage.h"
#include "PredictionParameters.h"
#include "Windowing.h"



namespace utils{

	Windowing::~Windowing(){
		//if(windowImage!=NULL){
		//	cvReleaseImage(&windowImage);
		//}
		//delete this->currentGarbage;
		
	}
	Windowing::Windowing(){
		this->frameNumber=0;
	}

	Windowing::Windowing(GarbageHistoric * focusGarbage,CvSize srcSize){
		this->srcSize=srcSize;
		this->frameNumber=0;
		this->currentGarbage=focusGarbage;
		
		MinimalBoundingRectangle * mbr= focusGarbage->garbage->boundingBox();
		int factor=(focusGarbage->age - focusGarbage->lastAppeareance);
		
		int w=100;//mbr->width;
		int h=100;//mbr->height;
		int x=mbr->x;
		int y=mbr->y;
		

		this->window=cvRect(x,y,w,h);
		this->windowImage=cvCreateImage(srcSize,8,3);
		this->tmpMat=cvCreateMat(this->window.height, this->window.width,CV_8UC1);
		//~ this->windowImage=NULL;
		this->last_last_window=NULL;
		this->last_window=NULL;
		this->release_window=false;
		
	}
	
	bool Windowing::isValidWindow(CvRect rect){
		
		if(rect.x<0 || rect.y <0)
			return false;
		if(rect.x>srcSize.width || rect.y>srcSize.height)
			return false;
		if(rect.x+rect.width>srcSize.width || rect.y+rect.height>srcSize.height)
			return false;
		
		return true;
	}
	CvRect Windowing::adjustWindow(CvRect rect){
		
		rect.x=rect.x<0?0:rect.x; 
		rect.y=rect.y<0?0:rect.y;
		
		rect.width=rect.x+rect.width>srcSize.width?(srcSize.width-rect.x)-1:rect.width;
		rect.height=rect.y+rect.height>srcSize.height?(srcSize.height-rect.y)-1:rect.height;
		
		
		return rect;
	}
	
	IplImage * Windowing::getWindow(IplImage * src){
		Garbage * newGarbage=this->currentGarbage->guessPosition();
		std::vector<int> newPos = newGarbage->getCentroid();
		
		this->window.x =  newPos[0]- this->window.width/2;
		this->window.y =  newPos[1]- this->window.height/2;
		
		if(!isValidWindow(this->window)){
			this->window=this->adjustWindow(this->window);
		}
		
		
		
		if(this->window.width < 10|| 
			this->window.height <10 ){
				return NULL;
		}
		if(this->window.width < 10|| 
			this->window.height <10 ){
				return NULL;
		}
		
		printf("new window (%d,%d) %d %d \n",this->window.x,this->window.y,this->window.width,this->window.height);

		
		//~ bk=cvCreateImage(cvSize(this->window.height,this->window.width),8,3);
		
		//~ tmp =cvCreateMat(this->window.height, this->window.width,CV_8UC1);
		
		cvGetSubRect(src, this->tmpMat , this->window);
		
		this->windowImage=cvGetImage(this->tmpMat, this->windowImage);
		
		
		//~ cvReleaseMat(&tmp);
		//delete newGarbage;
		return this->windowImage;
	}
	
	//transforms window coordinates to big image coordinates
	std::list<Garbage*> Windowing::correctGarbages(std::list<Garbage*> garbageList){
		std::list<Garbage*>  newGarbageList;
		std::list<Garbage*>::iterator itGar;
		std::vector<int> newCentroid;
		Garbage * movedGarbage;
		MinimalBoundingRectangle * newMbr,*oldMbr;
		
		for(itGar = garbageList.begin(); itGar != garbageList.end(); itGar++){
			MinimalBoundingRectangle * oldMbr=(*itGar)->boundingBox();
			int x=oldMbr->x;
			int y=oldMbr->y;
			int h=oldMbr->getHeight();
			int w=oldMbr->getWidth();
			newMbr=new MinimalBoundingRectangle(x+this->window.x,y+this->window.y,h,w);
			
			newCentroid=(*itGar)->getCentroid();
			
			newCentroid[0]+=this->window.x;
			newCentroid[1]+=this->window.y;
			
			movedGarbage=new Garbage(newMbr,newCentroid);
			movedGarbage->area=(*itGar)->area;
			movedGarbage->perimeter=(*itGar)->perimeter;
			movedGarbage->isFocused=true;
			newGarbageList.push_back(movedGarbage);
			
			//delete old garbage & mbr
			delete (*itGar);
			delete oldMbr;
		}
		
		return newGarbageList;
	}


}

