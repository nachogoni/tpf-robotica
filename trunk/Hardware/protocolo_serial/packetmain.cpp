#include <cstdlib>
#include <iostream>
#include "packet.h"
#include "GroupPacket.h"
#include "DCMotorPacket.h"
#include "ServoMotorPacket.h"

using namespace std;

int main(int argc, char *argv[])
{
	GroupPacket * p = new GroupPacket(0x09);
	p->calculateCRC();
	p->setCommand(0x2D);
	if ( p->checkCRC() )
	    printf("CRC OK\n");
	else
		printf("CRC FAILED\n");

	p->calculateCRC();
	if ( p->checkCRC() )
	    printf("CRC OK\n");
	else
		printf("CRC FAILED\n");

	char error [] = {0x00, 'O', 'o', 'o', 'p', 's', '!', '!', '!', '!'};
	p->setError();
	p->addData(error,10);
	p->calculateCRC();
	if ( p->isError() ){
		std::string s = p->getErrorString();
		cout << s;
	}
	if ( p->checkCRC() )
	    printf("CRC OK\n");
	else
		printf("CRC FAILED\n");
		
	ServoMotorPacket * smp = new ServoMotorPacket();
	DCMotorPacket * dcm = new DCMotorPacket();

	getchar();
	
    return EXIT_SUCCESS;
}
