#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>


#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256

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

void init_pwm(){
  // initialize timer0 in PWM mode
    TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);//|(1<<CS00);
    TCCR1A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);

    TCCR0B |= (1<<CS00);
    TCCR1B |= (1<<CS00);

    // make sure to make OC0A pin (pin PB7) as output pin
    DDRB |= (1<<7);
    DDRB |= (1<<5);
    DDRB |= (1<<6);
    DDRG |= (1<<5);
}



int main(void){

	init_pwm();
	UART_init();
	uint8_t buf[MAX_BUF];
	OCR0A=0x00;
	OCR1A=0x00;
  OCR1B=0x00;
  OCR0B=0x00;

	UART_putString((uint8_t*)"Give me a name!\n (8 char)\n");
	UART_getString(buf);
    //char testo[256];
    char name[8];
    for (int i=0;i<8;i++){
      name[i]=carattere(buf[i]);
    }

    UART_putString((uint8_t*)"My new name is: ");
    UART_putString((uint8_t*)name);
    UART_putString((uint8_t*)"\n");

    while(1) {
		UART_putString((uint8_t*)"What do you want to do?\n (All the commands are described in readme.txt ) \n");
		UART_getString(buf);
		char testo[256];
		for (int i=0;i<MAX_BUF;i++){
		testo[i]=carattere(buf[i]);
		}

		if(strncmp(testo,name, 8)==0 && (testo[8]=='_' && testo[9] == 'o' && testo[10] == 'n')){

J1:			UART_putString((uint8_t*)"How much intensity?\n");
			UART_getString(buf);
			char testo[256];
			for (int i=0;i<MAX_BUF;i++) {testo[i]=carattere(buf[i]);}

			if(strncmp(testo,"30%",strlen("30%"))==0){
				OCR0A=0x4C;
				OCR1A=0x4C;
        OCR1B=0x4C;
        OCR0B=0x4C;
				UART_putString((uint8_t*)"Done!\n");
			}
			else if(strncmp(testo,"60%",strlen("60%"))==0){
				OCR0A=0x9B;
				OCR1A=0x9B;
        OCR1B=0x9B;
        OCR0B=0x9B;
				UART_putString((uint8_t*)"Done!\n");
			}
			else if(strncmp(testo,"100%",strlen("100%"))==0){
				OCR0A=0xFF;
				OCR1A=0xFF;
        OCR1B=0x9B;
        OCR0B=0x9B;
				UART_putString((uint8_t*)"Done!\n");
			}
			else{
				UART_putString((uint8_t*)"Mmmm...I don't know this command!\n");
				goto J1;
			}

		}

		else if(strncmp(testo,name, 8)==0 && (testo[8]=='_' && testo[9] == 'o' && testo[10] == 'f' && testo[11] == 'f')){
			OCR0A=0x00;
			OCR1A=0x00;
      OCR1B=0x00;
      OCR0B=0x00;
			UART_putString((uint8_t*)"Done!\n");
		}

		else if(strncmp(testo,"rename", strlen("rename"))==0){
			UART_putString((uint8_t*)"Give me a new name!\n (8 char)\n");
			UART_getString(buf);
			for (int i=0;i<8;i++){
				name[i]=carattere(buf[i]);
			}

			UART_putString((uint8_t*)"My new name is: ");
			UART_putString((uint8_t*)name);
			UART_putString((uint8_t*)"\n");
		}

		else UART_putString((uint8_t*)"Mmmm...I don't know this command!\n");
	}



    _delay_ms(500); // from delay.h, wait 0.5 sec aaa


}
