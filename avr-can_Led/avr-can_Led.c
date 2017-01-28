/*	Sample program for Olimex AVR-CAN with AT90CAN128 processor
 *	Blink the Led
 *	Compile with AVRStudio+WinAVR (gcc version 3.4.6)
 */

#define	__AVR_AT90CAN128__	1
#define OSCSPEED	16000000		/* in Hz */

#include "avr/io.h"
#include "libraries/lib_mcu/can/can_lib.h"
#include "libraries/lib_mcu/can/can_drv.h"
#include "config.h"

void PORT_Init()
{
	PORTA = 0b00000000;
	DDRA = 0b00000000;

	PORTB = 0b00000000;
	DDRB = 0b00000000;

	PORTC = 0b00000000;
	DDRC = 0b00000000;

	PORTD = 0b00000000;
	DDRD = 0b00001000;			//Led set as output (Bit4 = 1)

	PORTE = 0b00000000;
	DDRE = 0b00010000;

	PORTF = 0b00000000;
	DDRF = 0b00000000;
}

void WDT_Off(void)
{
	asm("push r30");		// using GCC without optimizations results in
	asm("ldi r30, 0x18");	// a code not meeting the requirements
	asm("sts 0x60, r30");	// of the datasheet.
	asm("ldi r30, 0x7");
	asm("sts 0x60, r30");
	asm("pop r30");
	//WDTCR = (1<<WDCE) | (1<<WDE);
	//WDTCR = 0x07;		//increase Watchdog time-out period (Bit0,1,2 = 1)
	//WDTCR = 0x00;		//increase Watchdog time-out period (Bit0,1,2 = 1)
}

int main()
{
	unsigned long i;
	WDT_Off();
	PORT_Init();
	asm("WDR");
	while (1)
	{
		asm("WDR");		//avoiding reset of the program
		PORTE = PORTE | 0b00010000;		//Led off (Bit4 = 1)
		for (i=0; i<100000; i++);			//Delay
		asm("WDR");
		PORTE = PORTE & 0b11101111;		//Led on (Bit4 = 0)
		for (i=0; i<400000; i++);			//Delay
	}

}
