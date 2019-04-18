#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>


#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)

void UART_init(void){
  // Set baud rate
  UBRR0H = (uint8_t)(MYUBRR>>8);
  UBRR0L = (uint8_t)MYUBRR;

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8-bit data */
  UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);   /* Enable RX and TX */

}

void UART_putChar(uint8_t c){
  // wait for transmission completed, looping on status bit
  while ( !(UCSR0A & (1<<UDRE0)) );

  // Start transmission
  UDR0 = c;
}
char carattere(uint8_t c){
  // wait for transmission completed, looping on status bit
  while ( !(UCSR0A & (1<<UDRE0)) );

  // Start transmission
  return (char *)c;

}

uint8_t UART_getChar(void){
  // Wait for incoming data, looping on status bit
  while ( !(UCSR0A & (1<<RXC0)) );

  // Return the data
  return UDR0;

}

// reads a string until the first newline or 0
// returns the size read
uint8_t UART_getString(uint8_t* buf){
  uint8_t* b0=buf; //beginning of buffer
  while(1){
    uint8_t c=UART_getChar();
    *buf=c;
    ++buf;
    // reading a 0 terminates the string
    if (c==0)
      return buf-b0;
    // reading a \n  or a \r return results
    // in forcedly terminating the string
    if(c=='\n'||c=='\r'){
      *buf=0;
      ++buf;
      return buf-b0;
    }
  }
}

void UART_putString(uint8_t* buf){
  while(*buf){
    UART_putChar(*buf);
    ++buf;
  }
}

void stampaRiepilogo(uint8_t* array){
	UART_putString((uint8_t*)"\n\nRiepilogo led:\n");
	int key;
	for(int i = 0; i<4; i++){
		char curr = i+48;
		key=(PINK&array[i])==0; // we extract the bit value of the 6th bit
		UART_putString((uint8_t*)"led 0");
		UART_putChar((uint8_t*)curr);
		if(key){
			 UART_putString((uint8_t*)", ON\n");
		 }
		else UART_putString((uint8_t*)", OFF\n");
	}
	UART_putString((uint8_t*)"\nWrite ledon_xx or ledoff_xx\n");
}


#define MAX_BUF 256
int main(void){

  UART_init();
  UART_putString((uint8_t*)"Write ledon_xx or ledoff_xx\n");
  UART_putString((uint8_t*)"(leds turned on by default)\n");
  uint8_t buf[MAX_BUF];
  
  //
  //Inizio cambiamenti
  //
  const uint8_t mask = (1<<6);
  const uint8_t mask_1 = (1<<7);
  const uint8_t mask_2 = (1<<5);
  const uint8_t mask_3 = (1<<4);
  
  uint8_t *array = (uint8_t*) malloc (4*sizeof(uint8_t));
  array[0]=mask;
  array[1]=mask_1;
  array[2]=mask_2;
  array[3]=mask_3;
  
  //OUTPUT
  
  // we configure the pin as output, clearing the bit 6
  DDRB |= mask;
  PORTB &= ~mask;
  
  // we configure the pin as output, clearing the bit 7
  DDRB |= mask_1;
  PORTB &= ~mask_1;
  
  // we configure the pin as output, clearing the bit 5
  DDRB |= mask_2;
  PORTB &= ~mask_2;
  
  // we configure the pin as output, clearing the bit 4
  DDRB |= mask_3;
  PORTB &= ~mask_3;
  
  //INPUT
  
  // we configure the pin as input, clearing the bit 6
  // we enable pullup resistor on that pin
  DDRK &= ~mask;
  PORTK |= mask;
  
  // we configure the pin as input, clearing the bit 7
  // we enable pullup resistor on that pin
  DDRK &= ~mask_1;
  PORTK |= mask_1;
  
  // we configure the pin as input, clearing the bit 5
  // we enable pullup resistor on that pin
  DDRK &= ~mask_2;
  PORTK |= mask_2;
  
  // we configure the pin as input, clearing the bit 4
  // we enable pullup resistor on that pin
  DDRK &= ~mask_3;
  PORTK |= mask_3;
  
  while(1) {
    UART_getString(buf);
    char testo[256];
    UART_putString((uint8_t*)"received\n");
    
    
    for (int i=0;i<MAX_BUF;i++){
      testo[i]=carattere(buf[i]);
    }
    
    if(strncmp(testo,"ledon_",strlen("ledon_"))==0){
		switch (testo[7]){
			
			case '0':
				PORTB &= ~mask;
				UART_putString((uint8_t*)"accensione led 00...\n");
				break;
				
			case '1':
				PORTB &= ~mask_1;
				UART_putString((uint8_t*)"accensione led 01...\n");
				break;
			
			case '2':
				PORTB &= ~mask_2;
				UART_putString((uint8_t*)"accensione led 02...\n");
				break;
			
			case '3':
				PORTB &= ~mask_3;
				UART_putString((uint8_t*)"accensione led 03...\n");
				break;
			
			default:
				UART_putString((uint8_t*)"led non esistente\n");
		}
	}
	
    else if(strncmp(testo,"ledoff_",strlen("ledoff_"))==0){
		switch (testo[8]){
			
			case '0':
				PORTB |= mask;
				UART_putString((uint8_t*)"spegnimento led 00...\n");
				break;
				
			case '1':
				PORTB |= mask_1;
				UART_putString((uint8_t*)"spegnimento led 01...\n");
				break;
			
			case '2':
				PORTB |= mask_2;
				UART_putString((uint8_t*)"spegnimento led 02...\n");
				break;
				
			case '3':
				PORTB |= mask_3;
				UART_putString((uint8_t*)"spegnimento led 03...\n");
				break;
			
			default:
				UART_putString((uint8_t*)"led non esistente\n");
			}
	}
	else UART_putString((uint8_t*)"comando non accettato\n ");
		
    _delay_ms(500); // from delay.h, wait0.5 sec
    
    stampaRiepilogo(array);
    
    _delay_ms(500); // from delay.h, wait 0.5 sec
    
    UART_putString((uint8_t*)"\n\n");

  }
}
