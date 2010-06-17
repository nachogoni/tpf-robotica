#include <iostream>
#include <stdio.h>
#include "GarbageHistoric.h"
#include <math.h>
#include "PredictionParameters.h"




namespace utils{
	
	GarbageHistoric::GarbageHistoric(){
		
	 }
	GarbageHistoric::~GarbageHistoric(){
		 delete &(this->deltaPos);
		 //~ delete this->garbage;
	 }
	 
	GarbageHistoric::GarbageHistoric(Garbage * newGarbage){
		this->garbage=newGarbage;
		std::vector<int> pepe (2,PREDICTION_DELTA);
		this->deltaPos=pepe;
		this->age=0;
		this->lastAppeareance=0;
		this->appeareances=0;
		this->currentPos=newGarbage->getCentroid();
		this->state=NOT_SHOW;
		this->maxNumberOfFramesNoAppear=PREDICTION_DEFAULT_FRAMES_NOAPPEAR;
		this->focus=false;

	}
	
	GarbageHistoric::GarbageHistoric(std::vector<int> currentPos){
		std::vector<int> pepe (2,PREDICTION_DELTA);
		this->deltaPos=pepe;
		this->age=0;
		this->lastAppeareance=0;
		this->appeareances=0;
		this->currentPos=currentPos;
		this->state=NOT_SHOW;
		this->maxNumberOfFramesNoAppear=PREDICTION_DEFAULT_FRAMES_NOAPPEAR;
		this->focus=false;
	}
	
	void GarbageHistoric::updateHistoricWithGarbage(Garbage * movedGarbage){
		std::vector<int> newCentroid (movedGarbage->getCentroid());
		std::vector<int> previousCentroid (this->currentPos);
		
		
		//update stats (found)
		this->lastAppeareance=age;
		this->age++;
		this->appeareances++;
		
		//update position
		int deltax=newCentroid[0]-previousCentroid[0];
		int deltay=newCentroid[1]-previousCentroid[1];
		
		
		
		this->deltaPos[0]=deltax;
		this->deltaPos[1]=deltay;
		//~ this->currentPos[0]+=deltaPos[0];
		//~ this->currentPos[1]+=deltaPos[1];
		this->currentPos[0]=newCentroid[0];
		this->currentPos[1]=newCentroid[1];
		
		//update garbage
		delete this->garbage;
		this->garbage=movedGarbage;
		
		
	}
	void GarbageHistoric::updateHistoric(){
		this->age++;
	}
	
	bool GarbageHistoric::isSameObject(Garbage * foundGarbage){
		
		//tells if a newGarbage is similar to that found in previous frames
		std::vector<int> newCentroid (foundGarbage->getCentroid());
		std::vector<int> previousCentroid (this->currentPos);
		
		int factor=3*(this->age - this->lastAppeareance);
		
		//garbage has to be in previous centroid +- deltaPos
		int deltax=newCentroid[0]-previousCentroid[0];
		int deltay=newCentroid[1]-previousCentroid[1];
		
		int toleranceX=factor*fabs(deltaPos[0]) < PREDICTION_DELTA ? PREDICTION_DELTA : factor*fabs(deltaPos[0]);
		int toleranceY=factor*fabs(deltaPos[1]) < PREDICTION_DELTA ? PREDICTION_DELTA : factor*fabs(deltaPos[1]);
		
		if(fabs(deltax)<toleranceX && fabs(deltay)<toleranceY){
			if(compareContour(foundGarbage))
				return true;
		}
		
		return false;
	}
	bool GarbageHistoric::compareContour(Garbage * foundGarbage){
		double selfArea,selfPer,areaDiff,perDiff;
		
		//~ return true;
		
		if(foundGarbage==NULL)
			return true;
			
		selfArea=this->garbage->area;
		selfPer=this->garbage->perimeter;
		
		areaDiff=fabs(selfArea-foundGarbage->area);
		perDiff=fabs(selfPer-foundGarbage->perimeter);
		
		if(areaDiff < 0.25 * selfArea && perDiff < 0.25 * selfPer)
			return true;
		else{
			//~ printf("areaDiff %g perDiff %g\n",areaDiff,perDiff);
			return false;
		}
		
	}

	
	void GarbageHistoric::printPrediction(){
		std::vector<int> centroid (this->garbage->getCentroid());
		std::vector<int> delta (this->deltaPos);
		//~ printf("Historic age: %d , centroid (%d-%d),deltaPos (%d-%d)\n , last appereance: %d, apperances :%d \n", 
			//~ this->age, centroid[0],centroid[1],delta[0],delta[1],this->lastAppeareance,this->appeareances);	
	}
	
	Garbage * GarbageHistoric::guessPosition(){
		//assumes linear movement using last known delta/position
		std::vector<int> newCentroid (2,0);
		std::vector<int> centroid (this->garbage->getCentroid());
		std::vector<int> delta (this->deltaPos);
		int factor=(this->age - this->lastAppeareance);
		
		if(factor<=1){
			return this->garbage;
		}

		int deltax=(factor-1)*delta[0];
		int deltay=(factor-1)*delta[1];
		newCentroid[0]=centroid[0] +deltax;
		newCentroid[1]=centroid[1] +deltay;
	
		
		MinimalBoundingRectangle * oldMbr=this->garbage->boundingBox();
		int x=oldMbr->x;
		int y=oldMbr->y;
		int h=oldMbr->getHeight();
		int w=oldMbr->getWidth();
		
		MinimalBoundingRectangle * mbr= new MinimalBoundingRectangle(x+deltax,y+deltay,h,w);
		Garbage * newGarbage=new  Garbage(mbr,newCentroid);
		
		return newGarbage;
	}
	
	
	
}
