// Constantes del protocolo
#include <protocol.h>

#define CARD_GROUP	MOTOR_DC	// Ver protocol.h
#define CARD_ID		1		// Valor entre 0 y E

// Descripcion de la placa
#define DESC		"PLACA GENERICA - 1.0" // Maximo DATA_SIZE bytes

// Si la placa es la ultima en la familia -> comentar la siguiente linea
#define RESEND_GROUP_BROADCAST

// Buffer del puerto serial
#define MAX_BUFFER_SIZE	45
#define DATA_SIZE		20

/* Modulo Generico - main.c
 * PIC16F88 - MAX232 - GENERICO
 *
 *                               PIC16F88
 *                .------------------------------------.
 *               -|RA2/AN2/CVREF/VREF           RA1/AN1|- LED
 *               -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- 
 *           LED -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
 * RST/ICD2:MCLR -|RA5/MCLR/VPP           RA6/OSC2/CLKO|- XT CLOCK pin2, 27pF to GND
 *           GND -|VSS                              VDD|- +5v
 *               -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD
 *               -|RB1/SDI/SDA  RB6/AN5/PGC/T1OSO/T1CKI|- ICD2:PGC
 *               -|RB2/SDO/RX/DT           RB5/SS/TX/CK|- 
 *      ICD2:PGM -|RB3/PGM/CCP1             RB4/SCK/SCL|- 
 *                '------------------------------------'
 *    
 */

#include <16F88.h>
#DEVICE ADC = 10
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay (clock=20000000)

#use rs232(BAUD=115200,PARITY=N,XMIT=PIN_B5,RCV=PIN_B2,BITS=8,ERRORS,TIMEOUT=1,STOP=1,UART1)
#use fast_io(A)
#use fast_io(B)

#byte porta=0x05
#byte portb=0x06

// Led
#bit led1=porta.1
#bit led2=porta.4

// MAX232
#bit tx=portb.5
#bit rx=portb.2

#define THIS_CARD		(CARD_GROUP + CARD_ID)
#define THIS_GROUP		(CARD_GROUP)

struct command_t {
	int len;
	int to;
	int from;
	int cmd;
	char data[DATA_SIZE + 1];
	int crc;
};

short reset;

char buffer[MAX_BUFFER_SIZE];
int buffer_write;
int buffer_read;
int data_length;

// Comando parseado
struct command_t cmd;
// Respuesta
struct command_t resp;

/* Examina y ejecula el comando */
void command(struct command_t * cmd);
/* Envia los datos por el pto serial */
void send(struct command_t cmd);
/* Inicializa puertos y variables */
void init();
/* Analiza el buffer, ejecuta los 
comandos y envia las respuestas */
void runProtocol();



void init()
{
	// Inicializa puertos
	set_tris_a(0b11100111);
	set_tris_b(0b11100110);
	
	// Variables de comunicacion
	buffer_write = 0;
	buffer_read = 0;
	data_length = -1;

	// Seteo el Timer1 como fuente externa y sin divisor
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
	set_timer1(0);

	// Interrupcion Rcv
	enable_interrupts(INT_RDA);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);

	reset = false;
		
	return;	
}	

void main()
{
	// Placa Generica - Implementacion del protocolo

long tmr1;
long tmr2;
	
	init();

	buffer[0] = 0x04; // Falla x crc
	buffer[1] = 0x11;
	buffer[2] = 0x00;
	buffer[3] = 0x40;
	buffer[4] = 0xAA;//55;
	buffer[5] = 0x06; // No es para mi
	buffer[6] = 0x00;
	buffer[7] = 0x62;
	buffer[8] = 0x45;
	buffer[9] = 0x6B;
	buffer[10] = 0x03;
	buffer[11] = 0x49;
	buffer[12] = 0x04;// Msg broadcast
	buffer[13] = 0xFF;
	buffer[14] = 0x00;
	buffer[15] = 0x40;
	buffer[16] = 0xBB;
	buffer_read = 12;
	data_length = 17;

/*	buffer[0] = 'A';
	buffer[1] = 'S';
	buffer[2] = 'A';
	buffer[3] = 0x48;
	buffer[4] = 0x00;
	buffer[5] = 0x00;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	buffer[8] = 0x00;
	buffer[9] = 0x00;

	buffer[40] = 0x08;// Msg con DATA partido
	buffer[41] = 0x11;
	buffer[42] = 0x00;
	buffer[43] = 0x40;
	buffer[44] = 'C';
	buffer_read = 40;
	data_length = 9;*/
	
	// FOREVER
//	while(true)
	{
		// Hace sus funciones...

		// Protocolo
		tmr1 = get_timer1();
		runProtocol();
		tmr2 = get_timer1();
		
		printf("\n\rT1: %ld - T2: %ld - DIFF: %ld\n\r", tmr1, tmr2, tmr2 - tmr1);
		
	}

	return;
}

/* Verifica que el comando sea valido y lo ejecuta */
void command(struct command_t * cmd)
{
	int crc, i, len;
		
	// Calculo del CRC
	crc = cmd->len ^ cmd->to ^ cmd->from ^ cmd->cmd;
	
	len = cmd->len - 4;

	for (i = 0; i < len; i++)
	{
		crc ^= (cmd->data)[i];
	}
	
	// CRC ok?
	if (cmd->crc != crc)
	{		
		// Creo respuesta de error
		resp.len = 0x05;
		resp.to = cmd->from;
		resp.from = THIS_CARD;
		resp.cmd = COMMON_ERROR;
		resp.data[0] = 0x00;
		resp.crc = 0x05 ^ resp.to ^ THIS_CARD ^ COMMON_ERROR ^ 0x00;
		return;
	}

	// Minimo todos setean esto
	resp.len = 0x04;
	resp.to = cmd->from & 0x77;
	resp.from = THIS_CARD;
	resp.cmd = cmd->cmd | 0x80;

	switch (cmd->cmd)
	{
		// Comandos comunes
		case COMMON_INIT: 
			// Enviar la descripcion de la placa en texto plano
			strcpy(resp.data, DESC);
			resp.len += strlen(resp.data);
		break;
		case COMMON_RESET: 
			// Enviar la descripcion de la placa en texto plano
			strcpy(resp.data, DESC);
			resp.len += strlen(resp.data);
			// Reset!
			reset = true;
		break;
		case COMMON_PING: 

		break;
 		case COMMON_ERROR:
			// Por ahora se ignora el comando
		break;
		
		// Comandos especificos

		default:
			resp.len++;
			resp.cmd = ERROR;
			resp.data[0] = 0x01; // Comando desconocido
		break;
	}	

	// Calcular el crc
	resp.crc = resp.len ^ resp.to ^ THIS_CARD ^ resp.cmd;
	len = resp.len - 4;
	for (i = 0; i < len; i++)
	{
		resp.crc ^= (resp.data)[i];
	}

	return;
}

/* Envia los datos por el pto serial */
void send(struct command_t cmd)
{
	int i, len;
	
	len = cmd.len - 4;
	putc(cmd.len);
	putc(cmd.to);
	putc(cmd.from);
	putc(cmd.cmd);
	
/*		printf("LEN:%X\n\r", cmd.len);	// DEBUG
		printf("TO:%X\n\r", cmd.to);	// DEBUG
		printf("FROM:%X\n\r", cmd.from);	// DEBUG
		printf("CMD:%X\n\r", cmd.cmd);	// DEBUG

		printf("DATA:\n");	// DEBUG
*/	for (i = 0; i < len; i++)
	{
		putc((cmd.data)[i]);
//			printf("%X ", (cmd.data)[i]);	// DEBUG
	}
	
	// Enviar el CRC
	putc(cmd.crc);
//		printf("\n\rCRC:%X\n\r\n\r", cmd.crc);	// DEBUG
	
	return;	
}	

// Interrupcion del RS232
#INT_RDA
void RS232()
{
	// Un nuevo dato...
	buffer[buffer_write++] = getc();
	data_length++;
	if (buffer_write == MAX_BUFFER_SIZE)
		buffer_write = 0;
	return;
}

void runProtocol()
{
	// Analiza el buffer
	if (data_length >= 0x04 && buffer[buffer_read] <= data_length)
	{
		data_length -= buffer[buffer_read] + 1;
	
		cmd.len = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd.to = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd.from = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd.cmd = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		// Obtiene el campo DATA
		if ((buffer_read + cmd.len - 0x04) > MAX_BUFFER_SIZE)
		{
			// DATA esta partido en el buffer ciclico
			memcpy(cmd.data, buffer + buffer_read, MAX_BUFFER_SIZE - buffer_read);
			memcpy(cmd.data + MAX_BUFFER_SIZE - buffer_read, buffer, cmd.len - 0x04 - MAX_BUFFER_SIZE + buffer_read);
		} else {
			// DATA esta continuo
			memcpy(cmd.data, buffer + buffer_read, cmd.len - 0x04);
		}
	
		buffer_read += cmd.len - 0x04;
		if (buffer_read >= MAX_BUFFER_SIZE)
			buffer_read -= MAX_BUFFER_SIZE;
	
		cmd.crc = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		// Soy el destinatario?
		if (cmd.to == THIS_CARD)
		{
			// Ejecuta el comando
			command(&cmd);
		}	
		else // Es broadcast?
			if ((cmd.to & 0xF0) == 0xF0)
		{
			// Ejecuta el comando
			command(&cmd);
			// Envia la respuesta
			send(resp);
			// Envia nuevamente el comando recibido
			resp = cmd;
		}	
		else // Es broadcast para mi grupo? 
			if ((cmd.to & THIS_GROUP) == THIS_GROUP)
		{
			// Ejecuta el comando
			command(&cmd);	
#ifdef RESEND_GROUP_BROADCAST
			// Envia la respuesta
			send(resp);
			// Envia nuevamente el comando recibido
			resp = cmd;
#endif
		}	
		else
		{
			resp = cmd;
		}
	
		// Envia la respuesta...
		send(resp);
	}
	
	// Reset del micro
	if (reset == true)
		reset_cpu();

}