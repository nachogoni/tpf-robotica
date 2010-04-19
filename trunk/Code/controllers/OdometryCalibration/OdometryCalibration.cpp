#include <cstdlib>
#include <iostream>

#include <robotapi/IWbDeviceTag.h>
#include <robotapi/IDevice.h>

#include <robotapi/webts/WebotsRobot.h>
#include <robotapi/webts/WebotsDifferentialWheels.h>
#include <webots/DifferentialWheels.hpp>
#include <robotapi/real/RealRobot.h>
#include <GarbageCleaner.h>
#include <WorldInfo.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#define MAX_SPEED 100

// Speed 20
#define countsToTurn ((342.375 + 343.575)/2)
//#define countsToOneMeter ((7762.66 + 7763.86) / 2)

#define countsToOneMeter (7800)
using namespace std;

robotapi::IDifferentialWheels * wdw;
robotapi::webts::WebotsRobot * dwrobot;
WorldInfo * wi;

void turnLeft(double speed);
void turnRight(double speed);
void doOneMeter(double speed);
void doClockWise(double speed, int cant);
void doCounterClockWise(double speed, int cant);

int main(int argc, char *argv[])
{

	wi = new WorldInfo();

	webots::DifferentialWheels * edw = new webots::DifferentialWheels();
  dwrobot = new robotapi::webts::WebotsRobot(wi, *edw);
//   	GarbageCleaner::GarbageCleaner * gc = new GarbageCleaner::GarbageCleaner(wi, *robot);
//	gc->cleanGarbage();

/*
	wdw->setSpeed(100,100);
  printf("%g - %g\n",wdw->getLeftEncoder(),wdw->getRightEncoder());
	while ( 1 ){
    printf("%g - %g\n",wdw->getLeftEncoder(),wdw->getRightEncoder());
		robot->step(32);
  }
*/

  wdw = &(dwrobot->getDifferentialWheels(""));
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

/*
  double xcw = wdw->getPosition()->getX();
  double ycw = wdw->getPosition()->getY();
  double acw = wdw->getOrientation();
  doCounterClockWise(20,4);
  printf("Position After CW: %g %g %g\n",xcw,ycw,acw);
  printf("Position After CCW : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
*/

/*
  double currentLeftEncoderValue = wdw->getLeftEncoder();
  double currentRightEncoderValue = wdw->getRightEncoder();
  printf("Encoder values: %g %g \n",currentLeftEncoderValue,currentRightEncoderValue);
  doOneMeter(1);
  currentLeftEncoderValue = wdw->getLeftEncoder();
  currentRightEncoderValue = wdw->getRightEncoder();
  printf("Encoder values: %g %g \n",currentLeftEncoderValue,currentRightEncoderValue);
*/

  turnRight(100);
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
  }
}

void doCounterClockWise(double speed, int cant){
  for ( int i = 0 ; i < cant ; i++ ){
    doOneMeter(speed);
    turnLeft(speed);
    doOneMeter(speed);
    turnLeft(speed);
    doOneMeter(speed);
    turnLeft(speed);
    doOneMeter(speed);
    turnLeft(speed);
  }
}

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
/*    
    if ( speed [0] > MAX_SPEED )
      speed [0] = MAX_SPEED;
  */  
//    speed [1] = -(countsToDo[1]-abs(countsDone[1]));
    speed [1] = -( 1 - abs(countsDone[1])/countsToDo[1] ) * MAX_SPEED;
/*    
    if ( abs(speed [1]) > MAX_SPEED )
      speed [1] = -MAX_SPEED;
  */    
    wdw->setSpeed(speed[0],speed[1]);
    dwrobot->step(32);
  }
  wdw->setSpeed(0,0);
  dwrobot->step(32);
}

void doOneMeter(double speed){
  double currentLeftEncoderValue = wdw->getLeftEncoder();
  double currentRightEncoderValue = wdw->getRightEncoder();
  wdw->setSpeed(speed,speed);
  while( fabs( currentLeftEncoderValue - wdw->getLeftEncoder() ) < countsToOneMeter
          || fabs( currentRightEncoderValue - wdw->getRightEncoder() ) < countsToOneMeter ){
    
    dwrobot->step(32);
  }
  wdw->setSpeed(0,0);
  dwrobot->step(32);
}

