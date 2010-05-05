//CCS PCM V4.023 COMPILER

#define CARD_GROUP	DISTANCE_SENSOR	// Ver protocol.h
#define CARD_ID		0		// Valor entre 0 y E

// Descripcion de la placa
#define DESC		"PLACA SENSORES 1.0" // Maximo DATA_SIZE bytes

// Determina el tipo de sensores principales en la placa - CAMBIARLO SEGUN CORRESPONDA
// Posibles valores: FLOOR_SENSORS o TELEMETERS_SENSORS o NONE
#define SENSORS_TYPE	TELEMETERS_SENSORS

// Determina contra que esta conectado el pin TRIGGER - CAMBIARLO SEGUN CORRESPONDA
// Posibles valores: NONE o ULTRASONIC_SENSOR o SWITCH_SENSOR o LED
//#define TRIGGER_TYPE	ULTRASONIC_SENSOR
#define TRIGGER_TYPE	SWITCH_SENSOR

// Define el estado logico para prender o apagar los sensores - CAMBIARLO SEGUN CORRESPONDA
#define SENSOR_ON	0
#define SENSOR_OFF	1

// Ancho del pulso que se debe enviar al sensor de ultrasonido como INIT - CAMBIARLO SEGUN CORRESPONDA
#define ULTRASONIC_INIT_PULSE_WIDTH_US	15

// Frame de muestreo. Cada este tiempo se realiza una nueva lectura ver CalculoTMR.xls
#define COMPLEMENT_TIME					12411 // 85ms
// Tiempo necesario para realizar la lectura en los telemetros - CAMBIARLO SEGUN CORRESPONDA
#define TELEMETERS_WAITING_TIME_CYCLES	26792 //62ms
#define TELEMETERS_COMPLEMENT_TIME		51161 //23ms complemento a COMPLEMENT_TIME
// Tiempo necesario para realizar la lectura en los sensores de piso - CAMBIARLO SEGUN CORRESPONDA
#define FLOOR_WAITING_TIME_CYCLES		62411 //5ms
#define FLOOR_COMPLEMENT_TIME			15536 //80ms complemento a COMPLEMENT_TIME
// Tiempo necesario para realizar la lectura del sensor de ultrasonido - CAMBIARLO SEGUN CORRESPONDA
#define ULTRASONIC_WAITING_TIME_CYCLES	45536 //32ms
#define ULTRASONIC_COMPLEMENT_TIME		32411 //53ms complemento a COMPLEMENT_TIME

// Tiempo entre el cambio de canal del ADC y una muestra estable (minimo 10us) - CAMBIARLO SEGUN CORRESPONDA
#define ADC_DELAY	20

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

// Estados para la maquina de estados que controla las funciones de la placa
#define STATE_FREE			0
#define STATE_START_READING	1 
#define STATE_WAIT_TO_READ	2
#define STATE_READ_VALUES	3
#define STATE_WAITING		4

// Estados para la lectura del sensor de ultrasonido
#define USONIC_STATE_START	0
#define USONIC_STATE_STOP	1 

// Posibles conexiones del pin TRIGGER
#define NONE			  (0x00)
#define ULTRASONIC_SENSOR (NONE + 1)
#define SWITCH_SENSOR     (ULTRASONIC_SENSOR + 1)
#define LED				  (SWITCH_SENSOR + 1)

// Posibles sensores principales
#define TELEMETERS_SENSORS (NONE + 1)
#define FLOOR_SENSORS      (TELEMETERS_SENSORS + 1)

// Tiempo maximo para NONE
#define NONE_TIME	65530 //0ms
#define SWITCH_TIME 65530 //0ms
#define LED_TIME 	65530 //0ms

#if SENSORS_TYPE == FLOOR_SENSORS
	#if TRIGGER_TYPE == ULTRASONIC_SENSOR
		#define SENSORS_WAITING_TIME ULTRASONIC_WAITING_TIME_CYCLES
		#define WAITING_TIME ULTRASONIC_COMPLEMENT_TIME //Complemento 85ms -> 53ms
	#else
		#define SENSORS_WAITING_TIME FLOOR_WAITING_TIME_CYCLES
		#define WAITING_TIME FLOOR_COMPLEMENT_TIME //Complemento 85ms -> 80ms
	#endif
#elif SENSORS_TYPE == TELEMETERS_SENSORS
	#define SENSORS_WAITING_TIME TELEMETERS_WAITING_TIME_CYCLES
	#define WAITING_TIME TELEMETERS_COMPLEMENT_TIME //Complemento 85ms -> 23ms
#elif SENSORS_TYPE == NONE
	#if TRIGGER_TYPE == ULTRASONIC_SENSOR
		#define SENSORS_WAITING_TIME ULTRASONIC_WAITING_TIME_CYCLES
		#define WAITING_TIME ULTRASONIC_COMPLEMENT_TIME //Complemento 85ms -> 53ms
	#else
		#define SENSORS_WAITING_TIME NONE_TIME
		#define WAITING_TIME COMPLEMENT_TIME //85ms
	#endif
#endif

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

// IO como entrada o salida
int trisB_value = 0b00100101;

// Determina el estado actual
int state;

// Vector donde se almacenan los valores de los sensores
unsigned long values[6];

// Mascara que ignora ciertos sensores
int sensorMask;

// Determina cuales sensores estan siendo leidos en este instante
int actualReadSensor;

// Determina cuales son los sensores de los que se pide la lectura
int readSensor;

// Bufferea el pedido de sensores de los que se pide la lectura
int bufferedReadSensor;

// Determina el destinatario actual
int actalTO;

// Informa quien hizo el pedido
int requestFrom;

// Informa de quien vino el ultimo pedido (mientras habia una lectura en curso)
int bufferedFrom;

// Determina el comando actual
int actalCmd;

// Informa cual fue el comando del pedido
int requestCmd;

// Informa cual fue el comando del ultimo pedido (mientras habia una lectura en curso)
int bufferedCmd;

// Estado para el sensor de ultrasonido
short usonic_state;

// Tiempo para el comienzo del pulso del sensor de ultrasonido
unsigned long pulseStart;

// Flag de aviso sobre la interrupcion en TIMER1
short intTMR;

// Tipo de alarma en trigger
int alarmType;

// Flag de alarma en trigger
short triggerAlarm;

/* Habilita los sensores segun corresponda para comenzar la lectura */
void startReading(int sensors);

/* Realiza la lectura sobre los sensores segun corresponda */
void readSensors(int sensors);

/* Interrupcion del TIMER1 */
void int_timer(void);

/* Interrupcion de RB0 */
void int_trigger(void);

/* Crea la respuesta sobre la lectura de los sensores */
void sendValues(int to, int cmd, long * values, int sensors);

/* Interrupcion del TIMER1 */
#INT_TIMER1
void int_timer(void)
{
	// Habilita el flag de aviso que sucedio la interrupcion
	intTMR = 1;
	disable_interrupts(INT_TIMER1);
}

/* Interrupcion de RB0 */
#INT_EXT
void int_trigger(void)
{
#if TRIGGER_TYPE == ULTRASONIC_SENSOR
	if (usonic_state == USONIC_STATE_START)
	{
		// Tomo el tiempo en que comienza el pulso
		pulseStart = get_timer1();
		// Cambio el tipo de flanco
		ext_int_edge(H_TO_L);
		// Cambio el estado
		usonic_state = USONIC_STATE_STOP;
	} else {
		// Tomo el tiempo y guardo el valor
		values[5] = (get_timer1() - pulseStart)/2;
		// Deshabilita la interupcion
		disable_interrupts(INT_EXT);
	}
#elif TRIGGER_TYPE == SWITCH_SENSOR
	triggerAlarm = 1;
#endif


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
	values[5] = 0x0000;
	
	// Inicializa la mascara -> todos habilitados (0x3F)
	sensorMask = 0x3F;
	
	//Determina el estado actual
	state = STATE_FREE;
	
	// Sin lectura temprana
	readSensor = 0x00;
	bufferedReadSensor = 0x00;
	actalTO = 0x00;
	requestFrom = 0x00;
	bufferedFrom = 0x00;
	actalCmd = 0x00;
	requestCmd = 0x00;
	bufferedCmd = 0x00;

	alarmType = 0x00;
	triggerAlarm = 0;
#if TRIGGER_TYPE == SWITCH_SENSOR
	disable_interrupts(INT_EXT);
#endif

#if TRIGGER_TYPE == LED
	// TRIGGER como escritura
	bit_clear(trisB_value, 0);
	set_tris_b(trisB_value);
#endif

	return;	
}	

void sendAlarm()
{
	/*TODO: revisar protocolo*/	
	triggerAlarm = 0;
	return;
}	

void main()
{
	// Placa Generica - Implementacion del protocolo
	init();

	// Init del protocol
	initProtocol();


readSensor = 0xFF;

	// FOREVER
	while(true)
	{
		
		
delay_ms(1);		
		
		// Ejecucion de la maquina de estados
		switch (state)
		{
			case STATE_FREE:
				/*
				* Analiza los paquetes y retransmite o lanza el pedido de lectura
				*/
				
#if TRIGGER_TYPE == SWITCH_SENSOR
				// Enviar alarma de trigger
				if (triggerAlarm == 1)
					sendAlarm();
#endif

				// Protocolo
				runProtocol(&command);



readSensor = 0x3F;


				
				if (readSensor != 0x00)
				{
					// Almacena el pedido sobre los sensores
					actualReadSensor = readSensor;
					readSensor = 0x00;
					actalTO = requestFrom;
					actalCmd = requestCmd;
					// Cambio de estado
					state = STATE_START_READING;
				}	
				break;
			case STATE_START_READING:
				/*
				* Genera un pedido de lectura a los sensores que lo necesiten
				* y setea los TIMERs para contabilizar los tiempos
				*/
				
				// Limpio la variable
				bufferedReadSensor = 0x00;

				// Inhabilita los sensores enmascarados
				actualReadSensor &= sensorMask;
				
				// Pedido de -START-
				startReading(actualReadSensor);
				
				// Flag de interrupcion
				intTMR = 0;
				
				// Seteo de TIMERs
				set_timer1(SENSORS_WAITING_TIME);
				enable_interrupts(INT_TIMER1);
				
				// Cambio de estado
				state = STATE_WAIT_TO_READ;
				
				break;
			case STATE_WAIT_TO_READ:
				/*
				* Espera el tiempo necesario para tomar las muestras en los sensores
				* Recibe nuevos pedidos y los agraga para una proxima lectura
				*/

#if TRIGGER_TYPE == SWITCH_SENSOR
				// Enviar alarma de trigger
				if (triggerAlarm == 1)
					sendAlarm();
#endif

				// Protocolo
				runProtocol(&command);
				
				// Almacena el pedido sobre los sensores si no hay otro ya
				if ((readSensor != 0x00) && (bufferedReadSensor == 0x00))
				{
					bufferedReadSensor = readSensor;
					bufferedFrom = requestFrom;
					bufferedCmd = requestCmd;
					readSensor = 0x00;
				}
				
				// Cambio de estado?
				if (intTMR == 1)
				{
					// El TIMER1 hizo timeout -> leer sensores
					state = STATE_READ_VALUES;
				}	
				
				break;
			case STATE_READ_VALUES:
				/*
				* Toma las muestras en los sensores y envia el paquete de respuesta
				*/
				
				// Toma las muestras de los sensores
				readSensors(actualReadSensor);
				
				// Mandar paquete de respuesta
				sendValues(actalTO, actalCMD, values, actualReadSensor);
				
				// Flag de interrupcion
				intTMR = 0;
				
				// Setea el tiempo de espera y pasa al estado de espera
				set_timer1(WAITING_TIME);
				enable_interrupts(INT_TIMER1);
				
				state = STATE_WAITING;
				
				break;
			case STATE_WAITING:
				/*
				* Espera a que pase el tiempo de espera entre lecturas para evitar rebotes
				* Recibe nuevos pedidos y los agraga para una proxima lectura
				*/
				
#if TRIGGER_TYPE == SWITCH_SENSOR
				// Enviar alarma de trigger
				if (triggerAlarm == 1)
					sendAlarm();
#endif

				// Protocolo
				runProtocol(&command);
				
				// Almacena el pedido sobre los sensores si no hay otro ya
				if ((readSensor != 0x00) && (bufferedReadSensor == 0x00))
				{
					bufferedReadSensor = readSensor;
					bufferedFrom = requestFrom;
					bufferedCmd = requestCmd;
					readSensor = 0x00;
				}
				
				// Cambio de estado?
				if (intTMR == 1)
				{
					// El TIMER1 hizo timeout
					if (bufferedReadSensor != 0x00)
					{
						// Hay un pedido pendiente y lo carga
						actualReadSensor = bufferedReadSensor;
						bufferedReadSensor = 0x00;
						actalTO = bufferedFrom;
						actalCmd = bufferedCmd;
						// Cambio de estado 
						state = STATE_START_READING;
					} else {
						state = STATE_FREE;
					}	
				}

				break;
			default:
				init();
				break;
		}
	}

	return;
}

/* Habilita los sensores segun corresponda para comenzar la lectura*/
void startReading(int sensors)
{
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

	// Sensor6
#if TRIGGER_TYPE == ULTRASONIC_SENSOR
	// Sensor6 -> ULTRASONIC_SENSOR
	if (bit_test(sensors, 5) == 1)
	{
		// Comienza el pulso de habilitacion -> TRIGGER como escritura
		bit_clear(trisB_value, 0);
		set_tris_b(trisB_value);
		// Pin en estado habilitado -> envio del pulso INIT
		trigger = 1;
		delay_us(ULTRASONIC_INIT_PULSE_WIDTH_US);
		trigger = 0;
		// Termina el pulso de habilitacion -> TRIGGER como lectura
		bit_set(trisB_value, 0);
		set_tris_b(trisB_value);
		// Setea la interrupcion sobre RB0 en flanco ascendente
		ext_int_edge(L_TO_H);
		// Seteo el estado actual del pulso del sensor de ultrasonido
		usonic_state = USONIC_STATE_START;
		// Habilita la interrupcion
		enable_interrupts(INT_EXT);
	}
#elif TRIGGER_TYPE == SWITCH_SENSOR
	// Sensor6 -> SWITCH_SENSOR
	if (bit_test(sensors, 5) == 1)
	{
		if (trigger == 1)
			values[5] = 0xFFFF;
		else
			values[5] = 0;		
	} 
#endif

	return;
}	

/* Realiza la lectura sobre los sensores segun corresponda */
void readSensors(int sensors)
{
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
	
	return;
}	

/* Crea la respuesta sobre la lectura de los sensores */
void sendValues(int to, int cmd, long * values, int sensors)
{
	int idx = 1, i;
	signed long * tmp16;
	
	command.len = MIN_LENGTH + 1;
	command.to = to;
	command.from = THIS_CARD;
	command.cmd = cmd;
	command.data[0] = sensors;

printf("TO: %X\n\rFROM: %X\n\rCOMMAND: %X\n\r", command.to, command.from, command.cmd);
	
	// Valores de los sensores en command.data segun corresponda
	for (i = 0; i < 6; i++)
	{
		if (bit_test(sensors, i) == 1)
		{
			// A la posicion 0 dentro de command.data la tomo como signed long *
			tmp16 = (command.data + idx);
			// Le asigno el valor del sensor
			(*tmp16) = values[i];

printf("DATA: %ld\n\r", values[i]);

			idx+=2;
			command.len += 2;
		}
	}

	command.crc = generate_8bit_crc((char *)(&command), command.len, CRC_PATTERN);
	// Envio del comando
//	send(&command);

printf("LEN: %X\n\rCRC: %X\n\r\n\n\n\n", command.len, command.crc);

	
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
			El ID 0x05 hace referencia al led de la placa si esta presente, 
			en caso contrario se ignora.
			:RESP:
			-
			*/
			// Enciende los sensores segun corresponda
			if ((cmd->data)[0] == 0)
				// Sensor1
				sensor1 = SENSOR_ON;
			else if ((cmd->data)[0] == 1)
				// Sensor2
				sensor2 = SENSOR_ON;
			else if ((cmd->data)[0] == 2)
				// Sensor3
				sensor3 = SENSOR_ON;
			else if ((cmd->data)[0] == 3)
				// Sensor4
				sensor4 = SENSOR_ON;
			else if ((cmd->data)[0] == 4)
				// Sensor5
				sensor5 = SENSOR_ON;
#if TRIGGER_TYPE == LED
			else if ((cmd->data)[0] == 5)
				// Led
				led1 = 1;
#endif
		break;
 		case DISTANCE_SENSOR_OFF_DISTANCE_SENSOR:
			/* Apaga el sensor de distancia indicado.
			:DATO:
			Valor de 0x00 a 0x05 que representa el ID del sensor a apagar. El
			ID 0x05 hace referencia al sensor de ultrasonido o switch de la placa.
			:RESP:
			-
			*/
			// Apaga los sensores segun corresponda
			if ((cmd->data)[0] == 0)
				// Sensor1
				sensor1 = SENSOR_OFF;
			else if ((cmd->data)[0] == 1)
				// Sensor2
				sensor2 = SENSOR_OFF;
			else if ((cmd->data)[0] == 2)
				// Sensor3
				sensor3 = SENSOR_OFF;
			else if ((cmd->data)[0] == 3)
				// Sensor4
				sensor4 = SENSOR_OFF;
			else if ((cmd->data)[0] == 4)
				// Sensor5
				sensor5 = SENSOR_OFF;
#if TRIGGER_TYPE == LED
			else if ((cmd->data)[0] == 5)
				// Led
				led1 = 0;
#endif
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
			sensorMask = (cmd->data)[0];
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
			// Envio la mascara de sensores
			response.data[0] = sensorMask;
			// Corrijo el largo del paquete
			response.len++;
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
			readSensor = (cmd->data)[0];
			requestFrom = 	response.to;
			requestCmd = response.cmd;
			sendResponse = false;
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
			readSensor = (cmd->data)[0];
			requestFrom = 	response.to;
			requestCmd = response.cmd;
			sendResponse = false;
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
#if TRIGGER_TYPE == SWITCH_SENSOR
			switch ((cmd->data)[0])
			{
				case 0x00:
					// Ignorar
					disable_interrupts(INT_EXT);
					break;
				case 0x01:
					// Cualquier cambio
					enable_interrupts(INT_EXT);
					break;
				case 0x02:
					// flanco ascendente
					ext_int_edge(H_TO_L);
					enable_interrupts(INT_EXT);
					break;
				case 0x03:
					// flanco descendente
					ext_int_edge(L_TO_H);
					enable_interrupts(INT_EXT);
					break;
				default:
					(cmd->data)[0] = 0x00;
					break;
			}
			alarmType = (cmd->data)[0];
#endif
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
