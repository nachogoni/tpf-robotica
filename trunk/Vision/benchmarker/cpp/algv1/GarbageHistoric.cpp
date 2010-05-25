#include <iostream>
#include <stdio.h>
#include "GarbageHistoric.h"



namespace utils{
	
	
	GarbageHistoric::GarbageHistoric(){
	 }
	GarbageHistoric::~GarbageHistoric(){
		 delete &(this->deltaPos);
	 }
	 
	GarbageHistoric::GarbageHistoric(Garbage * newGarbage){
		this->garbage=newGarbage;
		std::vector<int> pepe (2,0);
		this->currentPos=currentPos;
		this->deltaPos=pepe;
		this->age=0;
		this->lastAppeareance=0;
		this->appeareances=0;
		
	}
	
	GarbageHistoric::GarbageHistoric(std::vector<int> currentPos){
		std::vector<int> pepe (2,0);
		this->currentPos=currentPos;
		this->deltaPos=pepe;
		this->age=0;
		this->lastAppeareance=0;
		this->appeareances=0;
	}
	
	void GarbageHistoric::updateHistoric(Garbage * movedGarbage){
		printf("asd");
	}
	bool GarbageHistoric::isSameObject(Garbage * foundGarbage){
		return true;
	}
	
	
	
}
