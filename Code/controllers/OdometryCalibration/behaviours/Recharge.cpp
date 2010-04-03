#include "Recharge.h"

namespace behaviours {

	Recharge::Recharge(WorldInfo * wi, robotapi::IBattery * robotBatt, robotapi::IBattery * pcBatt) : AbstractBehaviour("Recharge"){
       	this->robotBattery = robotBatt;
		this->pcBattery = pcBatt;
		this->wi = wi;
	}

	void Recharge::sense(){
	}

	void Recharge::action(){
	}

} /* End of namespace behaviours */
