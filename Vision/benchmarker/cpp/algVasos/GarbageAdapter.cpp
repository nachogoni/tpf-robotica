#include <stdio.h>
#include "GarbageAdapter.h"
#include "GarbageRecognition.h"
#include "Garbage.h"
#include "MinimalBoundingRectangle.h"
#include "Cobject.h"
#include <list>

using namespace benchmark;
using namespace utils;


	GarbageAdapter::GarbageAdapter(){
		this->gr=new GarbageRecognition();
		this->gr->disablePrediction();
		this->gr->disableWindowing();

	}
	
	GarbageAdapter::~GarbageAdapter(){};
	std::list<Cobject*> GarbageAdapter::recognizeObjects(IplImage * img)
	{
		std::list<benchmark::Cobject*> resp;
		int index=0;
		std::vector<int> centroid;
		Cobject * obj;
		
		
		std::list<Garbage*> garbage=gr->getGarbageList(img);
		for (std::list<Garbage*>::iterator it = garbage.begin(); it != garbage.end(); it++)
		{
				MinimalBoundingRectangle * aMbr=(*it)->boundingBox();
				centroid=(*it)->getCentroid();
				obj=new benchmark::Cobject( index ,aMbr->x,aMbr->y,aMbr->width,aMbr->height,centroid);
				obj->setPredicted((*it)->isPredicted);
				obj->setVisualized((*it)->isVisualized);
				obj->setFocused((*it)->isFocused);
				resp.push_back(obj);
				//~ delete aMbr;
				//~ delete (*it);
				index++;
		}
		
		
		return resp;
	}
	
	

