//CCS PCM V4.023 COMPILER
#define VERSION "1.0"

/* Modulo Motor - main.c
 * PIC16F88 - MAX232 - SERVO
 *
 *                               PIC16F88
 *                .------------------------------------.
 *  SOFTWARE_PWM -|RA2/AN2/CVREF/VREF           RA1/AN1|- 
 *           LED -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- 
 *           LED -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
 * RST/ICD2:MCLR -|RA5/MCLR/VPP           RA6/OSC2/CLKO|- XT CLOCK pin2, 27pF to GND
 *           GND -|VSS                              VDD|- +5v
 *  HARDWARE_PWM -|RB0/INT/CCP1       RB7/AN6/PGD/T1OSI|- ICD2:PGD
 *               -|RB1/SDI/SDA  RB6/AN5/PGC/T1OSO/T1CKI|- ICD2:PGC
 *  MAX232:R1OUT -|RB2/SDO/RX/DT           RB5/SS/TX/CK|- MAX232:T1IN
 *      ICD2:PGM -|RB3/PGM/CCP1             RB4/SCK/SCL|- 
 *                '------------------------------------'
 *    
 */

#include <16F88.h>
#DEVICE ADC = 10
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay (clock=20000000)

#use rs232(BAUD=115200,PARITY=N,XMIT=PIN_B5,RCV=PIN_B2,BITS=8,ERRORS,TIMEOUT=1,STOP=1,UART1)
#use fast_io(A)
#use fast_io(B)

#byte porta=0x05
#byte portb=0x06

// Led
#bit led1=porta.3
#bit led2=porta.4
// PWMs
#bit pwm1=porta.2
#bit pwm2=porta.2 // TODO: select another pin!
#bit pwm3=porta.2 // TODO: select another pin!
#bit pwm4=porta.2 // TODO: select another pin!
#bit pwm5=porta.2 // TODO: select another pin!
// MAX232
#bit tx=portb.5
#bit rx=portb.2

#define CARD_GROUP		0x01
#define CARD_ID			0x01

// Buffer del pto serial
#define MAX_BUFFER_SIZE	46
char buffer[MAX_BUFFER_SIZE];
int buffer_idx = 0;

// Buffer de respuesta
char resp[30];
int resp_idx = 0;

short check_comm = 0;
int command_size = 6;
int read_idx = 0;


// Software PWM - Minimo 1750 (~0.71ms)
long pwm1_t = 1750;
long pwm2_t = 1750;
long pwm3_t = 1750;
long pwm4_t = 1750;
long pwm5_t = 1750;

// 1� ~ 27.77 cuentas ~ 69.4us

// Tiempo maximo que puede durar un pulso - Maximo 6755(~2.71ms)
#define PULSE_MAX	6755
// Tiempo entre pulsos (~25ms -> ~22.29ms fijos de espera)
#define PWM_MAX		62500

/* Examina y ejecula el comando */
void command(char * cmd, int size);
/* Envia los datos por el pto serial */
void send(char * response, int size);

// Interrupcion del RS232
#INT_RDA
void RS232()
{			led1=1;
	// Agrego al buffer el caracter
	buffer[buffer_idx++] = getc();
	if (buffer_idx == MAX_BUFFER_SIZE)
		buffer_idx = 0;
					led1=0;
	return;
}

void main()
{
	long tmr1 = 1;
	int i;
	int c = 0;
	int s = 0;

/*	long j = 0;
	float k = 0f;
	char a = 0, b = 0;
	
	k = 54.37f;
	
	//a = (int)(k / 10) * 16 + ((int)k % 10);
	//b = ((int)(k * 10) % 10) * 16 + ((int)(k * 100) % 10);
	
	c = k - floor(k);
	s = (int)k;*/
	
	// Control de Velocidad comandado por RS232

	set_tris_a(0b11100011);  // TODO: check!
	set_tris_b(0b11100110);
	
	// ***TIMER1 - ENCODER COUNTER***
	// Seteo el Timer1 como fuente externa y sin divisor
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
	
	// Interrupciones
	enable_interrupts(INT_RDA);

	// Habilito las interrupciones
	enable_interrupts(GLOBAL);
	
	// Inicio del periodo
	set_timer1(0);
	// Activo todos los servos
	pwm1 = 1;
	pwm2 = 1;
	pwm3 = 1;
	pwm4 = 1;
	pwm5 = 1;

	// FOREVER
	while(1)
	{	
		// Software PWM

		// Tomo el tiempo
		tmr1 = get_timer1();
		
		// Es hora de reiniciar el pulso?
		if (tmr1 >= PWM_MAX)
		{
			// Detiene el analisis de comandos
			check_comm = 0;
			
			//led1=1;
			
			
			// Inicio del periodo
			set_timer1(0);
			// Activo todos los servos
			pwm1 = 1;
			pwm2 = 1;
			pwm3 = 1;
			pwm4 = 1;
			pwm5 = 1;
			
			if (s == 1 && c++ == 60)
			{
				pwm1_t -= 139;//28;
				c = 0;
			}
			if (s == 0 && c++ == 60)
			{
				pwm1_t += 139;//28;
				c = 0;
			}
			if (pwm1_t > 6750)
			{
				s = 1;
			}
			if (pwm1_t < 1750)
			{
				s = 0;
			}
			


			
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
			
//			led1=0;
			
		} else {
			// Tomo el tiempo
			tmr1 = get_timer1();

			// Es tiempo de desactivar el PWM?
			if (tmr1 >= pwm1_t)
				pwm1 = 0;
			if (tmr1 >= pwm2_t)
				//pwm2 = 0;
				pwm2_t = pwm2_t;
			if (tmr1 >= pwm3_t)
				//pwm3 = 0;
				pwm3_t = pwm3_t;
			if (tmr1 >= pwm4_t)
				//pwm4 = 0;
				pwm4_t = pwm4_t;
			if (tmr1 >= pwm5_t)
				//pwm5 = 0;
				pwm5_t = pwm5_t;
		}

        // Mini consola
        if (check_comm == 1)
        {
	        //printf("\r\nAnalizando...");
	        // Analizo si hay un comando
			for (i = (buffer_idx - read_idx); i > 0; i--)
				putc(buffer[read_idx++]);
			read_idx = buffer_idx;
        }
	}


	return;
}

/* Verifica que el comando sea valido y lo ejecuta */
void command(char * cmd, int size)
{
        // TODO: Crear protocolo
        /*if (cmd[0] == '+')
        {
	        duty++;
	        if (duty > 1023)
	        	duty = 1023;

			printf("\rDuty a: %ld", duty);
        } else 
        if (cmd[0] == '-')
        {
	        duty--;
	        if (duty < 0)
	        	duty = 0;

			printf("\rDuty a: %ld", duty);
        } else 
        if (cmd[0] == 'd')
        {
            duty = atol(cmd + 1);
	        if (duty < 0)
	        	duty = 0;
	        if (duty > 1023)
	        	duty = 1023;

			printf("\rDuty a: %ld", duty);
        }*/
        
       
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