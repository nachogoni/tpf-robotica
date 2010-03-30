//CCS PCM V4.023 COMPILER

#define CARD_GROUP	DISTANCE_SENSOR	// Ver protocol.h
#define CARD_ID		0		// Valor entre 0 y E

// Descripcion de la placa
#define DESC		"PLACA SENSORES 1.0" // Maximo DATA_SIZE bytes

/* Modulo Generico - main.c
 * PIC16F88 - MAX232 - SENSORES
 *
 *                               PIC16F88
 *                .------------------------------------.
 *       SENSE_3 -|RA2/AN2/CVREF/VREF           RA1/AN1|- SENSE_2
 *       SENSE_4 -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- SENSE_1
 *       SENSE_5 -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
 * RST/ICD2:MCLR -|RA5/MCLR/VPP           RA6/OSC2/CLKO|- XT CLOCK pin2, 27pF to GND
 *           GND -|VSS                              VDD|- +5v
 * ON/OFF SWITCH -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD/SENSOR_5
 *      SENSOR_1 -|RB1/SDI/SDA  RB6/AN5/PGC/T1OSO/T1CKI|- ICD2:PGC/SENSOR_4
 *  MAX232:R1OUT -|RB2/SDO/RX/DT           RB5/SS/TX/CK|- MAX232:T1IN
 *     	SENSOR_2 -|RB3/PGM/CCP1             RB4/SCK/SCL|- SENSOR_3
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

// MAX232
#bit tx=portb.5
#bit rx=portb.2

// Led - Comparte el pin con el switch On/Off
#bit led1=portb.0
// Pin para switch On/Off
#bit inOnOff=portb.0

// Telemetros - Base de los transistores
#bit sensor1=portb.1
#bit sensor2=portb.3
#bit sensor3=portb.4
#bit sensor4=portb.6
#bit sensor5=portb.7

// Telemetros - Salida de los telemetros (SENSE)
#bit sense1=porta.0
#bit sense2=porta.1
#bit sense3=porta.2
#bit sense4=porta.3
#bit sense5=porta.4

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
	set_tris_a(0b11111111);
	set_tris_b(0b00100101);

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
	crc = generate_8bit_crc((char *)cmd, cmd->len, CRC_PATTERN);
	
	// CRC ok?
	if (cmd->crc != crc)
	{		
		// Creo respuesta de error
		response.len = MIN_LENGTH + cmd->len + 2 + 1;
		response.to = cmd->from;
		response.from = THIS_CARD;
		response.cmd = COMMON_ERROR;
		response.data[0] = 0x00;
		// Agrego el paquete que contiene el error de CRC
		response.data[1] = cmd->len;
		response.data[2] = cmd->to;
		response.data[3] = cmd->from;
		response.data[4] = cmd->cmd;
		// Campo data
		len = cmd->len - MIN_LENGTH;
		for (i = 0; i < len; i++)
			response.data[5 + i] = (cmd->data)[i];
		// CRC erroneo
		response.data[5 + len] = cmd->crc;
		// CRC esperado
		response.data[5 + len + 1] = crc;
		// CRC de la respuesta
		response.crc = generate_8bit_crc((char *)(&response), response.len, CRC_PATTERN);
	
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

 		case DISTANCE_SENSOR_ENABLE_DISTANCE_SENSOR:
			/* Enciende el sensor de distancia indicado.
			:DATO:
			Valor de 0x00 a 0x04 que representa el id del sensor a encender.
			:RESP:
			-
			*/
		break;
 		case DISTANCE_SENSOR_DISABLE_DISTANCE_SENSOR:
			/* Apaga el sensor de distancia indicado.
			:DATO:
			Valor de 0x00 a 0x04 que representa el id del sensor a apagar.
			:RESP:
			-
			*/
		break;
 		case DISTANCE_SENSOR_SET_ALL_DISTANCE_SENSORS:
			/* Enciende o apaga cada uno de los sensores de distancia conectados al controlador.
			:DATO:
			Valor de 0x00 a 0x1F donde cada bit representa el id del sensor a encender o apagar.
			Si 2^ID = 1 entonces el sensor ID esta encendido.
			Si 2^ID = 0 entonces el sensor ID esta apagado.
			:RESP:
			-
			*/
		break;
 		case DISTANCE_SENSOR_GET_VALUE:
			/* Obtiene el valor promedio de la entrada del sensor indicado.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del sensor del que se quiere la lectura.
			:RESP:
			Valor de 0x00 a 0x04 que determina el id del sensor del que proviene el la lectura de distancia.
			Numero entero positivo de 16 bits en el rango desde 0x0000 hasta 0x03FF, con el valor de la lectura
			que representa la distancia al objeto.
			*/
		break;
 		case DISTANCE_SENSOR_GET_ALL_VALUES:
			/* Obtiene las distancias de cada uno de los sensores conectados al controlador.
			:DATO:
			-
			:RESP:
			Consta de 5 numero entero positivos de 16 bits concatenados, en el rango desde 0x0000 hasta 0x03FF,
			uno para cada uno de los sensores conectados al controlador.
			*/
		break;
 		case DISTANCE_SENSOR_GET_ONE_VALUE:
			/* Obtiene el valor de la entrada del sensor indicado.
			Igual al comando DISTANCE_SENSOR_GET_VALUE pero si es necesario enciende el sensor, toma la lectura y luego
			lo apaga para un mayor ahorro de energia.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del sensor del que se quiere la lectura.
			:RESP:
			Valor de 0x00 a 0x04 que determina el id del sensor del que proviene el la lectura de distancia.
			Numero entero positivo de 16 bits en el rango desde 0x0000 hasta 0x03FF, con el valor de la lectura
			que representa la distancia al objeto.
			*/
		break;
		case DISTANCE_SENSOR_GET_ONE_VALUE_FOR_ALL:
			/* Obtiene las distancias de cada uno de los sensores conectados al controlador.
			Igual al comando DISTANCE_SENSOR_GET_ALL_VALUES pero si es necesario enciende los sensores, toma las lecturas
			y luego los apaga para un mayor ahorro de energia.
			:DATO:
			-
			:RESP:
			Consta de 5 numero entero positivos de 16 bits concatenados, en el rango desde 0x0000 hasta 0x03FF,
			uno para cada uno de los sensores conectados al controlador.
			*/
		break;
		default:
			response.len++;
			response.cmd = COMMON_ERROR;
			response.data[0] = 0x01; // Comando desconocido
		break;
	}	

	// CRC de la respuesta
	response.crc = generate_8bit_crc((char *)(&response), response.len, CRC_PATTERN);

	return;
}