#define VERSION "0.1"

/* Modulo Motor - main.c
 * PIC16F88 - MAX232 - L298 - MR-2-60-FA
 *
 *                               PIC16F88
 *                .------------------------------------.
 *          VREF -|RA2/AN2/CVREF/VREF           RA1/AN1|- MOTOR:CHA_B
 *           LED -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- L298:SEN
 *           LED -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
 * RST/ICD2:MCLR -|RA5/MCLR/VPP           RA6/OSC2/CLKO|- XT CLOCK pin2, 27pF to GND
 *           GND -|VSS                              VDD|- +5v
 *   L298:ENABLE -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD
 *     MOTOR:IDX -|RB1/SDI/SDA  RB6/AN5/PGC/T1OSO/T1CKI|- ICD2:PGC/MOTOR:CHA_A
 *  MAX232:R1OUT -|RB2/SDO/RX/DT           RB5/SS/TX/CK|- MAX232:T1IN
 * L298:INPUT_B/ -|RB3/PGM/CCP1             RB4/SCK/SCL|- L298:INPUT_A
 *     ICD2:PGM   '------------------------------------'
 *    
 */

#include <16F88.h>
#include <stdio.h>
#include <stdlib.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay (clock=20000000)
//#device adc = 8
#use rs232(BAUD=115200,PARITY=N,XMIT=PIN_B5,RCV=PIN_B2,BITS=8,ERRORS,TIMEOUT=1,STOP=1,UART1)
#use fast_io(A)
#use fast_io(B)

#byte porta=0x05
#byte portb=0x06
// Led
#bit led1=porta.3
#bit led2=porta.4
// L298
#bit inputA=portb.4
#bit inputB=portb.3
#bit enable=portb.0
#bit sensor=porta.0
// Motor inuts
#bit motorIDX=portb.1
#bit channelA=portb.6
#bit channelB=porta.1
// MAX232
#bit tx=portb.5
#bit rx=portb.2

#define CARD_GROUP		0x01
#define CARD_ID			0x01

// Girar -> clockwise or unclockwise
// Intercambiar entre el motor derecho y el izquierdo
#define CLOCKWISE	1
#define UNCLOCKWISE	-1

signed int turn = CLOCKWISE;

// Buffer del pto serial
#define MAX_BUFFER_SIZE	50
char buffer[MAX_BUFFER_SIZE];
int buffer_idx = 0;

// Buffer de respuesta
char resp[30];
int resp_idx = 0;

// Cantidad de overflows del TMR0
long tmr0_ticks = 0;

// Valor acumulado del ADC - Consumo aprox
float adc_value = 0;

// Valor de duty del PWM
signed long duty = 0;

signed long counts_expected = 0;
signed long counts_total = 0;
signed long counts_to_stop = 0;
signed long last_counts = 0;
signed long last_counts2 = 0;
short counts_check = 0;
short correct_duty = 1;

short rs232c = 0;
char c = 0;
int command_size = 6;

/* Examina y ejecula el comando */
void command(char * cmd, int size);
/* Setea el PWM */
void SetPWM(signed long pwm);
/* Envia los datos por el pto serial */
void send(char * response, int size);

// Interrupcion del Timer0
#INT_RTCC
void Timer0_INT()
{
	long tmr1;
	// Seteo el valor para que interrumpa cada 6.25ms
	set_timer0(12);
	
	// Tomo la lectura del ADC
	adc_value += read_adc();
	//printf(":%d:",read_adc()); // DEBUG

	// Agrego al historico de cuentas el ultimo acumulado
	tmr1 = get_timer1();
	counts_total += (tmr1 - last_counts2) * turn;
	last_counts2 = tmr1;
	
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
			tmr1 = get_timer1();
			counts_to_stop -= (tmr1 - last_counts);// * turn;
			correct_duty = 1;
			last_counts = tmr1;
		}
	} else {
		correct_duty = 1;
	}
	
	if (++tmr0_ticks == 32)
	{
		// Entra cada 200ms
		led1 = 1;// DEBUG?
		// Obtengo la cantidad de cuentas desde la ultima entrada
		tmr1 = get_timer1();
		set_timer1(0);
		last_counts = 0;
		last_counts2 = 0;
		// Promedio el consumo segun la cantidad de tmr0_ticks
		adc_value /= tmr0_ticks;
		printf("\n\rTimer1: %ld | Expected: %ld | duty: %ld | ", tmr1, counts_expected, duty); // DEBUG
		printf("consumtion avrg: %g\n\r", adc_value); // DEBUG
		tmr0_ticks = 0;

		// Mantengo el consumo promedio desde que arranque y borro el temporal
		adc_value = 0;

		// Corrijo el PWM segun lo esperado
		if ((correct_duty == 1) && (tmr1 != counts_expected))
		{
			duty += (counts_expected - tmr1) * 5;
			if (duty > 1023L)
				duty = 1023;
			else if (duty < 0)
				duty = 0;
			SetPWM(duty * turn);
		}
	} else {
		led1 = 0; // DEBUG?
	}
	return;
}

void main()
{
	// Control de Velocidad comandado por RS232

	set_tris_a(0b11100111);
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
	enable_interrupts(INT_RDA);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);
	
	//counts_expected = 30;
	
	// FOREVER
	while(true)
	{
		// Mini consola
		if (rs232c == 1)
		{
			rs232c = 0;

			//DoSomething
			

			
			if (buffer_idx == command_size)
			{
				// Execute command
				command(buffer, command_size);
				// Clean buffer
				for (buffer_idx = 0; buffer_idx <= command_size; buffer_idx++)
					buffer[buffer_idx] = '\0';
				buffer_idx = 0;
			}		

		}
	}

	return;
}

// Interrupcion del RS232
#INT_RDA
void RS232()
{
	// Agrego al buffer el caracter
	buffer[buffer_idx] = c = getc();
	/*
	// Campo LARGO
	if (buffer_idx == 5)
	{
		command_size = 6 + 1 + buffer[buffer_idx];
	}
	
	// Comando completo
	if (buffer_idx == command_size)
		rs232c = 1;

	// Buffer ciclico
	if (buffer_idx == MAX_BUFFER_SIZE)
		buffer_idx = 0;
	else
		buffer_idx++;*/
	putc(c);
	return;
}

/* Verifica que el comando sea valido y lo ejecuta */
void command(char * cmd, int size)
{
	short read = 0;
	short echo = 0;
	
	// Broadcast general
	if (buffer[0] == 0xFF)
	{
		// Atiende el mensaje
		read = 1;
		// Repite el comando
		echo = 1;
	}
	else if ((buffer[0] & 0x7F) == CARD_GROUP)
	{
		if (((buffer[0] & 0x80) == 0x80) || (buffer[1] == CARD_ID))
		{
			// Atiende el mensaje
			read = 1;
			// Repite el comando
			echo = ((buffer[0] & 0x80) == 0x80);
		}
	}
	
	if (read == 1)
	{
		// Parte comun a todas las respuestas
		resp[0] = cmd[2] & 0x7F;
		resp[1] = cmd[3];
		resp[2] = CARD_GROUP;
		resp[3] = CARD_ID;

		switch (cmd[4])
		{
			case 0x03:
				// Ping -> Pong
				resp[4] = cmd[4];
				resp[5] = 0;
				resp_idx = 6;
				send(resp, resp_idx);
			break;

			default:
			break;
		}	
		
		
	}
	
	// Hacer echo si corresponde (TODO: ver CRC)
	if (echo == 1)
		send(buffer, size);

	return;	
}	

/* Envia los datos por el pto serial */
void send(char * response, int size)
{
	int i, checksum = 0;
	
	for (i = 0; i < size; i++)
	{
		// Calcular el crc
		checksum ^= response[i];
		putc(response[i]);
	}
	
	// Enviar el CRC
	putc(checksum);
	
	return;	
}	

/* Setea el duty del PWM segun el valor. Positivo o negativo determina el sentido */
void setPWM(signed long pwm)
{
  long pset;
 
  if (pwm < 0)
  {
	inputA = 0;
	inputB = 1;
  } else {
	inputA = 1;
	inputB = 0;
  }

  pset = (abs(pwm));

  if (pset > 1023L)
    pset = 1023;

  set_pwm1_duty(pset);
  
  return;
}