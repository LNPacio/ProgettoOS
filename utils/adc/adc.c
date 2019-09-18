#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h" // this includes the printf and initializes it

void init_analogic_input(){
      DIDR0 = 0x00;           //D.I. disabled on all ADC ports
      PORTF = 0xFF;			// port F as input
      PRR0 &= ~(1<<PRADC);     //Power Reduction ADC bit disable
      ADMUX = 0x61;           //AVcc, right adjusted and PIN A1
      ADCSRA = 0xcF;          //ADC Enabled, no auto trigger, Iterrupt enabled, 128 prescaller
}

int read_analog(){
    uint8_t reading = ADCH;
    ADCSRA |= (1<<ADSC); //conversion start
    return reading;
}


int main(void){
  

  // this initializes the printf/uart thingies
  printf_init();

  // this initializes the ADC channel
  init_analogic_input();

  while(1){
	 printf("In Volt su PIN A1: %d \n", read_analog());
	  _delay_ms(100); // from delay.h, wait 0.5 sec
	  if(OCR0A > 243) {
		  do{OCR0A-=10;
			  _delay_ms(100); // from delay.h, wait 0.5 sec
			  } while (OCR0A > 10);
	  }
  }

}
