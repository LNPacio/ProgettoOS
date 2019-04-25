#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h" // this includes the printf and initializes it

void init_analogic_input(){
      DIDR0 = 0x00;           //D.I. disabled on all ADC ports
      PRR0 &= ~(1<<PRADC);     //Power Reduction ADC bit disable
      ADMUX = 0x61;           //AVcc, right adjusted and PIN A1
      ADCSRA = 0xcF;          //ADC Enabled, no auto trigger, Iterrupt enabled, 128 prescaller
}

int read_analog(){
    uint8_t reading = ADCH;
    ADCSRA |= (1<<ADSC); //conversion start
    return reading;
}

void init_pwm(){
  // initialize timer0 in PWM mode
    TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<WGM01);//|(1<<CS00);

    TCCR0B |= (1<<CS00);

    // make sure to make OC0 pin (pin PB3 for atmega32) as output pin
    DDRB |= (1<<7);
}
int main(void){
  // initialize timer0 in PWM mode
  init_pwm();

  // this initializes the printf/uart thingies
  printf_init();

  // this initializes the ADC channel
  init_analogic_input();

  OCR0A=0x0f;
  while(1){

	  printf("In Volt su PIN A0: %d \n", read_analog());
	  _delay_ms(500); // from delay.h, wait 0.5 sec
  }

}
