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

#define MAX_BUF 256
int main(void){

  UART_init();
  UART_putString((uint8_t*)"write ledon_xx or ledoff_xx\n");
  UART_putString((uint8_t*)"(turned on by default)\n");
  uint8_t buf[MAX_BUF];
  
  //
  //Inizio cambiamenti
  //
  const uint8_t mask = (1<<6);
  const uint8_t mask_1 = (1<<7);
  const uint8_t mask_2 = (1<<5);

  // we configure the pin as output, clearing the bit 6
  DDRB |= mask;
  PORTB &= ~mask;
  
  // we configure the pin as output, clearing the bit 7
  DDRB |= mask_1;
  PORTB &= ~mask_1;
  
  // we configure the pin as output, clearing the bit 5
  DDRB |= mask_2;
  PORTB &= ~mask_2;
  
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
  
  while(1) {
    UART_getString(buf);
    char testo[256];
    UART_putString((uint8_t*)"received\n");
    
    
    for (int i=0;i<MAX_BUF;i++){
      testo[i]=carattere(buf[i]);
    }
    
    
    if(strncmp(testo,"ledon_00\n",strlen("ledon_00\n"))==0){
      PORTB &= ~mask;
      UART_putString((uint8_t*)"led 00 acceso\n");
      }
      
    else if (strncmp(testo,"ledon_01\n",strlen("ledon_01\n"))==0){
      PORTB &= ~mask_1;
      UART_putString((uint8_t*)"led 01 acceso\n");
    }
    
    else if (strncmp(testo,"ledon_02\n",strlen("ledon_02\n"))==0){
      PORTB &= ~mask_2;
      UART_putString((uint8_t*)"led 02 acceso\n");
    }
    
    else if (strncmp(testo,"ledoff_00\n",strlen("ledoff_00\n"))==0){
      PORTB |= mask;
      UART_putString((uint8_t*)"led 00 spento\n");
      }
      
    else if (strncmp(testo,"ledoff_01\n",strlen("ledoff_01\n"))==0){
      PORTB |= mask_1;
      UART_putString((uint8_t*)"led 01 spento\n");
    }
    
    else if (strncmp(testo,"ledoff_02\n",strlen("ledoff_02\n"))==0){
      PORTB |= mask_2;
      UART_putString((uint8_t*)"led 02 spento\n");
    }
    
    else UART_putString((uint8_t*)"comando non accettato\n ");
		
    
    UART_putString((uint8_t*)"\n\n");
    
    _delay_ms(1000); // from delay.h, wait 1 sec
    
    int key=(PINK&mask)==0; // we extract the bit value of the 6th bit
    if(key) UART_putString((uint8_t*)"led 00, 1\n");
    else UART_putString((uint8_t*)"led 00, 0\n");
    
    int key_1=(PINK&mask_1)==0; // we extract the bit value of the 7th bit
    if(key_1) UART_putString((uint8_t*)"led 01, 1\n");
    else UART_putString((uint8_t*)"led 01, 0\n");
    
    int key_2=(PINK&mask_2)==0; // we extract the bit value of the 5th bit
    if(key_2) UART_putString((uint8_t*)"led 02, 1\n");
    else UART_putString((uint8_t*)"led 02, 0\n");
    _delay_ms(1000); // from delay.h, wait 1 sec

  }
}
