#include <cstdlib>
#include <iostream>

#include "robotapi/IWbDeviceTag.h"
#include "robotapi/IDevice.h"
#include "robotapi/Real/RealDeviceTag.h"
#include "robotapi/Webts/WebotsDeviceTag.h"
#include "robotapi/Webts/WebotsDevice.h"

using namespace std;

int main(int argc, char *argv[])
{
    robotapi::webts::WebotsDeviceTag * robot = new robotapi::webts::WebotsDeviceTag(3);

    //robotapi::webts::WebotsDevice * device = new robotapi::webts::WebotsDevice;
    system("PAUSE");
    printf("HOLA");
    system("PAUSE");
    printf("HOLA");
    return EXIT_SUCCESS;
}
