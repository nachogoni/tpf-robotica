#include <cstdlib>
#include <iostream>

#include <robotapi/real/RealRobot.h>
#include <GarbageCleaner.h>
#include <WorldInfo.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#define MAX_SPEED 100
#define TURN_SPD 60.0
#define METER_SPD 200.0

// Speed 20
#define countsToTurn (150.86672)
//#define countsToOneMeter ((7762.66 + 7763.86) / 2)

#define countsToOneMeter (1.5*598)
using namespace std;

robotapi::real::RealRobot * dwrobot;
robotapi::IDifferentialWheels * wdw;
WorldInfo * wi;

void turnLeft(double qspeed);
void turnRight(double qspeed);
void doOneMeter(double qspeed);
void doClockWise(double speed, int cant);
void doCounterClockWise(double speed, int cant);

int main(int argc, char *argv[])
{
	FILE * parametersFile = fopen("realParameters.txt","r+");
	behaviours::BehavioursParameters::Init(parametersFile);
	wi = new WorldInfo();

	dwrobot = new robotapi::real::RealRobot(wi);

  wdw = &(dwrobot->getDifferentialWheels("dw0"));
  wdw->enableEncoders(32);

/*
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
  turnLeft(100);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
  turnRight(100);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
  turnRight(100);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
  turnLeft(100);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
*/

/*
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
  doOneMeter(100);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());  
*/
/*  
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
  doCounterClockWise(20,4);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
  */


  double xcw = wdw->getPosition()->getX();
  double ycw = wdw->getPosition()->getY();
  double acw = wdw->getOrientation();
  printf("Position Before CWW: %g %g %g\n",xcw,ycw,acw);
  doCounterClockWise(0,4);
  printf("Position After CCW : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());

	


  //double currentLeftEncoderValue = wdw->getLeftEncoder();
  //double currentRightEncoderValue = wdw->getRightEncoder();
  //printf("Encoder values: %g %g \n",currentLeftEncoderValue,currentRightEncoderValue);
/*
  double xcw = wdw->getPosition()->getX();
  double ycw = wdw->getPosition()->getY();
  double acw = wdw->getOrientation();
  printf("Position Before One Meter: %g %g %g\n",xcw,ycw,acw);
  turnLeft(100);

  double currentLeftEncoderValue = wdw->getLeftEncoder();
  double currentRightEncoderValue = wdw->getRightEncoder();
  printf("Encoder values: %g %g \n",currentLeftEncoderValue,currentRightEncoderValue);
  printf("Position After One Meter: %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
*/

  wdw->setSpeed(0,0);
  dwrobot->step(32);
  //turnRight(100);
    return EXIT_SUCCESS;
}

void doClockWise(double speed, int cant){
  for ( int i = 0 ; i < cant ; i++ ){
    doOneMeter(speed);
    turnRight(speed);
    doOneMeter(speed);
    turnRight(speed);
    doOneMeter(speed);
    turnRight(speed);
    doOneMeter(speed);
    turnRight(speed);
	printf("Completed %d times\n",i+1);
  }
}

void doCounterClockWise(double speed, int cant){
  for ( int i = 0 ; i < cant ; i++ ){
    turnLeft(speed);
    doOneMeter(speed);
    turnLeft(speed);
    doOneMeter(speed);
    turnLeft(speed);
    doOneMeter(speed);
    turnLeft(speed);
	doOneMeter(speed);

	printf("Completed %d times\n",i+1);
  }
}
/*
void turnLeft(double speed){
  double currentLeftEncoderValue = wdw->getLeftEncoder();
  double currentRightEncoderValue = wdw->getRightEncoder();
  wdw->setSpeed(-speed,speed);

  while( fabs( currentLeftEncoderValue - wdw->getLeftEncoder() ) < countsToTurn
	  || fabs( currentRightEncoderValue - wdw->getRightEncoder() ) < countsToTurn ){
    
    dwrobot->step(32);
  }

  wdw->setSpeed(0,0);
  dwrobot->step(32);
}
*/
/*
void turnRight(double speedi){
  double currentLeftEncoderValue = wdw->getLeftEncoder();
  double currentRightEncoderValue = wdw->getRightEncoder();
  //wdw->setSpeed(speedi,-speedi);
  
  // Tiene que recorrer distancia entre ruedas(e-puck) * pi / 2 en distancia
  // cm = 0.00012874421156
  double distanceToDo = wi->getDistanceBetweenWheels() * wi->getDistanceBetweenWheelsFactor() * PI / 4;
  double leftWheelRadius = wi->getLeftWheelRadius() * wi->getLeftWheelFactor();
  double rightWheelRadius = wi->getRightWheelRadius() * wi->getRightWheelFactor();
  printf("%g %g\n",leftWheelRadius ,rightWheelRadius );
  double countsToDo [2] = {0,0};
  countsToDo[0] = distanceToDo * 159.23 / leftWheelRadius;
  countsToDo[1] = distanceToDo * 159.23 / rightWheelRadius;
  double countsDone [2] = {0,0};
  double speed [2] = {0,0};
  
  //printf("%g %g %g\n",countsToDo,countsDone[0],countsDone[1]);
  
  while( abs(countsDone[0]) < countsToDo[0] || abs(countsDone[1]) < countsToDo[1] ){
    countsDone[0] += (wdw->getLeftEncoder() - currentLeftEncoderValue);
    currentLeftEncoderValue = wdw->getLeftEncoder();
    countsDone[1] += (wdw->getRightEncoder() - currentRightEncoderValue);
    currentRightEncoderValue = wdw->getRightEncoder();

//    printf("%g %g %g\n",countsToDo,countsDone[0],countsDone[1]);    
    if ( abs(countsDone[0]) > countsToDo[0] )
      countsDone[0] = countsToDo[0];
    if ( abs(countsDone[1]) > countsToDo[1] )
      countsDone[1] = countsToDo[1];

    printf("%g %g - %g %g\n",countsToDo[0],countsDone[0],countsToDo[1],countsDone[1]);
    speed [0] = ( 1 - abs(countsDone[0])/countsToDo[0] ) * MAX_SPEED;
    
    if ( speed [0] > MAX_SPEED )
      speed [0] = MAX_SPEED;
  */  
//    speed [1] = -(countsToDo[1]-abs(countsDone[1]));
/*
    speed [1] = -( 1 - abs(countsDone[1])/countsToDo[1] ) * MAX_SPEED;
    
    if ( abs(speed [1]) > MAX_SPEED )
      speed [1] = -MAX_SPEED;
  */ 
/*   
    wdw->setSpeed(speed[0],speed[1]);
    dwrobot->step(32);
  }
  wdw->setSpeed(0,0);
  dwrobot->step(32);
}
*/
void doOneMeter(double qspeed){
	double speed = METER_SPD;
  double initialLeftEncoderValue = wdw->getLeftEncoder();
  double initialRightEncoderValue = wdw->getRightEncoder();
  double lastLeftEncoderValue = initialLeftEncoderValue;
  double lastRightEncoderValue = initialRightEncoderValue;
  sleep(1);
  wdw->setSpeed(0,0);
  wdw->moveWheels(countsToOneMeter,countsToOneMeter);
  wdw->setSpeed(speed,speed);
  
  //sleep(5);
	int i = 0;
	while ( i++ < 100 )
  		dwrobot->step(32);

  printf("----%g------------%g--------\n",wdw->getLeftEncoder(),wdw->getRightEncoder());
  printf("Position After One Meter: %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
  
  wdw->setSpeed(0,0);
  dwrobot->step(32);
  printf("One Meter done\n");
}

void turnLeft(double qspeed){
	double speed = TURN_SPD;
  double initialLeftEncoderValue = wdw->getLeftEncoder();
  double initialRightEncoderValue = wdw->getRightEncoder();
  double lastLeftEncoderValue = initialLeftEncoderValue;
  double lastRightEncoderValue = initialRightEncoderValue;
  sleep(1);
  wdw->setSpeed(0,0);
  wdw->moveWheels(countsToTurn,countsToTurn);
  wdw->setSpeed(-speed,speed);
  

//  sleep(5);
	int i = 0;
	while ( i++ < 100 )
  		dwrobot->step(32);
	
  printf("----%g------------%g--------\n",wdw->getLeftEncoder(),wdw->getRightEncoder());
  printf("Position After Turn: %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
  
  wdw->setSpeed(0,0);
  dwrobot->step(32);
  printf("Left turn done\n");
}

void turnRight(double qspeed){
	double speed = TURN_SPD;
  double initialLeftEncoderValue = wdw->getLeftEncoder();
  double initialRightEncoderValue = wdw->getRightEncoder();
  double lastLeftEncoderValue = initialLeftEncoderValue;
  double lastRightEncoderValue = initialRightEncoderValue;
  sleep(1);
  wdw->setSpeed(speed,-speed);
  sleep(2);
  while( fabs( initialLeftEncoderValue - lastLeftEncoderValue ) < countsToTurn
          || fabs( initialRightEncoderValue - lastRightEncoderValue ) < countsToTurn ){
    dwrobot->step(32);
  double aux = wdw->getLeftEncoder();
	while( aux == lastLeftEncoderValue ){
		usleep(1000);
  		aux = wdw->getLeftEncoder();
	}
  lastLeftEncoderValue = aux;
	
  aux = wdw->getRightEncoder();
	while( aux == lastRightEncoderValue ){
		usleep(1000);
  		aux = wdw->getRightEncoder();
	}
  lastRightEncoderValue = aux;
    printf("----%g------------%g--------\n",lastLeftEncoderValue,lastRightEncoderValue);
  }
  wdw->setSpeed(0,0);
  dwrobot->step(32);
  printf("Right turn done\n");
}

