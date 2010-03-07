//CCS PCM V4.023 COMPILER

#define CARD_GROUP	MOTOR_DC	// Ver protocol.h
#define CARD_ID		0		// Valor entre 0 y E

// Descripcion de la placa
#define DESC		"PLACA GENERICA - 1.0" // Maximo DATA_SIZE bytes

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

#include <../../protocolo/src/protocol.c>
/*
** Variables definidas en protocol.c

short reset; // Variable para hacer el reset
short crcOK; // Informa si el CRC del paquete parseado fue correcto

char buffer[MAX_BUFFER_SIZE];	// Buffer de recepcion de comandos
int buffer_write;				// Indice de escritura
int buffer_read;				// Indice de lectura
int data_length;				// Largo de los datos en el buffer

struct command_t command; 	// Comando parseado
struct command_t response; 	// Respuesta

** Implementar las siguientes funciones (usadas por el protocolo)

void init(); // Inicializa puertos y variables
void doCommand(struct command_t * cmd); // Examina y ejecula el comando

***/

void init()
{
	// Inicializa puertos
	set_tris_a(0b11100101);
	set_tris_b(0b11100110);

	// Variable para hacer el reset
	reset = false;

	return;	
}	

void main()
{
	// Placa Generica - Implementacion del protocolo
	init();

	// Init del protocol
	initProtocol();

	// FOREVER
	while(true)
	{
		// Hace sus funciones...

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
	
	len = cmd->len - MIN_LENGTH;

	for (i = 0; i < len; i++)
	{
		crc ^= (cmd->data)[i];
	}
	
	// CRC ok?
	if (cmd->crc != crc)
	{		
		// Creo respuesta de error
		response.len = 0x05;
		response.to = cmd->from;
		response.from = THIS_CARD;
		response.cmd = COMMON_ERROR;
		response.data[0] = 0x00;
		response.crc = 0x05 ^ response.to ^ THIS_CARD ^ COMMON_ERROR ^ 0x00;
		crcOK = false;
		return;
	}

	crcOK = true;
	
	// Minimo todos setean esto
	response.len = MIN_LENGTH;
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
			// No hace falta hacer mas nada
		break;
 		case COMMON_ERROR:
			// Por ahora se ignora el comando
		break;
		
		/* Comandos especificos */

 		case 0x40:
			/* 
			:DATO:
			-
			:RESP:
			-
			*/
		break;

		default:
			response.len++;
			response.cmd = COMMON_ERROR;
			response.data[0] = 0x01; // Comando desconocido
		break;
	}	

	// Calcular el crc
	response.crc = response.len ^ response.to ^ THIS_CARD ^ response.cmd;
	len = response.len - MIN_LENGTH;
	for (i = 0; i < len; i++)
	{
		response.crc ^= (response.data)[i];
	}

	return;
}
