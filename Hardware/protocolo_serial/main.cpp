#include <cstdlib>
#include <iostream>

#include "serial.h"

using namespace std;

int main()
{
HANDLE my=SerialInit("com1",1200);
char letter;

HANDLE *ptr;
*ptr=my;
SerialPutc(ptr,'a'); 
//letter=SerialGetc(ptr);
getch(); 

return 0;

}
