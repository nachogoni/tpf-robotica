#include <stdio.h>
#include "GarbageAdapter.h"
#include "alg8/GarbageRecognition.h"
#include "alg8/Garbage.h"
#include "alg8/MinimalBoundingRectangle.h"
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
		std::vector<int> centroid;
		Cobject * obj;
		GarbageRecognition * gr= new GarbageRecognition();
		
		std::list<Garbage*> garbage=gr->getGarbageList(img);
		for (std::list<Garbage*>::iterator it = garbage.begin(); it != garbage.end(); it++)
		{
				MinimalBoundingRectangle * aMbr=(*it)->boundingBox();
				centroid=(*it)->getCentroid();
				obj=new benchmark::Cobject( index ,aMbr->x,aMbr->y,aMbr->width,aMbr->height,centroid);
				resp.push_back(obj);
				delete aMbr;
				delete (*it);
				index++;
		}
		
		delete gr;
		return resp;
	}
	
	

