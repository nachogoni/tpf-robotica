#include "Wander.h"
#include <utils/MyPoint.h>

#define TURN_FACTOR 0.15

namespace behaviours {

	Wander::Wander(WorldInfo * wi, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Wander"){
		this->wheels = wheels;
		this->wi = wi;
	}

	Wander::~Wander(){}

    void Wander::sense(){}

    void Wander::action(){
		double leftSpeed = WANDER_SPD;
		double rightSpeed = WANDER_SPD;

		utils::MyPoint * p = this->wheels->getPosition();
		double orientation = this->wheels->getOrientation();

		std::vector<utils::ArenaGridSlot *> nb = this->wi->getArenaGrid()->getNeighboursAt(p,orientation);
		if ( nb.size() != 0 ){
			utils::ArenaGridSlot * oldestSlot = this->getOldestSlot(nb);
			int dir = oldestSlot->getDir();
			if ( dir < 0 )
				rightSpeed *= (1-TURN_FACTOR);
	
			if ( dir > 0 )
				leftSpeed *= (1-TURN_FACTOR);
		}
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
