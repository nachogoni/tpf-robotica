#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <protocol/packets/BoardPacket.h>
#include <protocol/packets/ServoMotorPacket.h>
#include <protocol/Packet.h>

#define SERIAL_PORT "/dev/ttyUSB0"

#define MAX(a,b) (a>b?a:b)
#define PIPE_IN 0
#define PIPE_OUT 1

typedef struct {
    const char * cmd;
    void (*f)(char * data);
    const char * cmd_help;
} cmd_type;

bool quit = false, groupBC = false, fullBC = false;
int fd;
int dest_group = 0, dest_card = 0, from_group = 0, from_card = 0;
protocol::packets::ServoMotorPacket * packetToSend = NULL;
int pipes[2];

bool init();
char getFrom();
char getDest();

// Command functions
void cmd_help(char * data);
void cmd_quit(char * data);
void cmd_init(char * data);
void cmd_reset(char * data);
void cmd_ping(char * data);
void cmd_error(char * data);
void cmd_setpos(char * data);
void cmd_dest(char * data);
void cmd_from(char * data);
void cmd_groupBC(char * data);
void cmd_fullBC(char * data);

// Command list
cmd_type commands[] = {
    {"init", cmd_init, "Send init command"},
    {"reset", cmd_reset, "Send reset command"},
    {"ping", cmd_ping, "Send ping command"},
    {"error", cmd_error, "Send error command. Params: \%d for error"},
    // More commands here
//    {"dest", cmd_dest, "Set group and card id for destination. Params: \%d \%d for group and card (0 to 15)"},
//    {"from", cmd_from, "Set group and card id for origin. Params: \%d \%d for group and card (0 to 15)"},
    {"setpos", cmd_setpos, "set position"},
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

void sendAPacket(protocol::Packet * p);
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
    
    struct termios tc; // 115200 baud, 8n1, no flow control
    
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
    
	if ( pipe(pipes) == -1 ){
		printf("PIPE ERROR");
		return 1;
	}

    packetToSend = new protocol::packets::ServoMotorPacket(0x02,0x00);
    packetToSend->setOriginGroup(0);
    packetToSend->setOriginId(0);
    
    if (init() != true)
    {
        return -1;
    }

    // Set file descriptors
    FD_SET(fd,&readfd);
    FD_SET(pipes[PIPE_IN],&readfd);
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
            printf("caracter : %X", serial_buffer[0]);
			fflush(stdout);
        }

        // PIPE
        if ( FD_ISSET(pipes[PIPE_IN], &readfd) )
        {
			int c;
			int pipe_buf[256];
            // Have things in buffer! :P
            read(pipes[PIPE_IN], &c, 1);
			write(fd,&c,1);
            read(pipes[PIPE_IN], pipe_buf, c);
			printf("escribi : %d bytes en el serial\n",write(fd,pipe_buf,c)+1);
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
       
    return 0;
}


void sendAPacket(protocol::Packet * p){
	unsigned char i;
	p->calculateCRC();
	char * packet = p->getPacket();
	p->print();
	printf("escribi : %d bytes en el pipe\n",write(pipes[PIPE_OUT],packet,p->getActualLength()));
	//this->waitingForResponse.push_back(p);
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
    packetToSend = new protocol::packets::ServoMotorPacket(dest_group,dest_card);
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
    
    packetToSend->setOriginGroup(from_group);
    packetToSend->setOriginId(from_card);
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
    packetToSend->clear();
    packetToSend->setInit();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

void cmd_reset(char * data)
{
    packetToSend->clear();
    packetToSend->setReset();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

void cmd_error(char * data)
{
    packetToSend->clear();
    packetToSend->setPing();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    // TODO: armar paquete de error y mandarlo
    return;
}

void cmd_setpos(char * data)
{
	int servoid;
	int servopos;
    if (data == NULL || sscanf(data, "%d %d", &servoid, &servopos) != 2)
    {
        printf("Wrong parameters\n");
        return;
    }
    
    packetToSend->clear();
    packetToSend->setPosition(servoid,servopos);
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
    return;
}

void cmd_ping(char * data)
{
    packetToSend->clear();
    packetToSend->setPing();
    packetToSend->prepareToSend();
    sendAPacket(packetToSend);
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
