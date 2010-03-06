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

bool quit = false, groupBC = false, fullBC = false;
int fd;
int dest_group = 0, dest_card = 0, from_group = 0, from_card = 0;

bool init();
char getFrom();
char getDest();

// Command functions
void cmd_help(char * data);
void cmd_quit(char * data);
void cmd_init(char * data);
void cmd_reset(char * data);
void cmd_error(char * data);
void cmd_dest(char * data);
void cmd_from(char * data);
void cmd_groupBC(char * data);
void cmd_fullBC(char * data);

// Command list
cmd_type commands[] = {
    {"init", cmd_init, "Send init command"},
    {"reset", cmd_reset, "Send reset command"},
    {"error", cmd_error, "Send error command. Params: \%d for error"},
    // More commands here
    {"dest", cmd_dest, "Set group and card id for destination. Params: \%d \%d for group and card (0 to 15)"},
    {"from", cmd_from, "Set group and card id for origin. Params: \%d \%d for group and card (0 to 15)"},
    {"groupBC", cmd_groupBC, "Change group broadcast state"},
    {"fullBC", cmd_fullBC, "Change full broadcast state"},
    {"help", cmd_help, "This help"},
    {"quit", cmd_quit, "Quit to system"},
    {NULL, cmd_quit}
};

char getFrom()
{
    return (from_group & 0x0F) * 16 + (from_card & 0x0F);
}

char getDest()
{
    int group = dest_group & 0x0F;
    int card = dest_card & 0x0F;
    
    if (fullBC == true)
    {
        group = 0x0F;
        card = 0x0F;
    } else if (groupBC == true)
    {
        card = 0x0F;
    } 
    
    return group * 16 + card;
}

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
    char * data = NULL;
    
    // Get parameters
    if ((data = strstr(cmd, " ")) != NULL)
    {
        data[0] = '\0';
        data++;
    }
    
    while (found == false && commands[i].cmd != NULL)
    {
        if (strcmp(commands[i].cmd, cmd) == 0)
        {
            // Command found!
            commands[i].f(data);
            found = true;
        }
        i++;
    }
    
    if (found == false)
        printf("Command not found\n");
    
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
    
    printf("\nPossible commands:\n\n");

    while (commands[i].cmd != NULL)
    {
        printf("%s\t\t%s\n", commands[i].cmd, commands[i].cmd_help);
        i++;
    }
    
    return;
}

void cmd_dest(char * data)
{
    if (data == NULL || sscanf(data, "%d %d", &dest_group, &dest_card) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
        
    printf("GroupID: %d CardID: %d -> (%X)\n", dest_group, dest_card, getDest());
    
    return;
}

void cmd_from(char * data)
{
    if (data == NULL || sscanf(data, "%d %d", &from_group, &from_card) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    
    printf("GroupID: %d CardID: %d -> (%X)\n", from_group, from_card, getFrom());
    
    return;
}

void cmd_quit(char * data)
{
    quit = true;
    return;
}

void cmd_init(char * data)
{
    // TODO: armar paquete de init y mandarlo
    return;
}

void cmd_reset(char * data)
{
    // TODO: armar paquete de reset y mandarlo
    return;
}

void cmd_error(char * data)
{
    // TODO: armar paquete de error y mandarlo
    return;
}

void cmd_groupBC(char * data)
{
    groupBC = !groupBC;
    printf("Group broadcast state: %s\n", (groupBC?"Activated":"Deactivated"));
    return;
}

void cmd_fullBC(char * data)
{
    fullBC = !fullBC;
    printf("Full broadcast state: %s\n", (fullBC?"Activated":"Deactivated"));
    return;
}
