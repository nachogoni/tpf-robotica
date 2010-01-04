#define VERSION "0.1"

/* Modulo Motor - main.c
 * PIC16F88 - MAX232 - L298 - MR-2-60-FA
 *
 *                               PIC16F88
 *                .------------------------------------.
 *           LED -|RA2/AN2/CVREF/VREF           RA1/AN1|- MOTOR:CHA_B
 *               -|RA3/AN3/VREF+/C1OUT          RA0/AN0|- L298:SEN
 *               -|RA4/AN4/T0CKI/C2OUT    RA7/OSC1/CLKI|- XT CLOCK pin1, 27pF to GND
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
#use rs232(BAUD=115200,PARITY=N,XMIT=PIN_B5,RCV=PIN_B2,BITS=8,ERRORS, TIMEOUT=1, STOP=1)
#use fast_io(A)
#use fast_io(B)

#byte porta=0x05
#byte portb=0x06
// Led
#bit led=porta.2
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

// Buffer del pto serial
#define MAX_BUFFER_SIZE	50
char buffer[MAX_BUFFER_SIZE];
int buffer_idx = 0;

// Cantidad de overflows del TMR0
long tmr0_ticks = 0;

// Valor acumulado del ADC - Consumo aprox
float adc_value = 0;
float average_consumption = 0;

// Valor de dutty del PWM
signed long dutty = 0;

// Girar -> clockwise or unclockwise
// Intercambiar entre el motor derecho y el izquierdo
#define CLOCKWISE	1
#define UNCLOCKWISE	-1

signed int turn = CLOCKWISE;
signed long counts_expected = 0;
signed long counts_total = 0;
signed long counts_to_stop = 0;
signed long last_counts = 0;
signed long last_counts2 = 0;
short counts_check = 0;
short correct_dutty = 1;

/* Examina y ejecula el comando */
void command(char * cmd);
/* Setea el PWM */
void SetPWM(signed long pwm);


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
			dutty = 0;
			SetPWM(dutty);
			correct_dutty = 0;
			last_counts = 0;
		} else {
			tmr1 = get_timer1();
			counts_to_stop -= (tmr1 - last_counts) * turn;
			correct_dutty = 1;
			last_counts = tmr1;
		}
	} else {
		correct_dutty = 1;
	}
	
	if (++tmr0_ticks == 32)
	{
		// Entra cada 200ms
		led = 1;// DEBUG?
		// Obtengo la cantidad de cuentas desde la ultima entrada
		tmr1 = get_timer1();
		set_timer1(0);
		last_counts = 0;
		last_counts2 = 0;
		// Promedio el consumo segun la cantidad de tmr0_ticks
		adc_value /= tmr0_ticks;
		printf("\n\rTimer1: %ld | Expected: %ld | dutty: %ld | ", tmr1, counts_expected, dutty); // DEBUG
		printf("consumtion: %d avrg: %g hist: %g\n\r", read_adc(), adc_value, average_consumption); // DEBUG
		tmr0_ticks = 0;

		// Mantengo el consumo promedio desde que arranque y borro el temporal
		average_consumption = (average_consumption + adc_value) / 2;
		adc_value = 0;

		// Corrijo el PWM segun lo esperado
		if ((correct_dutty == 1) && (tmr1 != counts_expected))
		{
			dutty += (counts_expected - tmr1) * 5;
			if (dutty > 1023L)
				dutty = 1023;
			else if (dutty < 0)
				dutty = 0;
			SetPWM(dutty * turn);
		}
	} else {
		led = 0; // DEBUG?
	}
}

void main() {

	// Control de Velocidad comandado por RS232
	byte c = 0;

	set_tris_a(0b11111011);
	set_tris_b(0b11100110);
	
	// ***ADC***
	setup_port_a(sAN0|VSS_VREF);
	setup_adc(ADC_CLOCK_INTERNAL);
	set_adc_channel(0);
	setup_adc_ports(sAN0);
	
	// ***PWM***
	setup_ccp1(CCP_PWM);
	// Seteo al PWM con f: 4.88 kHz, dutty = 0
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

	counts_check = 1;
	counts_to_stop = 179;
	counts_expected = 30;// DEBUG

	// Splash
	/*printf("\n\rINIT - CONTROL DE VELOCIDAD DE MOTOR - DEV: %s VERSION: %s", getenv("DEVICE"), VERSION);// DEBUG
	printf("\n\rsc: clockwise, su: unclockwise, d<value>: set dutty");
	printf("\n\re<value>: set expected, t<value>: set counts to stop");
	printf("\n\rf: dont stop, c: get total counts, z: set total counts to zero");
	printf("\n\rv<value>: set counts to");*/
	
	// Recibe datos por serial -> pasarlo a interrupcion?
	while(1)
	{
		// Mini consola
		c = getc();
		if (c)
		{
			switch (c)
			{
				case 0:
					break;
				case 13:
					// Ingreso un comando...
					buffer[buffer_idx] = '\0';
					command(buffer);
					buffer_idx = 0;
					break;
				case 8:
					// Backspace
					if (buffer_idx > 0)
						buffer_idx--;
					break;
				case '*':
					// Pido el valor del Timer0
					printf("\n\rTimer0: %d\n\r", get_timer0());
					break;
				default:
					// Otro caracter
					if (buffer_idx <= MAX_BUFFER_SIZE)
						buffer[buffer_idx++] = c;
					break;				
			}
			putc(c);
		}
	}

	return;
}

/* Verifica que el comando sea valido y lo ejecuta */
void command(char * cmd)
{
	// TODO: Crear protocolo
	if (cmd[0] == 't')
	{
		counts_check = 1;
		counts_to_stop = atol(cmd + 1);
		printf("\rCounts to stop: %ld", counts_to_stop);
	} else 
	if (cmd[0] == 'p')
	{
		printf("\rCounts to stop: %ld", counts_to_stop);
	} else 
	if (cmd[0] == 'c')
	{
		printf("\rCounts total: %ld", counts_total);
	} else 
	if (cmd[0] == 'v')
	{
		counts_total = atol(cmd + 1);
		printf("\rCounts total: %ld", counts_total);
	} else 
	if (cmd[0] == 'z')
	{
		counts_total = 0;
		printf("\rCounts total to zero");
	} else 
	if (cmd[0] == 'f')
	{
		counts_check = 0;
		counts_to_stop = 0;
		printf("\rDon't stop!");
	} else 
	if (cmd[0] == 'd')
	{
		dutty = atol(cmd + 1);
		SetPWM(dutty);
		printf("\rPWM dutty to: %ld", dutty);
	} else 
	if (cmd[0] == 'e')
	{
		counts_expected = atol(cmd + 1);
		printf("\rExpected changed to: %ld", counts_expected);
	} else 
	if (cmd[0] == 's')
	{
		turn = (cmd[1] == 'c') ? CLOCKWISE : UNCLOCKWISE;
		if (turn == CLOCKWISE)
			printf("\rSide changed to: clockwise");
		else
			printf("\rSide changed to: unclockwise");
	}
	
	return;	
}	

/* Setea el dutty del PWM segun el valor. Positivo o negativo determina el sentido */
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