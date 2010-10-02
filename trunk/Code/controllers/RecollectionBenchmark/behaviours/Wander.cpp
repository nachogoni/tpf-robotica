#include "Wander.h"
#include <utils/MyPoint.h>

namespace behaviours {

	Wander::Wander(WorldInfo * wi, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Wander"){
		this->wheels = wheels;
		this->wi = wi;
		this->isComingFromBase = false;
		this->leftWasLast = true;
		this->turnFactor = behaviours::BehavioursParameters::getParameter(WANDER_TURN_FACTOR);
		this->steps = 0;
	}

	Wander::~Wander(){}

    void Wander::sense(){}

	void Wander::comingFromBase(){
		this->isComingFromBase = true;
	}

    void Wander::action(){
		double leftSpeed = behaviours::BehavioursParameters::getParameter(WANDER_SPD);
		double rightSpeed = behaviours::BehavioursParameters::getParameter(WANDER_SPD);
		if ( this->isComingFromBase ){
			if ( this->steps == 0 )
				this->leftWasLast = !this->leftWasLast;
			
			this->steps++;
			if ( !leftWasLast ){
				// Turn right
				rightSpeed *= this->turnFactor;
			}else{
				// Turn left
				leftSpeed *= this->turnFactor;
			}
			
			if ( this->steps != 0 && (this->steps % (int)behaviours::BehavioursParameters::getParameter(WANDER_STEPS_BASE) == 0) ){
				this->steps = 0;
				this->isComingFromBase = false;
			}
		}

		utils::MyPoint * p = this->wheels->getPosition();
		double orientation = this->wheels->getOrientation();
		/*
		std::vector<utils::ArenaGridSlot *> nb = this->wi->getArenaGrid()->getNeighboursAt(p,orientation);
		if ( nb.size() != 0 ){
			utils::ArenaGridSlot * oldestSlot = this->getOldestSlot(nb);
			int dir = oldestSlot->getDir();
			if ( dir < 0 )
				rightSpeed *= (1-behaviours::BehavioursParameters::getParameter(WANDER_TURN_FACTOR));
	
			if ( dir > 0 )
				leftSpeed *= (1-behaviours::BehavioursParameters::getParameter(WANDER_TURN_FACTOR));
		}
		*/
		printf("left speed %g, right speed %g\n",leftSpeed,rightSpeed);
		this->wheels->setSpeed(leftSpeed,rightSpeed);
	}

	utils::ArenaGridSlot * Wander::getOldestSlot(std::vector<utils::ArenaGridSlot *> nb){
		unsigned int i = 1;
		utils::ArenaGridSlot * oldestSlot = nb.at(0);
		time_t oldestTimeStamp = oldestSlot->getTimeStamp();
		while( i < nb.size() ){
			time_t thisTimeStamp = (nb.at(i))->getTimeStamp();
			if ( thisTimeStamp < oldestTimeStamp ){
				oldestSlot = (nb.at(i));
				oldestTimeStamp = thisTimeStamp;
			}
			i++;
		}
		return oldestSlot;
	}

} /* End of namespace behaviours */
