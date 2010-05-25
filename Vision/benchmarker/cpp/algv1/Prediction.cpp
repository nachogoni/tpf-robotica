


#include <iostream>
#include <stdio.h>
#include "Prediction.h"
#include "GarbageHistoric.h"



namespace utils{
	
	//~ Prediction::Prediction(){
		//~ 
	//~ }
	//~ Prediction::Prediction(int minAppereances,int maxLife){
		//~ 
	//~ }
	//~ 
	//~ Prediction::~Prediction(){
	//~ }
	//~ Prediction::Prediction(int minAppereances,int maxLife){
		//~ 
	//~ }
	
	
	std::list<Garbage*> Prediction::getPrediction(std::list<Garbage*> garbageList){
		
		std::list<Garbage*> newGarbageList;
				
		this->updateHistorical(garbageList);
		//update then collect
		
		//~ for (std::list<GarbageHistoric*>::iterator itHist = this->ghist.begin(); itHist != this->ghist.end(); itHist++)
		//~ {
			//~ newGarbageList.push_back((*itHist)->garbage);
		//~ }
			
		return newGarbageList;
	}
	
	void Prediction::updateHistorical(std::list<Garbage*> garbageList){
		//traverse a garbageList, for every new garbage create a new historical
			//~ printf("garbageList size:%d/n",garbageList.size());
			//~ printf("Historic size:%d\n",this->ghist.size());
			

		
			for (std::list<Garbage*>::iterator itGar = garbageList.begin(); itGar != garbageList.end(); itGar++)
			{
				//~ this->ghist2.size();
				//~ for(std::list<GarbageHistoric*>::iterator itHist = (this->ghist).begin();this->ghist.size()!=0, itHist != (this->ghist).end(); itHist++)
				//~ {
					//~ if((*itHist)->isSameObject((*itGar))){
						//~ (*itHist)->updateHistoric((*itGar));
					//~ }else{
						//~ GarbageHistoric * garbageHist=new GarbageHistoric((*itGar));
						//~ this->ghist.push_back(garbageHist);
					//~ }
				//~ }
			}
	}
}
