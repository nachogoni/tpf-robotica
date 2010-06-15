
#include <stdio.h>
#include <cv.h>
#include "Result.h"
#include "Frame.h"

namespace benchmark { 
	
		Result::Result()
		{
			this->falsePositives=0;
			this->falseNegatives=0;
			this->nObjects=0;
			this->frameXml=NULL;
		}	
		Result::Result(Frame * frameXml)
		{
			this->falsePositives=0;
			this->falseNegatives=0;
			this->nObjects=frameXml->getObjects().size();
			this->frameXml=frameXml;
		}	
		Result::~Result(){
		}
		void Result::addMiss(Cobject * obj)
		{
			missList.push_back(obj);
			this->falsePositives++;
		}
		void Result::addFound(int index)
		{
			foundList.push_back(index);
		}
		int Result::detectedObjects(){
			return this->foundList.size();
		}
		bool Result::hasTestFrame(){
			return this->frameXml!=NULL;
		}
	

}

