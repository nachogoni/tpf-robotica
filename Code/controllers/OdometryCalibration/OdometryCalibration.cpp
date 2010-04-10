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

// Speed 100
#define countsToTurn ((342.375 + 343.575)/2)
#define countsToOneMeter ((7762.66 + 7763.86) / 2)

using namespace std;

robotapi::IDifferentialWheels * wdw;
robotapi::webts::WebotsRobot * dwrobot;
void turnLeft(double speed);
void turnRight(double speed);
void doOneMeter(double speed);
void doClockWise(double speed, int cant);
void doCounterClockWise(double speed, int cant);

int main(int argc, char *argv[])
{

	WorldInfo * wi = new WorldInfo();

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
  
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
  doCounterClockWise(20,4);
  printf("Current Position : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
/*
  double xcw = wdw->getPosition()->getX();
  double ycw = wdw->getPosition()->getY();
  double acw = wdw->getOrientation();
  doCounterClockWise(20,4);
  printf("Position After CW: %g %g %g\n",xcw,ycw,acw);
  printf("Position After CCW : %g %g %g\n",wdw->getPosition()->getX(),wdw->getPosition()->getY(),wdw->getOrientation());
*/
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

void turnRight(double speed){
  double currentLeftEncoderValue = wdw->getLeftEncoder();
  double currentRightEncoderValue = wdw->getRightEncoder();
  wdw->setSpeed(speed,-speed);
  while( fabs( currentLeftEncoderValue - wdw->getLeftEncoder() ) < countsToTurn
          || fabs( currentRightEncoderValue - wdw->getRightEncoder() ) < countsToTurn ){
    
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

