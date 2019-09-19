#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

static uint8_t admux_default = (1 << REFS0);
static uint8_t adcsra_flag = (1 << ADPS2)|(1 << ADEN)|(1 << ADEN)|(1<<ADIF);

void init_adc(){
	//Division factor 16, ADC enabled, interrupt enabled
	ADCSRA |= (1 << ADPS2);
	ADCSRA |= (1 << ADEN); 
	ADCSRA |= (1 << ADEN);
	
	
	// port F as input
	DDRF  = 0x0;
	PORTF = 0xFF;
	
	//Digital input disabled
	DIDR0 = 0xFF;
	
	//Internal 2.56V Voltage Reference with external capacitor at AREF pin
	//Default channel A0
	ADMUX = admux_default;
	
	ADCSRA |= (1<<ADSC); //first conversion start
	
	
}


int read_analog(int channel){
	ADMUX = admux_default | channel;
	
    ADCSRA |= (1<<ADSC); //conversion start
    while(!(ADCSRA == adcsra_flag)); //wait for the interrupt flag
    return (uint8_t) ADC;
}
