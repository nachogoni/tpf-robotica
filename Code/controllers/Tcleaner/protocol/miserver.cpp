#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
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
        fprintf(stderr, "cannot connect to LED lamp\n");
        return false;
    }
    
    struct termios tc;                  // 9600 baud, 8n1, no flow control
    
    tc.c_iflag = IGNBRK;
    tc.c_oflag = 0;
    tc.c_cflag = CS8 | CREAD | CLOCAL | HUPCL;
    tc.c_lflag = 0;

    cfsetispeed(&tc, B115200);
    cfsetospeed(&tc, B115200);

    tcsetattr(fd, TCSANOW, &tc);
    
    return true;
}

  int main( int        argc,                    // number of command-line options
      const char **argv)                  // each argument word is one command
  {
	
	char buf[200] = {0};
	char c;

	fd_set readfd, writefd;
	struct timeval timeout;

      init();
	FD_SET(fd,&readfd);
	FD_SET(0,&readfd);

	while ( c != 'q' ){

		int maxfd = MAX(fd,0)+1;
		select(maxfd,&readfd,&writefd,NULL,NULL);
		// TODO en vez de fd poner el max fd
		// TODO ver si sale por una interrupcion

		if ( FD_ISSET(fd,&readfd) ){
			// Have things in buffer! :P
			read(fd,buf,1);
			printf("caracter : %X\n",buf[0]);
		}


		if ( FD_ISSET(0,&readfd) ){
			// Have things in buffer! :P
			c = (char) getchar();
			printf("uiry : %d %c\n",write(fd,&c,1),c);
			//printf("caracter : %X\n",c);
		}


		FD_SET(fd,&readfd);
		FD_SET(0,&readfd);



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

