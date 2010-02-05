#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "serial.h"

// Flow control flags

#define FC_DTRDSR 0x01
#define FC_RTSCTS 0x02
#define FC_XONXOFF 0x04

// ascii definitions

#define ASCII_BEL 0x07
#define ASCII_BS 0x08
#define ASCII_LF 0x0A
#define ASCII_CR 0x0D
#define ASCII_XON 0x11
#define ASCII_XOFF 0x13
using namespace std;

// variables used with the com port
BOOL bPortReady;
DCB dcb;
COMMTIMEOUTS CommTimeouts;
BOOL bWriteRC;
BOOL bReadRC;
DWORD iBytesWritten;
DWORD iBytesRead;

HANDLE SerialInit(char *ComPortName, int BaudRate) 
{
    HANDLE hCom;
    
    hCom = CreateFile(ComPortName, 
                      GENERIC_READ | GENERIC_WRITE,
                      0, // exclusive access
                      NULL, // no security
                      OPEN_EXISTING,
                      0, // no overlapped I/O
                      NULL); // null template 
    
    bPortReady = SetupComm(hCom, 2, 128); // set buffer sizes
        
    bPortReady = GetCommState(hCom, &dcb);
    dcb.BaudRate = BaudRate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    // dcb.Parity = EVENPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fAbortOnError = FALSE;
    
    // set XON/XOFF
    dcb.fOutX = FALSE; // XON/XOFF off for transmit
    dcb.fInX = FALSE; // XON/XOFF off for receive

    // set RTSCTS
    dcb.fOutxCtsFlow = FALSE; // turn on CTS flow control
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    //dcb.fRtsControl = RTS_CONTROL_HANDSHAKE; // 

    // set DSRDTR
    dcb.fOutxDsrFlow = FALSE; // turn on DSR flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE; // 
    // dcb.fDtrControl = DTR_CONTROL_ENABLE; // 
    // dcb.fDtrControl = DTR_CONTROL_HANDSHAKE; // 
    
    bPortReady = SetCommState(hCom, &dcb);
    
    // Communication timeouts are optional
    
    bPortReady = GetCommTimeouts (hCom, &CommTimeouts);
    
    CommTimeouts.ReadIntervalTimeout = 5000;
    CommTimeouts.ReadTotalTimeoutConstant = 5000;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
    CommTimeouts.WriteTotalTimeoutConstant = 5000;
    CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
    
    bPortReady = SetCommTimeouts (hCom, &CommTimeouts);
    
    return hCom;
}

char SerialGetc(HANDLE *hCom)
{
    char rxchar;
    BOOL bReadRC;
    static DWORD iBytesRead;
    
    bReadRC = ReadFile(*hCom, &rxchar, 1, &iBytesRead, NULL);
    
    return rxchar;
}

void SerialPutc(HANDLE *hCom, char txchar)
{
    BOOL bWriteRC;
    static DWORD iBytesWritten;
    
    bWriteRC = WriteFile(*hCom, &txchar, 1, &iBytesWritten,NULL);
    
    return;
}

int main()
{
    HANDLE my = SerialInit("COM1",1200);
    int c;
    
    HANDLE *ptr;
    *ptr = my;
    
    //printf("Esperando caracteres para enviar... pulse q para salir\n");
    /*
    while ((c = getch()) != 'q')
    {
    //  SerialPutc(ptr,'a'); 
    //  putch(SerialGetc(ptr));
        putch(c);
    }*/

    //printf("saliendo...\n");
  
    system("PAUSE");
    return EXIT_SUCCESS;
}
