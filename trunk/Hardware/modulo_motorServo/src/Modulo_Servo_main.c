//CCS PCM V4.023 COMPILER

#define CARD_GROUP	SERVO_MOTOR	// Ver protocol.h
#define CARD_ID		0			// Valor entre 0 y E

// Descripcion de la placa
#define DESC		"SERVOR CONTROL - 1.0" // Maximo DATA_SIZE bytes

/* Modulo Servo - main.c
 * PIC16F88 - MAX232 - SERVO
 *
 *                               PIC16F88
 *                .------------------------------------.
 *       MOTOR_4 -|RA2/AN2/CVREF/VREF           RA1/AN1|- MOTOR_5
 *           LED -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- 
 *           LED -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
 * RST/ICD2:MCLR -|RA5/MCLR/VPP           RA6/OSC2/CLKO|- XT CLOCK pin2, 27pF to GND
 *           GND -|VSS                              VDD|- +5v
 *       MOTOR_1 -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD
 *       MOTOR_2 -|RB1/SDI/SDA  RB6/AN5/PGC/T1OSO/T1CKI|- ICD2:PGC
 *  MAX232:R1OUT -|RB2/SDO/RX/DT           RB5/SS/TX/CK|- MAX232:T1IN
 *     ICD2:PGM/ -|RB3/PGM/CCP1             RB4/SCK/SCL|- 
 *     MOTOR_3    '------------------------------------'
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

// PWMs
#bit pwm1=portb.0
#bit pwm2=portb.1
#bit pwm3=portb.3
#bit pwm4=porta.2
#bit pwm5=porta.1

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
void doCommand(struct command_t * cmd); // Examina y ejecuta el comando

***/

// Software PWM - Minimo 1750 (~0.71ms)
#define PULSE_MIN	1750
// Tiempo maximo que puede durar un pulso - Maximo 6755(~2.71ms)
#define PULSE_MAX	6755
// Tiempo entre pulsos (~25ms -> ~22.29ms fijos de espera)
#define PWM_MAX		62500
// 1º ~ 27.8 cuentas ~ 69.4us
#define DEGREE		27.8f

// Valor que representa el ancho del pulso para cada servo
long pwm_t[5];
// Angulo de cada servo
long pos[5];
// On/Off de cada servo
short servo[5];

void init()
{
	// Inicializa puertos
	set_tris_a(0b11100001);
	set_tris_b(0b11110100);

	// Seteo el Timer1 como fuente interna
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
	set_timer1(0);

	// Variable para hacer el reset
	reset = false;

	// Valor que representa el ancho del pulso para cada servo
	pwm_t[0] = PULSE_MIN;
	pwm_t[1] = PULSE_MIN;
	pwm_t[2] = PULSE_MIN;
	pwm_t[3] = PULSE_MIN;
	pwm_t[4] = PULSE_MIN;
	
	// Angulo de cada servo
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	pos[3] = 0;
	pos[4] = 0;
	
	// On/Off de cada servo
	servo[0] = 0;
	servo[1] = 0;
	servo[2] = 0;
	servo[3] = 0;
	servo[4] = 0;
	
	// Activo los servos segun este o no habilitado
	pwm1 = servo[0];
	pwm2 = servo[1];
	pwm3 = servo[2];
	pwm4 = servo[3];
	pwm5 = servo[4];
	
	return;	
}	

void main()
{
	short check_comm = 0;
	long tmr1;
	
	// Control de servomotores
	init();

	// Init del protocol
	initProtocol();


	// FOREVER
	while(true)
	{
		// Software PWM

		// Tomo el tiempo
		tmr1 = get_timer1();
		
		// Es hora de reiniciar el pulso?
		if (tmr1 >= PWM_MAX)
		{
			// Detiene el analisis de comandos
			check_comm = 0;
			
			// Inicio del periodo
			set_timer1(0);
			// Activo los servos segun este o no habilitado
			pwm1 = servo[0];
			pwm2 = servo[1];
			pwm3 = servo[2];
			pwm4 = servo[3];
			pwm5 = servo[4];
			
		} else
		// Llego al final del pulso?
		if (tmr1 >= PULSE_MAX)
		{
			// Pone las salidas a 0
			pwm1 = 0;
			pwm2 = 0;
			pwm3 = 0;
			pwm4 = 0;
			pwm5 = 0;
			// Analiza si hay comandos para ser atendidos
			check_comm = 1;
			
		} else {
			// Tomo el tiempo
			tmr1 = get_timer1();

			// Es tiempo de desactivar el PWM?
			if (tmr1 >= pwm_t[0])
				pwm1 = 0;
			if (tmr1 >= pwm_t[1])
				pwm2 = 0;
			if (tmr1 >= pwm_t[2])
				pwm3 = 0;
			if (tmr1 >= pwm_t[3])
				pwm4 = 0;
			if (tmr1 >= pwm_t[4])
				pwm5 = 0;
		}
		
		// Protocolo
		if (check_comm == 1)
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
		
 		case SERVO_MOTOR_SET_POSITION:
			/* Determina la posicion en la que debe colocarse el 
			servo motor indicado.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del servo al
			que se le aplicara la posicion. Valor entre 0x00 y 0xB4
			que representa el rango de 0 a 180 con 1 de presicion.
			:RESP:
			-
			*/
			i = ((cmd->data)[0] & 0x07); // Servo destinatario
			if (i < 5)
			{
				servo[i] = 1;
				pos[i] = (unsigned char)((cmd->data)[1]);
				pwm_t[i] = PULSE_MIN + pos[i] * DEGREE;
			}	
		break;
 		case SERVO_MOTOR_SET_ALL_POSITIONS:
			/* Determina las posiciones en la que deben colocarse
			cada uno de los servomotores
			:DATO:
			Consta de 5 valores entre 0x00 y 0xB4 concatenados, uno
			para cada uno de los servos conectados al controlador.
			Cada valor representa el rango de 0 a 180 con 1 de presicion.
			:RESP:
			-
			*/
			for (i = 0; i < 5; i++)
			{
				servo[i] = 1;
				pos[i] = (unsigned char)((cmd->data)[i]);
				pwm_t[i] = PULSE_MIN + pos[i] * DEGREE;
			}	
		break;
 		case SERVO_MOTOR_GET_POSITION:
			/* Obtiene la ultima posicion del servomotor indicado.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del servo del que
			se requiere la posicion.
			:RESP:
			Valor de 0x00 a 0x04 que determina el id del servo del que 
			se requirio la posicion. Valor entre 0x00 y 0xB4 que representa
			el rango de 0 a 180 con 1 de presicion.
			*/
			i = ((cmd->data)[0] & 0x07); // Servo destinatario
			if (i < 5)
			{
				response.data[0] = pos[i];
				response.len++;
			}	
		break;
 		case SERVO_MOTOR_GET_ALL_POSITIONS:
			/* Obtiene las últimas posiciones de todos los servomotor
			conectados al controlador.
			:DATO:
			-
			:RESP:
			Consta de 5 valores entre 0x00 y 0xB4 concatenados, uno para 
			cada uno de los servos conectados al controlador. Cada valor 
			representa el rango de 0 a 180 con 1 de presicion.
			*/
			response.data[0] = pos[0];
			response.data[1] = pos[1];
			response.data[2] = pos[2];
			response.data[3] = pos[3];
			response.data[4] = pos[4];
			response.len += 5;
		break;
		case SERVO_MOTOR_SET_SERVO_SPEED:
			/* Determina la velocidad a la que el servomotor indicado 
			llegara a la posicion.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del servo al que 
			se le aplicara la velocidad. Valor entre 0x00 y 0xB4, 
			velocidad en grados por segundo.
			:RESP:
			-
			*/
		break;
 		case SERVO_MOTOR_SET_ALL_SPEEDS:
			/* Determina las velocidades a la que cada uno de los 
			servomotores llegara a la posicion indicada.
			:DATO:
			Consta de 5 valores entre 0x00 y 0xB4 concatenados, uno 
			para cada uno de los servos conectados al controlador. 
			Cada valor representa a la velocidad en grados por segundo.
			:RESP:
			-
			*/
		break;
 		case SERVO_MOTOR_GET_SERVO_SPEED:
			/* Obtiene la velocidad asignada al servomotor indicado.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del servo del que
			se requiere la velocidad.
			:RESP:
			Valor de 0x00 a 0x04 que determina el id del servo del que 
			se requirio la velocidad. Valor entre 0x00 y 0xB4, velocidad 
			en grados por segundo.
			*/
		break;
 		case SERVO_MOTOR_GET_ALL_SPEEDS:
			/* Obtiene las velocidades de cada uno de los servomotor 
			conectados al controlador.
			:DATO:
			-
			:RESP:
			Consta de 5 valores entre 0x00 y 0xB4 concatenados, uno para 
			cada uno de los servos conectados al controlador. Cada valor 
			representa a la velocidad en grados por segundo.
			*/
		break;
 		case SERVO_MOTOR_FREE_SERVO:
			/* Deja de aplicar fuerza sobre el servo indicado.
			:DATO:
			Valor de 0x00 a 0x04 que determina el id del servo a liberar.
			:RESP:
			-
			*/
			i = ((cmd->data)[0] & 0x07); // Servo destinatario
			if (i < 5)
			{
				servo[i] = 0;
			}
		break;
 		case SERVO_MOTOR_FREE_ALL_SERVOS:
			/* Deja de aplicar fuerza sobre cada uno de los servomotor 
			conectados al controlador.
			:DATO:
			-
			:RESP:
			-
			*/
			servo[0] = 0;
			servo[1] = 0;
			servo[2] = 0;
			servo[3] = 0;
			servo[4] = 0;
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
