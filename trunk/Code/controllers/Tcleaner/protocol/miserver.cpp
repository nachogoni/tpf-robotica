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

typedef struct {
    const char * cmd;
    void (*f)(char * data);
    const char * cmd_help;
} cmd_type;

bool quit = false;
int fd;

bool init();

// Command functions
void cmd_help(char * data);
void cmd_quit(char * data);
void cmd_init(char * data);
void cmd_reset(char * data);
void cmd_error(char * data);

// Command list
cmd_type commands[] = {
    {"init", cmd_init, "Send init command"},
    {"reset", cmd_reset, "Send reset command"},
    {"error", cmd_error, "Send error command. Needs an error number as parameter"},
    // More commands here
    {"help", cmd_help, "This menu"},
    {"quit", cmd_quit, "Quit to system"},
    {NULL, cmd_quit}
};

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

protocol::Packet * command(char * cmd, int lenght)
{
    int i = 0;
    bool found = false;
    
    while (!found && commands[i].cmd != NULL)
    {
        if (strcmp(commands[i].cmd, cmd) == 0)
        {
            // Command found!
            commands[i].f(NULL); // TODO: Generar un packet
            found = true;
        }
        i++;
    }
    
    return NULL;
}

int main( int argc, const char **argv)
{
    int maxfd = 0, select_resp = 0;
    char stdin_buffer[256] = {0}, serial_buffer[256] = {0};
    int stdin_idx = 0;
    int c = 0;
    struct timeval timeout;
    fd_set readfd, writefd;
    fd_set readfd_b, writefd_b;
    protocol::Packet * packet = NULL;
    
    if (init() != true)
    {
        return -1;
    }

    // Set file descriptors
    FD_SET(fd,&readfd);
    FD_SET(0,&readfd);

    maxfd = MAX(fd,0) + 1;

    readfd_b = readfd;
    writefd_b = writefd;
    
    cmd_help(NULL);

    while ( quit != true )
    {

        if ((select_resp = select(maxfd, &readfd, &writefd, NULL, NULL)) == 0)
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
        if ( FD_ISSET(fd, &readfd) )
        {
            // Have things in buffer! :P
            read(fd, serial_buffer, 1);
            // TODO: hacer lo mismo que en los pics para interpretar el comando
            printf("caracter : %X\n", serial_buffer[0]);
        }

        // STDIN
        if ( FD_ISSET(0, &readfd) )
        {
            // Have things in buffer! :P
            c = read(0, stdin_buffer + stdin_idx, 1);
            
            if (stdin_buffer[stdin_idx] == '\n')
            {
                stdin_buffer[stdin_idx] = '\0';
                packet = command(stdin_buffer, stdin_idx);
                stdin_idx = 0;
            } else {
                stdin_idx++;
            }
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

void cmd_help(char * data)
{
    int i = 0;
    
    printf("Posibles comandos:\n\n");

    while (commands[i].cmd != NULL)
    {
        printf("%s\t%s\n", commands[i].cmd, commands[i].cmd_help);
        i++;
    }
    
    return;
}

void cmd_quit(char * data)
{
    quit = true;
    return;
}

void cmd_init(char * data)
{
    // TODO: armar paquete de init
    return;
}

void cmd_reset(char * data)
{
    // TODO: armar paquete de reset
    return;
}

void cmd_error(char * data)
{
    // TODO: armar paquete de error
    return;
}
