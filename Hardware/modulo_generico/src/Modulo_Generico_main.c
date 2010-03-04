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

<<<<<<< .mine
// PLACA DE PRUEBAS
#bit telemetroIN  	= porta.0
#bit telemetroOUT 	= portb.0
#bit servoPWM		= portb.1
#bit pisoIN		  	= porta.2
#bit pisoOUT	  	= portb.3


#define THIS_CARD		(CARD_GROUP * 16 + CARD_ID)
#define THIS_GROUP		(CARD_GROUP * 16)
=======
#define THIS_CARD		(CARD_GROUP + CARD_ID)
#define THIS_GROUP		(CARD_GROUP)
>>>>>>> .r127

struct command_t {
	int len;
	int to;
	int from;
	int cmd;
	char data[DATA_SIZE + 1];
	int crc;
};

short reset;
short crcOK;

char buffer[MAX_BUFFER_SIZE];
int buffer_write;
int buffer_read;
int data_length;

// Comando parseado
struct command_t command;
// Respuesta
struct command_t response;

/* Examina y ejecula el comando */
void doCommand(struct command_t * cmd);
/* Envia los datos por el pto serial */
void send(struct command_t * cmd);
/* Inicializa puertos y variables */
void init();
/* Analiza el buffer, ejecuta los 
comandos y envia las respuestas */
void runProtocol(struct command_t * cmd);

void init()
{
	// Inicializa puertos
	set_tris_a(0b11100101);
	set_tris_b(0b11100110);


	// ***ADC***
	setup_port_a(sAN2);//|VSS_VREF);
	setup_adc(ADC_CLOCK_INTERNAL);
	set_adc_channel(2);
	setup_adc_ports(sAN2);
	// Deberia usar VREF_A2... probar
	setup_vref(VREF_HIGH | 8); // VREF a 2.5V -> no hay cambios...

	
	// Variables de comunicacion
	buffer_write = 0;
	buffer_read = 0;
	data_length = 0;

	// Seteo el Timer1 como fuente externa y sin divisor
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
	set_timer1(0);

	// Interrupcion Rcv
	enable_interrupts(INT_RDA);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);

	reset = false;
	crcOK = false;
	
	return;	
}	

void main()
{
long adc_value[5];
int i = 0;

	// Placa Generica - Implementacion del protocolo
	init();
/*
// PLACA DE PRUEBAS
while (1)
{
	led1=1;
//	telemetroOUT=0;
	pisoOUT = 0;
	delay_ms(1000);
	led1=0;
//	telemetroOUT=1;
	pisoOUT = 1;
	delay_ms(1000);
}		
*/

while(1)
{
	for (i = 0; i < 5; i++)
	{
		// Apago
		pisoOUT = 1;
		read_adc(ADC_START_ONLY);
		delay_ms(1);
		// Leo
		adc_value[i] = read_adc(ADC_READ_ONLY);
		// Enciendo
		pisoOUT = 0;
		read_adc(ADC_START_ONLY);
		delay_ms(1);
		// Leo
		adc_value[i] = read_adc(ADC_READ_ONLY) - adc_value[i];
	}
	printf("Value: %ld %ld %ld %ld %ld ->  %ld\r\n", 
		adc_value[0], adc_value[1], adc_value[2], adc_value[3], adc_value[4],
		(adc_value[0] + adc_value[1] + adc_value[2] + adc_value[3] + adc_value[4]) );
	delay_ms(255);
}	

//while (1);

/*
	buffer[0] = 0x04; // Falla x crc
	buffer[1] = 0xFF;
	buffer[2] = 0x00;
	buffer[3] = 0x01;
	buffer[4] = 0xFA;//55;
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
	buffer_read = 0;
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
	while(true)
	{
		// Hace sus funciones...
		led1 = 0;

		// Protocolo
		runProtocol(&command);
		
	}

	return;
}

/* Verifica que el comando sea valido y lo ejecuta */
void doCommand(struct command_t * cmd)
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
<<<<<<< .mine
		response.len = 0x05;
		response.to = cmd->from;
		response.from = THIS_CARD;
		response.cmd = 0x04;
		response.data[0] = 0x00;
		response.crc = 0x05 ^ response.to ^ THIS_CARD ^ 0x04 ^ 0x00;
		crcOK = false;
=======
		resp.len = 0x05;
		resp.to = cmd->from;
		resp.from = THIS_CARD;
		resp.cmd = COMMON_ERROR;
		resp.data[0] = 0x00;
		resp.crc = 0x05 ^ resp.to ^ THIS_CARD ^ COMMON_ERROR ^ 0x00;
>>>>>>> .r127
		return;
	}

	crcOK = true;
	
	// Minimo todos setean esto
	response.len = 0x04;
	response.to = cmd->from & 0x77;
	response.from = THIS_CARD;
	response.cmd = cmd->cmd | 0x80;

	switch (cmd->cmd)
	{
		// Comandos comunes
		case COMMON_INIT: 
			init();
			// Enviar la descripcion de la placa en texto plano
			strcpy(response.data, DESC);
			response.len += strlen(response.data);
		break;
		case COMMON_RESET: 
			// Enviar la descripcion de la placa en texto plano
			strcpy(response.data, DESC);
			response.len += strlen(response.data);
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
<<<<<<< .mine
			response.len++;
			response.cmd = 0x04;
			response.data[0] = 0x01; // Comando desconocido
=======
			resp.len++;
			resp.cmd = ERROR;
			resp.data[0] = 0x01; // Comando desconocido
>>>>>>> .r127
		break;
	}	

	// Calcular el crc
	response.crc = response.len ^ response.to ^ THIS_CARD ^ response.cmd;
	len = response.len - 4;
	for (i = 0; i < len; i++)
	{
		response.crc ^= (response.data)[i];
	}

	return;
}

/* Envia los datos por el pto serial */
void send(struct command_t * cmd)
{
	int i, len;
	
	len = cmd->len - 4;
	putc(cmd->len);
	putc(cmd->to);
	putc(cmd->from);
	putc(cmd->cmd);
	
	for (i = 0; i < len; i++)
	{
		putc((cmd->data)[i]);
	}
	
	// Enviar el CRC
	putc(cmd->crc);
	
	return;	
}	

// Interrupcion del RS232
#INT_RDA
void RS232()
{
int c;
	led1 = 1;
c = getc();
putc(c);

/*
	led1 = 1;
	// Un nuevo dato...
	buffer[buffer_write++] = getc();
	data_length++;
	if (buffer_write == MAX_BUFFER_SIZE)
		buffer_write = 0;
*/	return;
}

void runProtocol(struct command_t * cmd)
{
	// Analiza el buffer
	if (data_length >= 0x04 && buffer[buffer_read] <= data_length)
	{
		data_length -= buffer[buffer_read] + 1;
	
		cmd->len = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd->to = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd->from = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		cmd->cmd = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;
	
		// Obtiene el campo DATA
		if ((buffer_read + cmd->len - 0x04) > MAX_BUFFER_SIZE)
		{
			// DATA esta partido en el buffer ciclico
			memcpy(cmd->data, buffer + buffer_read, MAX_BUFFER_SIZE - buffer_read);
			memcpy(cmd->data + MAX_BUFFER_SIZE - buffer_read, buffer, cmd->len - 0x04 - MAX_BUFFER_SIZE + buffer_read);
		} else {
			// DATA esta continuo
			memcpy(cmd->data, buffer + buffer_read, cmd->len - 0x04);
		}
	
		buffer_read += cmd->len - 0x04;
		if (buffer_read >= MAX_BUFFER_SIZE)
			buffer_read -= MAX_BUFFER_SIZE;
	
		cmd->crc = buffer[buffer_read++];
	
		if (buffer_read == MAX_BUFFER_SIZE)
			buffer_read = 0;

		// Soy el destinatario?
		if (cmd->to == THIS_CARD)
		{
			// Ejecuta el comando
			doCommand(cmd);
		} else // Es broadcast?
			if ((cmd->to & 0xF0) == 0xF0)
		{
			// Ejecuta el comando
			doCommand(cmd);

			if (crcOK == true)
			{
				// Envia la respuesta
				send(&response);
				// Envia nuevamente el comando recibido
				response = *cmd;
			}
		} else // Es broadcast para mi grupo? 
			if ((cmd->to & THIS_GROUP) == THIS_GROUP)
		{
			// Ejecuta el comando
			doCommand(cmd);	
#ifdef RESEND_GROUP_BROADCAST
			if (crcOK == true)
			{
				// Envia la respuesta
				send(&response);
				// Envia nuevamente el comando recibido
				response = *cmd;
			}
#endif
		} else {
			response = *cmd;
		}
	
		// Envia la respuesta...
		send(&response);
	}
	
	// Reset del micro
	if (reset == true)
	{
		delay_ms(10);
		reset_cpu();
	}	

}