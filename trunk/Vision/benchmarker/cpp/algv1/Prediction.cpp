


#include <iostream>
#include <stdio.h>
#include "Prediction.h"
#include "GarbageHistoric.h"



namespace utils{
	
	
	std::list<Garbage*> Prediction::getPrediction(std::list<Garbage*> garbageList){
		std::list<GarbageHistoric*>::iterator itHist;
		std::list<GarbageHistoric*> gHistCopy (this->ghist);
		std::list<Garbage*> newGarbageList;
		Garbage * guess;
		int factor=1;
		
		//update then collect		
		this->updateHistorical(garbageList);
		
		
		for (itHist = gHistCopy.begin(); itHist != gHistCopy.end(); itHist++)
		{
			(*itHist)->printPrediction();
			
			//check for removal
			if( (*itHist)->age - (*itHist)->lastAppeareance >= (*itHist)->maxNumberOfFramesNoAppear){
				(*itHist)->state=DEAD;
				//delete (*itHist);
				this->ghist.remove(*itHist);
				
			}
			
			if((*itHist)->age >= PREDICTION_MIN_AGE){
				if((*itHist)->state==NOT_SHOW && (*itHist)->appeareances/(double) (*itHist)->age > 0.75)
					(*itHist)->state=SHOW;
			}
			
			if((*itHist)->state==SHOW){
				factor=(*itHist)->lastAppeareance - (*itHist)->age;
				if( factor!=1){
					//make a guess
					guess=(*itHist)->guessPosition();
					newGarbageList.push_back(guess);
				}else{
					newGarbageList.push_back((*itHist)->garbage);
				}
			}
			
			
		}
		
			
		return newGarbageList;
	}
	
	void Prediction::updateHistorical(std::list<Garbage*> garbageList){
		//traverse a garbageList, for every new garbage create a new historical
		GarbageHistoric * garbageHist;
		
		std::list<Garbage*>::iterator itGar;
		std::list<GarbageHistoric*>::iterator itHist;
		
		//copy garbage and historic list
		std::list<GarbageHistoric*> garHist (this->ghist);
		std::list<Garbage*> gar (garbageList);
				
		
		//foreach existing garbageHistoric	
		for(itHist = (garHist).begin(); itHist != (garHist).end(); itHist++)
		{
			bool updated=false;
			//check if it match any garbage detected in this frame
			for (itGar = gar.begin(); itGar != gar.end(); itGar++)
			{
				if((*itHist)->isSameObject((*itGar))){
					//if so, update it with new garbage
					(*itHist)->updateHistoricWithGarbage((*itGar));
					updated=true;
					break;
				}
			}
			
			if(updated){
				//erase the garbage (avoid new match w/historic)
				gar.erase(itGar);
			}else{
				//update historic as it didnt match any garbage
				(*itHist)->updateHistoric();
			}	
		}
		
		//for remaining garbages which didnt match any historical, add a new Historical
		for (std::list<Garbage*>::iterator itGar = gar.begin(); itGar != gar.end(); itGar++)
		{
			garbageHist=new GarbageHistoric((*itGar));
			this->ghist.push_back(garbageHist);			
		}
	}
	
}
