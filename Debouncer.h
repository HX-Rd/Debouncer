/*
 * Debouncer.h
 *
 * Created: 1/1/2017 7:11:57 PM
 *  Author: hux
 */ 

/*
 TODO:
	1) historyArray has to be multidementional so i can use multiple banks
	3) State has to be an array of 3 ( the banks )
*/

#ifndef DEBOUNCER_H_
#define DEBOUNCER_H_

#include <stdint-gcc.h>
class Debouncer
{
	private:
	typedef void (*FPTR)(volatile uint8_t*, uint8_t);
	static FPTR highCallback;
	static FPTR lowCallback;
	static volatile uint8_t historyArray[3][8];
	static volatile uint8_t state;
	static uint8_t getBankIndex(volatile uint8_t *pinBank);
	public:
	static void pin(volatile uint8_t *pinBank, uint8_t pin);
	static void bank(volatile uint8_t *pinBank);
	static void setHighCallback(void (*fptr)(volatile uint8_t*, uint8_t));
	static void setLowCallback(void (*fptr)(volatile uint8_t*, uint8_t));
};

#endif /* DEBOUNCER_H_ */