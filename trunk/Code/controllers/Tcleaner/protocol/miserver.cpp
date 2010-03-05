#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Packet.h"

#define SERIAL_PORT "/dev/ttyUSB0"

#define MAX(a,b) (a>b?a:b)

int fd;

bool init()
{
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fd < 0)
    {
        perror("/dev/ttyUSB0");
        fprintf(stderr, "cannot connect to the serial port %s\n", SERIAL_PORT);
        return false;
    }
    
    struct termios tc; // 9600 baud, 8n1, no flow control
    
    tc.c_iflag = IGNBRK;
    tc.c_oflag = 0;
    tc.c_cflag = CS8 | CREAD | CLOCAL | CSTOPB;
    tc.c_lflag = 0;

    cfsetispeed(&tc, B115200);
    cfsetospeed(&tc, B115200);

    tcsetattr(fd, TCSANOW, &tc);
    
    return true;
}

protocol::Packet * command(int c)
{
    return NULL;
}

int printMenu(int broadcast)
{
    printf("Menu de envio de paquetes\n\n");
    printf("1 - Init\n");
    printf("2 - Reset\n");
    printf("3 - Ping\n");
    printf("4 - Error\n");
    printf("5 - ?\n");
    printf("6 - ?\n");
    printf("\nEstado actual: %s\n", (broadcast == 1?"broadcast":""));

    return 0;
}

int main( int argc, const char **argv)
{
    int maxfd = 0, select_resp = 0;
    char buf[200] = {0};
    char c = 0;
    struct timeval timeout;
    fd_set readfd, writefd;
    fd_set readfd_b, writefd_b;
    protocol::Packet * packet = NULL;
    int bc = 0;

    if (init() != true)
    {
        return -1;
    }

    // Set file descriptors
    FD_SET(fd,&readfd);
    FD_SET(0,&readfd);

    maxfd = MAX(fd,0)+1;

    readfd_b = readfd;
    writefd_b = writefd;
    
    printMenu(bc);
    while ( c != 'q' )
    {

        if ((select_resp = select(maxfd,&readfd,&writefd,NULL,NULL)) == 0)
        {
            // Select timed out!
            // TODO: retransmitir la lista de mensajes sin responder...
            printf("TIME OUT!\n");
        }

        if (errno == EINTR)
        {
            // A signal was delivered befor time_out
            errno = 0;
            continue;
        }

        // SERIAL PORT
        if ( FD_ISSET(fd,&readfd) )
        {
            // Have things in buffer! :P
            read(fd,buf,1);
            printf("caracter : %X\n",buf[0]);
        }

        // STDIN
        if ( FD_ISSET(0,&readfd) )
        {
            // Have things in buffer! :P
            c = (char) getchar();
            packet = command(c); //TODO: pasarle el buffer...
            printMenu(bc);
        }

        // Restore structures from backup
        readfd = readfd_b;
        writefd = writefd_b;
    }
       
    /*
    protocol::Packet * p = new protocol::Packet();
    p->setDestinationId(0x0F);
    p->setDestinationGroup(0x0F);
    p->setOriginId(0x00);
    p->setOriginGroup(0x00);
    p->setCommand(0x01);
    p->calculateCRC();
    char * data = p->getPacket();
    printf("escribi : %d\n",write(fd,data,p->getActualLength()));
    for(int i = 0; i < 5; i++ )
    printf("caracter : %X\n",data[i]);
    putchar('\n');
    for(int i = 0; i < 10000000; i++ )
    i= i + 1;


    while ( -1 != ( c = read(fd,buf,1) ) )
    if ( c != 0 )
    printf("caracter : %X\n",buf[0]);

    */
    return 0;
}
