/*
 * Debouncer.c
 *
 * Created: 12/31/2016 3:25:52 PM
 *  Author: hux
 */ 

 #include "Debouncer.h"
 #include <avr/io.h>

volatile uint8_t Debouncer::historyArray[3][8];
volatile uint8_t Debouncer::state = 0;

Debouncer::FPTR Debouncer::highCallback = 0;
Debouncer::FPTR Debouncer::lowCallback = 0;

uint8_t Debouncer::getBankIndex(volatile uint8_t *pinBank)
{
	if(*pinBank == PINB) return 0;
	if(*pinBank == PINC) return 1;
	if(*pinBank == PIND) return 2;
	return -1;
}

void Debouncer::pin(volatile uint8_t *pinBank, uint8_t pin)
{
	uint8_t bankIndex = Debouncer::getBankIndex(pinBank);
	// Pin is high when not pressed
	volatile uint8_t isPinHigh = ( *pinBank & ( 1 << pin ) ) ? 1 : 0;
	// Flip the bit so that 1 is on and 0 is off
	Debouncer::historyArray[bankIndex][pin] <<= 1;
	if(isPinHigh == 0)
	{
		Debouncer::historyArray[bankIndex][pin] |= 1 << 0;
	}

	// Call callbacks if we have a match
	if(Debouncer::historyArray[bankIndex][pin] == 0xFF)
	{
		if((!((Debouncer::state >> pin) & 1)) && Debouncer::highCallback != 0)
		{
			((Debouncer::FPTR)(Debouncer::highCallback))(pinBank, pin);
		}
		Debouncer::state ^= (-1 ^ Debouncer::state) & (1 << pin);
	}
	if(Debouncer::historyArray[bankIndex][pin] == 0x00)
	{
		if(((Debouncer::state >> pin) & 1) && Debouncer::lowCallback != 0)
		{
			((Debouncer::FPTR)(Debouncer::lowCallback))(pinBank, pin);
		}
		Debouncer::state ^= ~(-1 ^ Debouncer::state) & (1 << pin);
	}
}



void Debouncer::bank(volatile uint8_t *pinBank)
{
	for(int i = 0; i < 8; ++i)
	{
		Debouncer::pin(pinBank, i);
	}
}

void Debouncer::setHighCallback(void (*fptr)(volatile uint8_t*, uint8_t))
{
	Debouncer::highCallback = fptr;
}

void Debouncer::setLowCallback(void (*fptr)(volatile uint8_t*, uint8_t))
{
	Debouncer::lowCallback = fptr;
}
