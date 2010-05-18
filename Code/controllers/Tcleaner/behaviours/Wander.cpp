#include "Wander.h"
#include <utils/MyPoint.h>

namespace behaviours {

	Wander::Wander(WorldInfo * wi, robotapi::IDifferentialWheels * wheels) : AbstractBehaviour("Wander"){
		this->wheels = wheels;
		this->wi = wi;
	}

	Wander::~Wander(){}

    void Wander::sense(){}

    void Wander::action(){
		this->wheels->setSpeed(WANDER_SPD,WANDER_SPD);
		utils::MyPoint * p = this->wheels->getPosition();
		std::vector<utils::ArenaGridSlot *> nb = this->wi->getArenaGrid()->getNeighboursAt(p);
		int oldestSlot = this->getOldestSlot(nb);
		printf("I have to go to %d\n",oldestSlot);
	}

	int Wander::getOldestSlot(std::vector<utils::ArenaGridSlot *> nb){
		utils::ArenaGridSlot * left = nb.at(0);
		utils::ArenaGridSlot * right = nb.at(1);
		utils::ArenaGridSlot * top = nb.at(2);
		utils::ArenaGridSlot * bottom = nb.at(3);

		if ( left == NULL ){
			if ( right == NULL ){
				// RIGHT AND LEFT NULL
				if ( top == NULL ){
					// RIGHT, LEFT AND TOP NULL
					return 3;
				}else{
					if ( bottom == NULL )
					// RIGHT, LEFT AND BOTTOM NULL
						return 2;
					if ( top->getTimeStamp() < bottom->getTimeStamp() )
						return 2;
					else
						return 3;
				}
			}else{
				if ( top == NULL ){
					// TOP AND LEFT NULL
					if ( bottom == NULL )
						return 1;
					if ( right->getTimeStamp() < bottom->getTimeStamp() )
						return 1;
					else
						return 3;
				}else{
					// LEFT NULL
					if ( bottom == NULL ){
						if ( top->getTimeStamp() < right->getTimeStamp() )
							return 2;
						else
							return 1;
					}else{
						if ( right->getTimeStamp() < bottom->getTimeStamp() ){
							if ( right->getTimeStamp() < top->getTimeStamp() )
								return 1;
							else
								return 2;
						}else{
							if ( top->getTimeStamp() < bottom->getTimeStamp() )
								return 2;
							else
								return 3;
						}
					}
				}				
			}
		
	}else{
		if ( right == NULL ){
			// RIGHT NULL
			if ( top == NULL ){
				if ( bottom == NULL )
					return 0;
				if ( left->getTimeStamp() < bottom->getTimeStamp() )
					return 0;
				else
					return 3;
			}else{
				if ( bottom == NULL ){
					if ( left->getTimeStamp() < top->getTimeStamp() )
						return 0;
					else
						return 2;
				}else{
					if ( left->getTimeStamp() < bottom->getTimeStamp() ){
						if ( left->getTimeStamp() < top->getTimeStamp() )
							return 0;
						else
							return 2;
					}else{
						if ( top->getTimeStamp() < bottom->getTimeStamp() )
							return 2;
						else
							return 3;
					}
				}
			}
		}else{
			// RIGHT AND LEFT NOT NULL
			if ( top == NULL ){
				if ( bottom == NULL ){
					if ( left->getTimeStamp() < right->getTimeStamp() )
						return 0;
					else
						return 1;
				}else{
					if ( right->getTimeStamp() < bottom->getTimeStamp() ){
						if ( left->getTimeStamp() < right->getTimeStamp() )
							return 0;
						else
							return 1;
					}
					else{
						if ( left->getTimeStamp() < bottom->getTimeStamp() )
							return 0;
						else
							return 3;
					}
				}
			}else{
				if ( bottom == NULL ){
					if ( right->getTimeStamp() < top->getTimeStamp() ){
						if ( left->getTimeStamp() < right->getTimeStamp() )
							return 0;
						else
							return 1;
					}
					else{
						if ( left->getTimeStamp() < top->getTimeStamp() )
							return 0;
						else
							return 2;
					}
				}else{
					if ( left->getTimeStamp() < right->getTimeStamp() ){
						if ( left->getTimeStamp() < top->getTimeStamp() ){
							if ( left->getTimeStamp() < bottom->getTimeStamp() )
								return 0;
							else
								return 3;
						}else{
							if ( top->getTimeStamp() < bottom->getTimeStamp() )
								return 2;
							else
								return 3;
						}
					}else{
						if ( right->getTimeStamp() < top->getTimeStamp() ){
							if ( right->getTimeStamp() < bottom->getTimeStamp() )
								return 1;
							else
								return 3;
						}else{
							if ( top->getTimeStamp() < bottom->getTimeStamp() )
								return 2;
							else
								return 3;
						}
					}
				}
			}				
		}
	}

}

} /* End of namespace behaviours */
