//CCS PCM V4.023 COMPILER

#define CARD_GROUP	MOTOR_DC	// Ver protocol.h
#define CARD_ID		0		// Valor entre 0 y E
// Motor derecho (ID: 0) y Motor izquierdo (ID: 1)

// Descripcion de la placa
#define DESC		"CONTROL MOTOR DC 1.0" // Maximo DATA_SIZE bytes

/* Modulo Motor - main.c
 * PIC16F88 - MAX232 - L298 - MR-2-60-FA
 *
 *                               PIC16F88
 *                .------------------------------------.
 *          VREF -|RA2/AN2/CVREF/VREF           RA1/AN1|- LED
 *          VREF -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- L298:SEN
 *           LED -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
 * RST/ICD2:MCLR -|RA5/MCLR/VPP           RA6/OSC2/CLKO|- XT CLOCK pin2, 27pF to GND
 *           GND -|VSS                              VDD|- +5v
 *   L298:ENABLE -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD
 *     MOTOR:IDX -|RB1/SDI/SDA  RB6/AN5/PGC/T1OSO/T1CKI|- ICD2:PGC/MOTOR:CHA_A/B
 *  MAX232:R1OUT -|RB2/SDO/RX/DT           RB5/SS/TX/CK|- MAX232:T1IN
 * L298:INPUT_B/ -|RB3/PGM/CCP1             RB4/SCK/SCL|- L298:INPUT_A
 *     ICD2:PGM   '------------------------------------'
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

// Led
#bit led1=porta.1 // Amarillo
#bit led2=porta.4 // Rojo

// L298
#bit inputA=portb.4
#bit inputB=portb.3

#include <../../protocolo/src/protocol.c>
/*
** Variables definidas en protocol.c

short reset; // Variable para hacer el reset
short crcOK; // Informa si el CRC del paquete parseado fue correcto
short sendResponse; // Informa que no debe mandarse la respuesta automatica

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

#define MAX_CONSUMPTION	150
#define MAX_CONSUMPTION_COUNT	5
#define MAX_SAFE_CONSUMPTION_COUNT 5

// Correcion de la cantidad de cuentas por segundo en base al periodo del TMR0
#define INTERVAL_CORRECTION	5

// Girar -> clockwise or unclockwise
#define CLOCKWISE	1
#define UNCLOCKWISE	-1

// Intercambiar entre el motor derecho y el izquierdo
// Motor derecho (ID: 0) y Motor izquierdo (ID: 1)
#if CARD_ID == 0
	#define FORWARD		CLOCKWISE
	#define BACKWARD	UNCLOCKWISE
#else
	#define FORWARD		UNCLOCKWISE
	#define BACKWARD	CLOCKWISE
#endif

// Sentido de giro del motor
signed int turn;
// Cantidad de overflows del TMR0
long tmr0_ticks;
// Valor acumulado del ADC - Consumo aprox
long adc_value;
// Ultimo valor del consumo
long last_consumption;
// Valor de duty del PWM
signed long duty;
// Cantidad de cuentas del encoder medidas por intervalo
signed long counts_real;
signed long last_real_speed;
// Cantidad de cuentas del encoder esperadas por intervalo
signed long counts_expected;
// Cantidad de cuentas del encoder historicas (32 bits)
signed int32 counts_total;
// Cantidad de cuentas del encoder restantes para deterner el motor
signed long counts_to_stop;
// Cantidad de cuentas del encoder desde el ultimo intevalo
signed long last_counts;
signed long last_counts2;
// Tengo una cantidad de cuentas para hacer?
short counts_check;
// Corrijo el PWM segun lo esperado?
short correct_duty;
// Indica que hay que enviar una alarma de consumo
short consumption_alarm;
// Cuenta cuantas alarmas se enviaron
int alarm_count;
// Cuenta cuantos frames de buen consumo hubo
int safeConsumption;
// Indica que hay que avisar que el motor se detuvo
short shutdown_alarm;
// Indica si se apagaron los motores por el alto consumo
short motor_shutdown;
// Usado como flag para indicar que hubo una interrupcion en el TMR0
short interrupted;

// Variables temporales
signed int32 * tmp32;
signed long * tmp16;

/* Setea el PWM */
void SetPWM(signed long pwm);

// Interrupcion del Timer0
#INT_RTCC
void Timer0_INT()
{
	// Levanto el flag de interrupcion
	interrupted = 1;
	
	// Seteo el valor para que interrumpa cada 6.25ms
	set_timer0(12);
	
	// Comienza la lectura del ADC
	//read_adc(ADC_START_ONLY);
	
	// Hago un delay
	//delay_us(20);
	
	// Tomo la muestra para el ADC
	//adc_value += read_adc(ADC_READ_ONLY);
	
	// Tomo el valor del TMR1 -> encoder
	//counts_real = get_timer1();
	
	return;
}

void init()
{
	// Inicializa puertos
	set_tris_a(0b11101101);
	set_tris_b(0b11100110);

	// ***ADC***
	setup_port_a(sAN0);//|VSS_VREF);
	setup_adc(ADC_CLOCK_INTERNAL);
	set_adc_channel(0);
	setup_adc_ports(sAN0);
	// Deberia usar VREF_A2... probar
	setup_vref(VREF_HIGH | 8); // VREF a 2.5V -> no hay cambios...
	
	// ***PWM***
	setup_ccp1(CCP_PWM);
	// Seteo al PWM con f: 4.88 kHz, duty = 0
	set_pwm1_duty(0);
	setup_timer_2(T2_DIV_BY_4, 255, 1);

	// ***TIMER1 - ENCODER COUNTER***
	// Seteo el Timer1 como fuente externa y sin divisor
	setup_timer_1(T1_EXTERNAL | T1_DIV_BY_1);
	set_timer1(0);

	// ***TIMER0 - TIME BASE***
	// Seteo el Timer0 como clock -> dt = 6.25ms
	setup_counters(RTCC_INTERNAL, RTCC_DIV_128);
	set_timer0(12);
	// Interrupcion sobre el Timer0
	enable_interrupts(INT_RTCC);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);
	
	// Variable para hacer el reset
	reset = false;

	// Sentido de giro del motor
	turn = FORWARD;

	// Cantidad de overflows del TMR0
	tmr0_ticks = 0;
	// Valor acumulado del ADC - Consumo aprox
	adc_value = 0;
	last_consumption = 0;
	// Valor de duty del PWM
	duty = 0;
	// Cantidad de cuentas del encoder esperadas por intervalo
	counts_expected = 0;
	// Cantidad de cuentas del encoder totales
	counts_total = 0;
	// Velocidad en cuentas por cada frame
	last_real_speed = 0;
	// Cantidad de cuentas del encoder restantes para deterner el motor
	counts_to_stop = 0;
	// Cantidad de cuentas del encoder desde el ultimo intevalo
	last_counts = 0;
	last_counts2 = 0;
	// Tengo una cantidad de cuentas para hacer?
	counts_check = 0;
	// Corrijo el PWM segun lo esperado?
	correct_duty = 1;
	// Indica que hay que enviar una alarma de consumo
	consumption_alarm = 0;
	// Indica cuantos frames de buen consumo hubo
	safeConsumption = 0;
	// Cuenta cuantas alarmas se enviaron
	alarm_count = 0;
	// Indica que hay que enviar una alarma de consumo
	shutdown_alarm = 0;
	// Indica si se apagaron los motores por el alto consumo
	motor_shutdown = 0;
	// Usado como flag para indicar que hubo una interrupcion en el TMR0
	interrupted = 0;

	led2 = 0;
	return;	
}	

/* Setea el duty del PWM segun el valor. Positivo o negativo determina el sentido */
void setPWM(signed long pwm)
{
	if (pwm < 0)
	{
		inputA = 0;
		inputB = 1;
	} else {
		inputA = 1;
		inputB = 0;
	}
	
	if (motor_shutdown == 1)
	{
		pwm = 0;
	} else {
		pwm = (abs(pwm));
		
		if (pwm > 1023L)
			pwm = 1023;
	}

	set_pwm1_duty(pwm);
	
	return;
}

void main()
{
	// Placa Generica - Implementacion del protocolo
	init();

	// Init del protocol
	initProtocol();

//counts_expected = 30;

	// FOREVER
	while(true)
	{
		// Hace sus funciones -> Ex-Interrupcion
		if (interrupted == 1)
		{
			interrupted = 0;
			
			// Seteo el valor para que interrumpa cada 6.25ms
			set_timer0(12);
			
			// Comienza la lectura del ADC
			read_adc(ADC_START_ONLY);
			
			// Agrego al historico de cuentas el ultimo acumulado
			counts_real = get_timer1();
			counts_total += (counts_real - last_counts2) * turn * FORWARD;
			last_counts2 = counts_real;
			
			// Tengo una cantidad de cuentas para hacer?
			if (counts_check == 1)
			{
				// Verifico si pasaron las cuentas que se habian pedido
				if (counts_to_stop < 1)
				{
					// Detengo el motor
					counts_check = 0;
					counts_to_stop = 0;
					counts_expected = 0;
					duty = 0;
					SetPWM(duty);
					correct_duty = 0;
					last_counts = 0;
				} else {
					counts_real = get_timer1();
					counts_to_stop -= (counts_real - last_counts);
					correct_duty = 1;
					last_counts = counts_real;
				}
			} else {
				correct_duty = 1;
			}
			
			// Tomo la muestra
			adc_value += read_adc(ADC_READ_ONLY);
			
			if (++tmr0_ticks == 32)
			{
				// Entra cada 200ms
				led1++;
		
				// Obtengo la cantidad de cuentas desde la ultima entrada
				counts_real = get_timer1();
				last_real_speed = counts_real;
				
				set_timer1(0);
				last_counts = 0;
				last_counts2 = 0;
				// Promedio el consumo segun la cantidad de tmr0_ticks
				last_consumption = adc_value / tmr0_ticks;
				
				if (last_consumption >= MAX_CONSUMPTION)
				{
					consumption_alarm = 1;
					safeConsumption = 0;
					if (alarm_count++ == MAX_CONSUMPTION_COUNT)
					{
						motor_shutdown = 1;
						alarm_count = 0;
						consumption_alarm = 0;
						shutdown_alarm = 1;
					}	
				} else {
					// Consumo normal -> espero para resetear el contador de shutdown
					if (safeConsumption++ == MAX_SAFE_CONSUMPTION_COUNT)
					{
						alarm_count = 0;
					}
				}	
				
				tmr0_ticks = 0;
		
				// Mantengo el consumo promedio desde que arranque y borro el temporal
				adc_value = 0;
		
				// Corrijo el PWM segun lo esperado
				if ((correct_duty == 1) && (counts_real != counts_expected))
				{
					duty += (counts_expected - counts_real) * 5;
					if (duty > 1023L)
						duty = 1023;
					else if (duty < 0)
						duty = 0;
					SetPWM(duty * turn);
				} else if ((counts_expected == 0) && (duty != 0)) {		
					SetPWM(duty = 0);
				}	
			}
		}
	
		// Enviar alarma de alto consumo
		if (consumption_alarm == 1)
		{
			/* Indica al controlador principal que hay un consumo extremo en el motor,
			posiblemente un atasco del motor o de la rueda.
			:DATO:
			Numero entero positivo de 16 bits en el rango desde 0x0000 hasta
			0x03FF, que representa el consumo ante el que sono la alarma.
			:RESP:
			-
			*/
			command.len = MIN_LENGTH + 2;
			command.to = MAIN_CONTROLLER;
			command.from = THIS_CARD;
			command.cmd = DC_MOTOR_MOTOR_STRESS_ALARM;
			// A la posicion 0 dentro de command.data la tomo como signed long *
			tmp16 = (command.data);
			// Le asigno el valor del ultimo consumo del motor
			(*tmp16) = last_consumption;
			command.crc = generate_8bit_crc((char *)(&command), command.len, CRC_PATTERN);
			consumption_alarm = 0;
			// Envio del comando
			send(&command);
		}
		
		// Enviar aviso de motor apagado
		if (shutdown_alarm == 1)
		{
			/* Indica al controlador principal que el motor ha sido apagado debido al alto
			consumo. Enviado luego de sucesivos avisos del comando DC_MOTOR_MOTOR_STRESS_ALARM.
			:DATO:
			Numero entero positivo de 16 bits en el rango desde 0x0000 hasta
			0x03FF, que representa el consumo ante el que sono la alarma.
			:RESP:
			-
			*/
			command.len = MIN_LENGTH + 2;
			command.to = MAIN_CONTROLLER;
			command.from = THIS_CARD;
			command.cmd = DC_MOTOR_MOTOR_SHUT_DOWN_ALARM;
			// A la posicion 0 dentro de response->data la tomo como signed long *
			tmp16 = (command.data);
			// Le asigno el valor del ultimo consumo del motor
			(*tmp16) = last_consumption;
			command.crc = generate_8bit_crc((char *)(&command), command.len, CRC_PATTERN);
			shutdown_alarm = 0;
			led2 = 1;
			// Envio del comando
			send(&command);
		}
			
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

	led2 = 0;

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

 		case DC_MOTOR_SET_DIRECTION:
			/* Seteo del sentido de giro del motor
			:DATO:
			0x00 para sentido horario o 0x01 para sentido anti-horario.
			:RESP:
			-
			*/
			if (((cmd->data)[0] & 0x01) == 0)
			{
				turn = FORWARD;
			} else {
				turn = BACKWARD;
			}
		break;
 		case DC_MOTOR_SET_DC_SPEED:
			/* Seteo de la velocidad del motor en cuentas del encoder por segundo
			:DATO:
			0x00 para sentido horario o 0x01 para sentido anti-horario. Numero 
			entero de 16 bits con signo, que representa la velocidad en cuentas 
			por segundos.
			:RESP:
			-
			*/
			if (((cmd->data)[0] & 0x01) == 0)
			{
				turn = FORWARD;
			} else {
				turn = BACKWARD;
			}
			// A la posicion 1 dentro de cmd->data la tomo como signed long *
			tmp16 = (cmd->data) + 1;
			// Le asigno el valor de la velocidad ajustada a 1 segundo
			counts_expected = (*tmp16) / INTERVAL_CORRECTION;
			// Habilita el motor
			motor_shutdown = 0;
		break;
 		case DC_MOTOR_SET_ENCODER:
			/* Seteo de la cantidad de cuentas historicas del encoder
			:DATO:
			Numero entero de 32 bits con signo, con el valor para setear en el
			historico del encoder.
			:RESP:
			-
			*/
			// A la posicion 0 dentro de cmd->data la tomo como signed int32 *
			tmp32 = (cmd->data);
			// Le asigno el valor de las cuentas historicas
			counts_total = (*tmp32);
		break;
 		case DC_MOTOR_GET_ENCODER:
			/* Obtener la cantidad de cuentas historicas del encoder
			:DATO:
			-
			:RESP:
			Numero entero de 32 bits con signo, que representa el valor historico
			del encoder.
			*/
			// A la posicion 0 dentro de response->data la tomo como signed int32 *
			tmp32 = (response.data);
			// Le asigno el valor de las cuentas historicas
			(*tmp32) = counts_total;
			// Corrijo el largo del paquete
			response.len += 4;
		break;
 		case DC_MOTOR_RESET_ENCODER:
			/* Resetear las cuentas historicas a cero
			:DATO:
			-
			:RESP:
			-
			*/
			counts_total = 0;
		break;
 		case DC_MOTOR_SET_ENCODER_TO_STOP:
			/* Seteo de cuantas cuentas debe girar hasta detenerse
			:DATO:
			Numero entero de 16 bits con signo, que representa la cantidad de
			cuentas del encoder restantes para que el motor se detenga.
			:RESP:
			-
			*/
			// A la posicion 0 dentro de cmd->data la tomo como signed long *
			tmp16 = (cmd->data);
			// Le asigno el valor de la velocidad ajustada a 1 segundo
			counts_to_stop = (*tmp16);
			// Habilito el chequeo de cuentas para detener el motor
			counts_check = 1;
		break;
 		case DC_MOTOR_GET_ENCODER_TO_STOP:
			/* Obtener la cantidad de las cuentas restantes que quedan por 
			realizar hasta detenerse.
			:DATO:
			-
			:RESP:
			Numero entero de 16 bits con signo, que representa la cantidad
			de cuentas del encoder restantes para detener el motor.
			*/
			// A la posicion 0 dentro de response->data la tomo como signed long *
			tmp16 = (response.data);
			// Le asigno el valor de la velocidad ajustada a 1 segundo
			(*tmp16) = counts_to_stop;
			// Corrijo el largo del paquete
			response.len += 2;
		break;
 		case DC_MOTOR_DONT_STOP:
			/* Deshace los comandos DC_MOTOR_DONT_STOP y DC_MOTOR_GET_ENCODER_TO_STOP, 
			deshabilita el conteo de cuentas para frenar y sigue en el estado actual.
			:DATO:
			-
			:RESP:
			-
			*/
			counts_check = 0;
		break;
 		case DC_MOTOR_MOTOR_CONSUMPTION:
			/* Numero entero positivo de 16 bits en el rango desde 0x0000 hasta
			0x03FF, que representa el consumo promedio del ultimo segundo.
			:DATO:
			-
			:RESP:
			-
			*/
			// A la posicion 0 dentro de response->data la tomo como signed long *
			tmp16 = (response.data);
			// Le asigno el valor del ultimo consumo del motor
			(*tmp16) = last_consumption;
			// Corrijo el largo del paquete
			response.len += 2;
		break;
 		/*case DC_MOTOR_MOTOR_STRESS_ALARM:
		break;
 		case DC_MOTOR_MOTOR_SHUT_DOWN_ALARM:
		break;*/
 		case DC_MOTOR_GET_DC_SPEED:
			/* Obtiene la velocidad del motor en cuentas del encoder por segundo
			:DATO:
			0x00 para sentido horario o 0x01 para sentido anti-horario. Numero
			entero de 16 bits con signo, que representa la velocidad en cuentas
			por segundos.
			:RESP:
			-
			*/
			// Sentido de giro del motor
			if (turn == FORWARD)
			{
				(response.data)[0] = 0x00;
			} else {
				(response.data)[0] = 0x01;
			}
			// A la posicion 1 dentro de response->data la tomo como signed long *
			tmp16 = (response.data) + 1;
			// Le asigno el valor de la velocidad ajustada a 1 segundo
			(*tmp16) = last_real_speed * INTERVAL_CORRECTION;
			// Corrijo el largo del paquete
			response.len += 3;
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
