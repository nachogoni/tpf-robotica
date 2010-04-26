#include <stdio.h>
#include "GarbageAdapter.h"
#include "alg4/GarbageRecognition.h"
#include "alg4/Garbage.h"
#include "alg4/MinimalBoundingRectangle.h"
#include "Cobject.h"
#include <list>

using namespace benchmark;
using namespace utils;


	GarbageAdapter::GarbageAdapter(){};
	GarbageAdapter::~GarbageAdapter(){};
	std::list<Cobject*> GarbageAdapter::recognizeObjects(IplImage * img)
	{
		std::list<benchmark::Cobject*> resp;
		int index=0;
		Cobject * obj;
		GarbageRecognition * gr= new GarbageRecognition();
		
		std::list<Garbage*> garbage=gr->getGarbageList(img);
		for (std::list<Garbage*>::iterator it = garbage.begin(); it != garbage.end(); it++)
		{
				MinimalBoundingRectangle * aMbr=(*it)->boundingBox();
				obj=new benchmark::Cobject( index ,aMbr->x,aMbr->y,aMbr->height,aMbr->width);
				resp.push_back(obj);
				index++;
		}
		return resp;
	}
	
	

