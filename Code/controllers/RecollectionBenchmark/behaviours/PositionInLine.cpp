// Class automatically generated by Dev-C++ New Class wizard

#include <behaviours/PositionInLine.h> // class's header file
#include <behaviours/GoToBaseGroup.h>

namespace behaviours{

// class constructor
PositionInLine::PositionInLine(WorldInfo * wi, robotapi::IDifferentialWheels * wheels, std::vector<robotapi::IDistanceSensor*> & fss) : AbstractBehaviour("Position In Line")
{
	this->wheels = wheels;
	this->fss = &fss;
	this->wi = wi;
}

// class destructor
PositionInLine::~PositionInLine()
{
	// insert your code here
}


void PositionInLine::sense(){
}

void PositionInLine::action(){
    double rspd = POSITIONING_BASE_SPD;
    double lspd = POSITIONING_BASE_SPD;
	double currentAngle = this->wheels->getOrientation();
	if ( this->wheels->getPosition()->getY() < 0.06 ){
		// It is the top right line
		printf("On top right line\n");
		if ( currentAngle > PI ){
			// It is above the line
			printf("Above the top right line\n");
       	  	rspd = rspd * ( 1 - currentAngle/(HALF_PI) );
	   	  	if ( this->inLine((*this->fss).at(1)->getValue()) )
	  		    lspd = lspd * POSITION_TURN_FACTOR;
			printf("Turning right\n");
		}else{
			// It is below the line
   			printf("Below the top right line\n");
		  	lspd = lspd * ( currentAngle/(HALF_PI) - 1 );
		  	if ( this->inLine((*this->fss).at(1)->getValue()) )
	  		    rspd = rspd * POSITION_TURN_FACTOR;
			printf("Turning left\n");
		}
	}else if ( this->wheels->getPosition()->getY() > 0.15 ){
		// It is the top left line
		printf("On top left line\n");
       	if ( currentAngle > PI ){
			// It is above the line
			printf("Above the top left line\n");
		  	lspd = lspd * ( currentAngle/(HALF_PI) - 1 );
		  	if ( this->inLine((*this->fss).at(1)->getValue()) )
	  		    rspd = rspd * POSITION_TURN_FACTOR;
			printf("Turning left\n");
        }else{
			// It is below the line
   			printf("Below the top left line\n");
	   	  	rspd = rspd * ( 1 - currentAngle/(HALF_PI) );
   		  	if ( this->inLine((*this->fss).at(1)->getValue()) )
	  		    lspd = lspd * POSITION_TURN_FACTOR;
			printf("Turning right\n");
		}
	}

	this->wheels->setSpeed(lspd,rspd);
}

}
