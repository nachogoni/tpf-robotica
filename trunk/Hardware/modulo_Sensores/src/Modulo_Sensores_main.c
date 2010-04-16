//CCS PCM V4.023 COMPILER

#define CARD_GROUP	DISTANCE_SENSOR	// Ver protocol.h
#define CARD_ID		0		// Valor entre 0 y E

// Descripcion de la placa
#define DESC		"PLACA SENSORES 1.0" // Maximo DATA_SIZE bytes

// Posibles conexiones del pin TRIGGER
#define NONE			  (0x00)
#define ULTRASONIC_SENSOR (NONE + 1)
#define SWITCH_SENSOR     (ULTRASONIC_SENSOR + 1)
#define LED				  (SWITCH_SENSOR + 1)

// Determina contra que esta conectado el pin TRIGGER - CAMBIARLO SEGUN CORRESPONDA
#define TRIGGER_CONECTION	ULTRASONIC_SENSOR

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
 *       TRIGGER -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD/SENSOR_5
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

// Pin para el control del sensore de ultrasonido, switch on/off o led
#bit trigger=portb.0
// Led
#bit led1=portb.0
// Pin para switch On/Off
#bit inOnOff=portb.0
// Pin de control del sensor de ultrasonido
#bit ultrasonido=portb.0

// Sensores - Base de los transistores
#bit sensor1=portb.1
#bit sensor2=portb.3
#bit sensor3=portb.4
#bit sensor4=portb.6
#bit sensor5=portb.7

// Sensores - Salida de los sensores (SENSE)
#bit sense1=porta.0
#bit sense2=porta.1
#bit sense3=porta.2
#bit sense4=porta.3
#bit sense5=porta.4

// Define el estado logico para prender o apagar los sensores
#define SENSOR_ON	0
#define SENSOR_OFF	1

// Tiempo para el cual ya es estable la lectura de los telemetros
#define GET_TELEMETER_VALUE	60000
// Tiempo entre el cambio de canal del ADC y una muestra estable (minimo 10us)
#define ADC_DELAY	20
// Tiempo de duracion del pulso de inicializacion para el sensor de ultrasonido
#define STOP_USONIC_PULSE	20
// Tiempo maximo de duracion para la toma de muestras de los sensores
#define MAX_GET_VALUES_TIME 65000


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

// IO como entrada o salida
int trisB_value = 0b00100101;

// Vector donde se almacenan los valores de los sensores
long values[6];
// Mascara que ignora ciertos sensores
int sensorMask;

// Estado para el sensor de ultrasonido
short usonic_state;
// Tiempo para el comienzo del pulso del sensor de ultrasonido
long pulseStart;

/* Realiza la lectura sobre los telemetros y ultrasonido */
void getTelemeters(int sensors);

/* Realiza la lectura sobre los sensores de piso y ultrasonido */
void getFloorSensors(int sensors);

long count = 0;

#INT_TIMER1
void counter(void)
{
	//set_timer1(64918); // Cada 1ms
	//set_timer1(28042); // Cada 60ms
	set_timer1(26792); // Cada 62ms
	disable_interrupts(INT_TIMER1);
	//count++;
	/*if (count == 60)
		count = 0;*/
}

#INT_EXT
void triggerINT(void)
{
	if (usonic_state == 0)
	{
		// Tomo el tiempo en que comienza el pulso
		pulseStart = get_timer1();
		// Cambio el tipo de flanco
		ext_int_edge(H_TO_L);
		// Cambio el estado
		usonic_state = 1;
	} else {
		// Tomo el tiempo y guardo el valor
		values[5] = get_timer1() - pulseStart;
		// Deshabilita la interupcion
		disable_interrupts(INT_EXT);
	}
	return;
}	

void init()
{
	// Inicializa puertos
	set_tris_a(0b11111111);
	set_tris_b(trisB_value);

	// ***ADC***
	setup_port_a(sAN0);
	setup_adc(ADC_CLOCK_INTERNAL);
	set_adc_channel(0);
	setup_adc_ports(sAN0);
	setup_vref(VREF_HIGH | 8);
	
	// Seteo el Timer1 como fuente interna
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
	set_timer1(26786);
	
	// Interrupcion sobre el Timer1
	enable_interrupts(INT_TIMER1);

	// Seteo el pin RB0 - Sensor de ultrasonido :)
	ext_int_edge(L_TO_H);
	enable_interrupts(INT_EXT);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);
	
	// Variable para hacer el reset
	reset = false;

	// Apaga todos los sensores
	sensor1 = SENSOR_OFF;
	sensor2 = SENSOR_OFF;
	sensor3 = SENSOR_OFF;
	sensor4 = SENSOR_OFF;
	sensor5 = SENSOR_OFF;

	// Inicializa los valores
	values[0] = 0x0000;
	values[1] = 0x0000;
	values[2] = 0x0000;
	values[3] = 0x0000;
	values[4] = 0x0000;
	values[5] = 0xABCD;
	
	usonic_state = 0;

	// Inicializa la mascara -> todos habilitados (0x3F)
	sensorMask = 0x01;//0x3F;

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
			

		// Tomo valores
//		getTelemeters(0xFF);

//		printf("V0: %ld V1: %ld V2: %ld V3: %ld V4: %ld U: %ld\n\r", values[0], values[1], values[2], values[3], values[4], values[5]);

//delay_ms(100);

		// Envio respuestas

		// Protocolo
	//	runProtocol(&command);
	}

	return;
}

/* Realiza la lectura sobre los telemetros y ultrasonido */
void getTelemeters(int sensors)
{
	long tmr1;
	short done = 0;
	short usonic = 0;
	short usonic_pulse = 0;
	short gotSensors = 0;
	
	sensors &= sensorMask;
	
	// Habilita los sensores segun corresponda
	
	// Sensor1
	if (bit_test(sensors, 0) == 1)
		sensor1 = SENSOR_ON;

	// Sensor2
	if (bit_test(sensors, 1) == 1)
		sensor2 = SENSOR_ON;

	// Sensor3
	if (bit_test(sensors, 2) == 1)
		sensor3 = SENSOR_ON;

	// Sensor4
	if (bit_test(sensors, 3) == 1)
		sensor4 = SENSOR_ON;

	// Sensor5
	if (bit_test(sensors, 4) == 1)
		sensor5 = SENSOR_ON;

	// Sensor6 -> ULTRASONIC_SENSOR
	if ((bit_test(sensors, 5) == 1) && (TRIGGER_CONECTION == ULTRASONIC_SENSOR))
	{
		usonic = 1;
		// Comienza el pulso de habilitacion -> TRIGGER como escritura
		bit_clear(trisB_value, 0);
		set_tris_b(trisB_value);
		// Pin en estado habilitado
		trigger = 1;
		usonic_pulse = 1;
	} else 
	// Sensor6 -> SWITCH_SENSOR
	if ((bit_test(sensors, 5) == 1) && (TRIGGER_CONECTION == SWITCH_SENSOR))
	{
		if (trigger == 1)
			values[5] = 0xFFFF;
		else
			values[5] = 0;		
	} 

	// Inicializa el timer
	set_timer1(0);

	// Toma las muestras cuando corresponda	
	while (done == 0)
	{
		delay_ms(1);
		
		// Tomo el tiempo
		tmr1 = get_timer1();

		if (tmr1 >= MAX_GET_VALUES_TIME)
		{
			done = 1;
		}	
		
		if ((usonic_pulse == 1) && (usonic == 1) && (tmr1 >= STOP_USONIC_PULSE))
		{
			usonic_pulse = 0;
			// Termina el pulso de habilitacion -> TRIGGER como lectura
			bit_set(trisB_value, 0);
			set_tris_b(trisB_value);
			// Setea la interrupcion sobre RB0 en flanco ascendente
			ext_int_edge(0, L_TO_H);
			// Seteo el estado actual del pulso del sensor de ultrasonido
			usonic_state = 0;
			// Habilita la interrupcion
			enable_interrupts(INT_EXT);
		}	
		
		if ((gotSensors == 0) && (tmr1 >= GET_TELEMETER_VALUE))
		{
			// No reentrar			
			gotSensors = 1;
			
			// Sensor1
			if (bit_test(sensors, 0) == 1)
			{
				// ADC en el pin correcto
				set_adc_channel(0);
				// Espera el tiempo necesario
				delay_us(ADC_DELAY);
				// Toma la muestra
				values[0] = read_adc();
				sensor1 = SENSOR_OFF;
			}
		
			// Sensor2
			if (bit_test(sensors, 1) == 1)
			{
				// ADC en el pin correcto
				set_adc_channel(1);
				// Espera el tiempo necesario
				delay_us(ADC_DELAY);
				// Toma la muestra
				values[1] = read_adc();
				sensor2 = SENSOR_OFF;
			}
		
			// Sensor3
			if (bit_test(sensors, 2) == 1)
			{
				// ADC en el pin correcto
				set_adc_channel(2);
				// Espera el tiempo necesario
				delay_us(ADC_DELAY);
				// Toma la muestra
				values[2] = read_adc();
				sensor3 = SENSOR_OFF;
			}
		
			// Sensor4
			if (bit_test(sensors, 3) == 1)
			{
				// ADC en el pin correcto
				set_adc_channel(3);
				// Espera el tiempo necesario
				delay_us(ADC_DELAY);
				// Toma la muestra
				values[3] = read_adc();
				sensor4 = SENSOR_OFF;
			}
		
			// Sensor5
			if (bit_test(sensors, 4) == 1)
			{
				// ADC en el pin correcto
				set_adc_channel(4);
				// Espera el tiempo necesario
				delay_us(ADC_DELAY);
				// Toma la muestra
				values[4] = read_adc();
				sensor5 = SENSOR_OFF;
			}
		}
	
	}

	return;
}	

/* Realiza la lectura sobre los sensores de piso y ultrasonido */
void getFloorSensors(int sensors)
{
	// TODO
	
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

 		case DISTANCE_SENSOR_ON_DISTANCE_SENSOR:
			/* Enciende el sensor de distancia indicado.
			:DATO:
			Valor de 0x00 a 0x05 que representa el ID del sensor a encender.
			El ID 0x05 hace referencia al sensor de ultrasonido o switch de la placa.
			:RESP:
			-
			*/
		break;
 		case DISTANCE_SENSOR_OFF_DISTANCE_SENSOR:
			/* Apaga el sensor de distancia indicado.
			:DATO:
			Valor de 0x00 a 0x05 que representa el ID del sensor a apagar. El
			ID 0x05 hace referencia al sensor de ultrasonido o switch de la placa.
			:RESP:
			-
			*/
		break;
 		case DISTANCE_SENSOR_ENABLE_DISTANCE_SENSORS:
			/* Habilita o deshabilita cada uno de los sensores de distancia conectados al
			controlador. Permite identificar los sensores a los que se debera tener en
			cuenta para futuras lecturas.
			:DATO:
			Valor de 0x00 a 0x3F donde cada bit representa el ID del sensor a
			habilitar o deshabilitar. Si 2^ID = 1 entonces el sensor ID esta habilitado.
			Si 2^ID = 0 entonces el sensor ID esta deshabilitado.
			:RESP:
			-
			*/
		break;
 		case DISTANCE_SENSOR_GET_STATUS:
			/* Obtiene el estado de habilitacion de cada uno de los sensores de distancia
			conectados al controlador.
			:DATO:
			-
			:RESP:
			Valor de 0x00 a 0x3F donde cada bit representa el ID del sensor a
			habilitar o deshabilitar. Si 2^ID = 1 entonces el sensor ID esta habilitado.
			Si 2^ID = 0 entonces el sensor ID esta deshabilitado.
			*/
		break;
 		case DISTANCE_SENSOR_GET_VALUE:
			/* Obtiene el valor promedio de la entrada de los sensores indicados.
			:DATO:
			Valor de 0x00 a 0x3F donde cada bit representa el ID del sensor
			del cual obtener la lectura.
			:RESP:
			Valor de 0x00 a 0x3F donde cada bit representa el ID del sensor
			del cual proviene el la lectura de distancia. Secuencia de numeros enteros
			positivos de 16 bits en el rango desde 0x0000 hasta 0x03FF, con el valor de
			la lectura que representa la distancia al objeto. En la secuencia de numeros
			el orden esta dado de izquierda a derecha comenzando por el bit menos
			significativo.
			En el caso del sensor de ultrasonido el rango es desde 0x0000 hasta 0x7594
			que representa la minima y maxima lectura del sensor.
			En el caso del switch, un estado logico bajo se lee como 0x0000 y un estado
			logico alto se lee como 0xFFFF.
			*/
		break;
 		case DISTANCE_SENSOR_GET_ONE_VALUE:
			/* Obtiene el valor de la entrada del sensor indicado. Igual al comando 8.5 pero
			sin realizar un promedio de lecturas.
			:DATO:
			Valor de 0x00 a 0x3F donde cada bit representa el ID del sensor
			del cual obtener la lectura.
			:RESP:
			Valor de 0x00 a 0x3F donde cada bit representa el ID del sensor
			del cual proviene el la lectura de distancia. Secuencia de numeros enteros
			positivos de 16 bits en el rango desde 0x0000 hasta 0x03FF, con el valor de
			la lectura que representa la distancia al objeto. En la secuencia de numeros
			el orden esta dado de izquierda a derecha comenzando por el bit menos
			significativo.
			En el caso del sensor de ultrasonido el rango es desde 0x0000 hasta 0x7594
			que representa la minima y maxima lectura del sensor.
			En el caso del switch, un estado logico bajo se lee como 0x0000 y un estado
			logico alto se lee como 0xFFFF.
			*/
		break;
		case DISTANCE_SENSOR_ALARM_ON_STATE:
			/* Cuando un switch esta presente en el ID: 0x05, establece si se desea o no
			recibir una alarma ante cierto cambio de estado en el mismo. Puede ser ante
			cualquier cambio o sobre un anco ascendente o descendente.
			:DATO:
			Valor entre 0x00 y 0x03 con el tipo de cambio ante el cual generar
			la alarma. Con un 0x00 ignora cualquier cambio en el switch. Se utiliza
			0x01 para que cualquier cambio en el switch genere el mensaje, 0x02 para
			que sea solo ante un flanco ascendente y 0x03 para que sea solo ante un
			flanco descendente.
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

	// CRC de la respuesta
	response.crc = generate_8bit_crc((char *)(&response), response.len, CRC_PATTERN);

	return;
}
