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
    TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<WGM01);//|(1<<CS00);

    TCCR0B |= (1<<CS00);

    // make sure to make OC0A pin (pin PB7) as output pin
    DDRB |= (1<<7);
}



int main(void){

	init_pwm();
	UART_init();
	uint8_t buf[MAX_BUF];
	OCR0A=0x00;

  char *channel_name[8]={"uno","due","tre","quattro","cinque","sei","sette","otto"};

/*  char channel_name0[16];
  char channel_name1[16];
  char channel_name2[16];
  char channel_name3[16];
  char channel_name4[16];
  char channel_name5[16];
	*/


    while(1) {

		//lettura buffer e memorizzazione in altra variabile
		for (int i=0;i<MAX_BUF;i++){
		buf[i]=((uint8_t)'\0');
		}
		UART_getString(buf);
		char testo[256];
		for (int i=0;i<MAX_BUF;i++){
		testo[i]=carattere(buf[i]);
		}


		switch(testo[0]){
			case '0':
				if(testo[1] == '0'){//00 set_name

					char name[MAX_BUF];


					char testo[256];
					for (int i=0;i<MAX_BUF;i++){
					testo[i]=carattere(buf[i]);
					name[i] = NULL;
					}

					for (int i=0;i<MAX_BUF;i++){
						if(testo[i+2] == '\0') break;
						name[i]=testo[i+2];
					}
					UART_putString((uint8_t*)"My new name is: ");
					UART_putString((uint8_t*)name);
					UART_putString((uint8_t*)"\n");
				}
				if(testo[1] == '1'){//01 set_channel_name

          char curr_name[MAX_BUF];
          int num_channel;

          char testo[256];
					for (int i=0;i<MAX_BUF;i++){
					testo[i]=carattere(buf[i]);
					}

          num_channel = atoi(testo[2]);

          for (int i=0;i<MAX_BUF;i++){
						if(testo[i+3] == '\0') break;
						curr_name[i]=testo[i+3];
					}

          channel_name[num_channel] = curr_name;

          UART_putString((uint8_t*)"Switch_: ");
					UART_putString((uint8_t*)testo[2]);
					UART_putString((uint8_t*)" new name is: ");
          UART_putString((uint8_t*)channel_name[num_channel]);
          UART_putString((uint8_t*)"\n");
				}
				if(testo[1] == '2'){//02 set_channel_value
					UART_putString((uint8_t*)"Function not yet implemented\n");
				}
				if(testo[1] == '3'){//03 query_channels
					UART_putString((uint8_t*)"Function not yet implemented\n");
				}
				if(testo[1] == '4')
					UART_putString((uint8_t*)"Function not yet implemented\n");
				if(testo[1] == '5')
					UART_putString((uint8_t*)"Function not yet implemented\n");
				if(testo[1] == '6')
					UART_putString((uint8_t*)"Function not yet implemented\n");
				if(testo[1] == '7')
					UART_putString((uint8_t*)"Function not yet implemented\n");
				if(testo[1] == '8')
					UART_putString((uint8_t*)"Function not yet implemented\n");
				if(testo[1] == '9')
					UART_putString((uint8_t*)"Function not yet implemented\n");
				break;

			default:
					UART_putString((uint8_t*)"Function not yet implemented\n");
					break;
		}

	}



    _delay_ms(500); // from delay.h, wait 0.5 sec


}
