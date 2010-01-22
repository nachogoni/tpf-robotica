#include <cstdlib>
#include <iostream>

#include <robotapi/IWbDeviceTag.h>
#include <robotapi/IDevice.h>

#include <robotapi/Webts/WebotsDeviceTag.h>
#include <robotapi/Webts/WebotsDevice.h>
#include <robotapi/Webts/WebotsRobot.h>
#include <robotapi/Webts/WebotsDifferentialWheels.h>
#include <webots/DifferentialWheels.hpp>

using namespace std;

int main(int argc, char *argv[])
{
	webots::DifferentialWheels * wdw = new webots::DifferentialWheels();
    robotapi::webts::WebotsRobot * robot = new robotapi::webts::WebotsRobot(*wdw);
	robotapi::webts::WebotsDifferentialWheels * rwdw = (robotapi::webts::WebotsDifferentialWheels *)&robot->getDifferentialWheels("HOLA");

	cout << robot->getName();
	int i = 0;
	
	rwdw->enableEncoders(32);
	rwdw->setSpeed(63.2,30.4);
	
	while( i < 100 ){
		robot->step(32);
	}
    //robotapi::webts::WebotsDevice * device = new robotapi::webts::WebotsDevice;
    system("PAUSE");
    printf("HOLA");
    system("PAUSE");
    printf("HOLA");
    return EXIT_SUCCESS;
}
