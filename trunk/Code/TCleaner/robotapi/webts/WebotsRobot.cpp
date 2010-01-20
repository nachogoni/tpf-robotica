#include "WebotsRobot.h"

#include <robotapi/webts/WebotsCamera.h>
#include <robotapi/webts/WebotsDistanceSensor.h>
#include <robotapi/webts/WebotsServo.h>
#include <robotapi/webts/WebotsBattery.h>
#include <robotapi/webts/WebotsDifferentialWheels.h>
#include <webots/Camera.hpp>

namespace robotapi {
namespace webts {

    std::string WebotsRobot::getName(){
        return "HOLA";
    }
    
    double WebotsRobot::getTime(){
        return 2.0;
    }

    int WebotsRobot::getMode(){
        return 0;
    }

    bool WebotsRobot::getSynchronization(){
        return true;
    }
    
    double WebotsRobot::getBasicTimeStep(){
        return 2.0;
    }
    
    ICamera & WebotsRobot::getCamera(std::string name){
//        const_cast<webots::Camera>(
        WebotsCamera * cam = new WebotsCamera(*new webots::Camera(name));
        return * cam;
    }

    IDistanceSensor & WebotsRobot::getDistanceSensor(std::string name){
        WebotsDistanceSensor * ds = new WebotsDistanceSensor(NULL);
        return * ds;
    }

    IServo & WebotsRobot::getServo(std::string name){
        WebotsServo * s = new WebotsServo(NULL);
        return * s;
    }

    IDevice & WebotsRobot::getDevice(std::string name){
        IWbDeviceTag * wdt = new WebotsDeviceTag(3);
        WebotsDevice * d = new WebotsDevice(name, *wdt);
        return * d;
    }

    IDifferentialWheels & WebotsRobot::getDifferentialWheels(std::string name){
        WebotsDifferentialWheels * df = new WebotsDifferentialWheels(NULL);
        return * df;
    }

    IBattery & WebotsRobot::getBattery(std::string name){
        WebotsBattery * b = new WebotsBattery(NULL);
        return * b;
    }

    
} /* End of namespace robotapi::webts */
} /* End of namespace robotapi */
