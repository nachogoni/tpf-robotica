#include <cstdlib>
#include <iostream>
#include "packet.h"

using namespace std;

int main(int argc, char *argv[])
{
	Packet * p = new Packet();
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
    system("PAUSE");
    return EXIT_SUCCESS;
}
