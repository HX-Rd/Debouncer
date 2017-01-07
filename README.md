# Debouncer for Atmega88 [![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://en.wikipedia.org/wiki/MIT_License)

## About

This debounce algorithm is quite simple. It needs 8 consecutive reads of the same value to accept a change of state for the pin. The user code can register two call backs with the signature
```C++
void (volatile uint8_t *bank, uint8_t pin)
```
one for when the pin rises and on where it falls.

The user code is also responsible for calling the bank or pin functions to trigger reads of the pins. This is done so the code is independent from the clock and the frequency of the reads can be upto the user code.
The pin fuction reads one pin in one bank and the bank function reads all the pins in that bank.

## Example

Here is a simple example
```C++
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Debouncer.h"

// Setup some pins that we will be debounceing, here we are just using pin 0 and 1 in bank c as an example
void setupButtons()
{
	DDRC &= ~(1 << DDC0) | ~(1 << DDC1);
	PORTC |= (1 << PORTC0) | (1 << PORTC1);    // turn On the Pull-up
}

void setupTimer0()
{
	TCCR0B = (1 << CS00);         // No prescaler
	TIMSK0 = (1 << TOIE0 );       //enable timer interrupt
}

ISR(TIMER0_OVF_vect)
{
  Debouncer::bank(&PINB, PINB1);  // Read pin 1 in bank B
	Debouncer::bank(&PINC);         // Read pins in bank c
}

void buttonCallbackHigh(volatile uint8_t *bank, uint8_t pin)
{
	if(*bank == PINB && pin == PINB1)
	{
		// Do something
	}
	if(*bank == PINC && pin == PINC0)
	{
		// Do something
	}
	if(*bank == PINC && pin == PINC1)
	{
		// Do something
	}
}

void buttonCallbackLow(volatile uint8_t *bank, uint8_t pin)
{
	if(*bank == PINB && pin == PINB1)
	{
		// Do something
	}
	if(*bank == PINC && pin == PINC0)
	{
		// Do something
	}
	if(*bank == PINC && pin == PINC1)
	{
		// Do something
	}
}

int main(void)
{
	setupButtons();
	setupTimer0();
	Debouncer::setHighCallback(buttonCallbackHigh);
	Debouncer::setLowCallback(buttonCallbackLow);
	sei();
	while (1)
	{
	}
}
```
