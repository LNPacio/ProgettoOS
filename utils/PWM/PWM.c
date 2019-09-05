#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h" // this includes the printf and initializes it


void init_pwm(){
  // initialize timer0 in PWM mode
    TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<WGM01);//|(1<<CS00);

    TCCR0B |= (1<<CS00);

    // make sure to make OC0A pin (pin PB7 for atmega32) as output pin
    DDRB |= (1<<7);
}
int main(void){
  // initialize timer0 in PWM mode
  init_pwm();

  // this initializes the printf/uart thingies
  printf_init();

  

  OCR0A=0x00;
  while(1){
	  printf("Volt correnti: %d \n", OCR0A);
	  OCR0A+=10;
	  _delay_ms(100); // from delay.h, wait 0.5 sec
	  if(OCR0A > 243) {
		  do{printf("Volt correnti: %d \n", OCR0A);
			  OCR0A-=10;
			  _delay_ms(100); // from delay.h, wait 0.5 sec
			  } while (OCR0A > 10);
	  }
  }

}
