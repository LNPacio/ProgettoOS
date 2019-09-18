#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

void init_adc(){
	// port F as input
	DDRF  = 0x0;
	PORTF = 0xFF;

	DIDR0 = 0x00;           //D.I. disabled on all ADC ports
    PRR0 &= ~(1<<PRADC);    //Power Reduction ADC bit disable
    ADMUX = 0x61;           //AVcc, right adjusted and PIN A1
    ADCSRA = 0xcF;         	//ADC Enabled, no auto trigger, Iterrupt enabled, 128 prescaller
}

int read_analog(){
    uint8_t reading = ADCH;
    ADCSRA |= (1<<ADSC); //conversion start
    return reading;
}
