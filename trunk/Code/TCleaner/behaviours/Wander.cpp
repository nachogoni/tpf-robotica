#include "Wander.h"
#include <sstream>

namespace behaviours {

	Wander::Wander() : AbstractBehaviour(){
		std::stringstream sstr;
		sstr << "Wander Behaviour, Id: ";
		sstr << behaviour_id;
		s = sstr.str();
	}

    void Wander::sense(){}

    std::string Wander::toString(){
		return s;
	}

    void Wander::action(){}

} /* End of namespace behaviours */
