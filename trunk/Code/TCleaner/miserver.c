#include <stdio.h>
#include <string.h>
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <sys/types.h>
  #include <sys/stat.h>

  int fd;

  void init()
  {
      fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
      if (fd < 0) {
          perror("/dev/ttyUSB0");
          fprintf(stderr, "cannot connect to LED lamp\n");
          return;
      }
      struct termios tc;                  // 9600 baud, 8n1, no flow control
//      tcgetattr(fd, &tc);
      tc.c_iflag = IGNBRK;
      tc.c_oflag = 0;
      tc.c_cflag = CS8 | CREAD | CLOCAL | HUPCL;
      tc.c_lflag = 0;

      cfsetispeed(&tc, B115200);
      cfsetospeed(&tc, B115200);
 
    tcsetattr(fd, TCSANOW, &tc);
      tcgetattr(fd, &tc);
      printf("%d %d %d %d\n",tc.c_iflag,tc.c_oflag,tc.c_cflag,tc.c_lflag);
  }

  int main( int        argc,                    // number of command-line options
      const char **argv)                  // each argument word is one command
  {
      init();
	
	char buf[200] = {0};
	int c;

	while ( -1 != ( c = read(fd,buf,1) ) )
		printf("caracter : %X\n",buf[0]);


	return 0;
  }

