#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256

void init_pwm(){
  // initialize timer0,timer1,timer2,timer4 in PWM mode
  TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);
  TCCR1A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);
  TCCR2A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);
  TCCR3A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);
  TCCR4A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);

  TCCR0B |= (1<<CS00);
  TCCR1B |= (1<<CS00);
  TCCR2B |= (1<<CS00);
  TCCR3B |= (1<<CS00);
  TCCR4B |= (1<<CS00);

  // make sure to make OC.. pin as output pin
  
  DDRB |= (1<<6); //pin 12
  DDRB |= (1<<5); //pin 11
  DDRB |= (1<<4); //pin 10
  DDRG |= (1<<5); //pin 04
 
  DDRH |= (1<<4); //pin 07
  DDRH |= (1<<3); //pin 06
  DDRE |= (1<<3); //pin 05
  DDRE |= (1<<4); //pin 02
  
  

  
  OCR1B=0x00; //pin 12 -- switch_0
  OCR1A=0x00; //pin 11 -- switch_1
  OCR2A=0x00; //pin 10 -- switch_2
  OCR4B=0x00; //pin 05 -- switch_3
  OCR4A=0x00; //pin 07 -- switch_4
  OCR3A=0x00; //pin 06 -- switch_5
  OCR0B=0x00; //pin 04 -- switch_6
  OCR3B=0x00; //pin 02 -- switch_7

}
