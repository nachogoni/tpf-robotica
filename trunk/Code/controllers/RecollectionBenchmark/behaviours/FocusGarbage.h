#ifndef behaviours_FocusGarbage_h
#define behaviours_FocusGarbage_h

#include "AbstractBehaviour.h"
#include <utils/IGarbageRecognition.h>

namespace behaviours {

class FocusGarbage : public AbstractBehaviour {

	public:

		FocusGarbage(utils::IGarbageRecognition * gr, robotapi::IDifferentialWheels * wheels );
		~FocusGarbage();

		void sense();

	    void action();

	private:
        utils::IGarbageRecognition * gr;
		robotapi::IDifferentialWheels * wheels;
		utils::Garbage * currentGarbage;
	};

} /* End of namespace behaviours */

#endif // behaviours_FocusGarbage_h
