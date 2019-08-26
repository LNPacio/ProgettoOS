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
  char testo[256];
	OCR0A=0x00;

  //allocazione variabile per nomi switch
  char** channel_name = (char**) malloc(sizeof(char*) * 8);
  for(int i = 0; i<8; i++){
    channel_name[i] = (char*) malloc(sizeof(char) * 16);
  }
  char name[MAX_BUF];
  for (int i=0;i<MAX_BUF;i++){
  name[i]=NULL;
  }
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
		buf[i]=NULL;
    testo[i] = NULL;
		}
    _delay_ms(2000);
		UART_getString(buf);

		for (int i=0;i<MAX_BUF;i++){
		testo[i]=carattere(buf[i]);
		}


		if(testo[0] == '0'){
				if(testo[1] == '0'){//00 set_name




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

          char curr_arduino[16], curr_channel[16];
          int num_channel, len_arduino, len_channel;

          char testo[256];
					for (int i=0;i<MAX_BUF;i++){
					testo[i]=carattere(buf[i]);
					}

          //01Nname*channel_name
          num_channel = atoi(testo[2]);
          if(num_channel>7 || num_channel<0){
            UART_putString((uint8_t*)"Channel not valid");
            return 0;
          }

          for(int i=0; i<16; i++){
            curr_arduino[i] = NULL;
            curr_channel[i] = NULL;
          }
          len_arduino = 0;
          len_channel=0;

          while(testo[len_arduino+3] != '*'){
            curr_arduino[len_arduino] =  testo[len_arduino+3];
            len_arduino++;
          }

          while(testo[len_arduino+4+len_channel] != NULL){
            curr_channel[len_channel]=testo[len_arduino+4+len_channel];
            len_channel++;
          }

          int i =0;
          int flagarduino=1;
  
          while(curr_arduino[i] != NULL || name[i] != NULL){
            if(curr_arduino[i] != name[i]){
              flagarduino = 0;
              break;
            }
            i++;
              //UART_putString((uint8_t*)"DEBUG\n");
          }



          if(flagarduino < 1){

            UART_putString((uint8_t*)"Tha's not my name");
            return 0;
          }


          UART_putString((uint8_t*)"Name arduino: ");
          UART_putString((uint8_t*)curr_arduino);
          UART_putString((uint8_t*)" Name channel: ");
          UART_putString((uint8_t*)curr_channel);
          UART_putString((uint8_t*)"\n");



          //UART_putString((uint8_t*)"switch_");
					//UART_putChar((uint8_t) testo[2]);
					//UART_putString((uint8_t*)" new name is: ");
          //UART_putString((uint8_t*)channel_name[num_channel]);
          //UART_putString((uint8_t*)"\n");
				}
				if(testo[1] == '2'){//02 set_channel_value

          int num_channel, value;
          char channel_sel[16];

          for(int i=0; i<16; i++){
            channel_sel[i] = NULL;
          }

          char testo[256];
					for (int i=0;i<MAX_BUF;i++){
					testo[i]=carattere(buf[i]);
					}

          int j=0;
          //02pluto111

          while(testo[2+j+3]!=NULL){
            channel_sel[j] = testo[j+2];
            j++;
          }

          int flag = 1;
          for(int i =0; i<8; i++){
            if(strcmp(channel_sel, channel_name[i])==0){
              flag=0;
              num_channel=i;
              break;
            }
          }

          char cnum_channel = '0';
          cnum_channel += num_channel;

          if(flag) UART_putString((uint8_t*)"Channel name doesn't exist");

          else{

            value = itoa(atoi(testo[2+j])*100 + atoi(testo[2+j+1])*10 + atoi(testo[2+j+2]));
            UART_putString((uint8_t*)testo);
            UART_putString((uint8_t*)value);
            for (int i=0;i<MAX_BUF;i++){
        		testo[i]=NULL;
        		}
          //  testo=NULL;
            /*UART_putString((uint8_t*)"switch_");
            UART_putChar((uint8_t) cnum_channel);
            UART_putString((uint8_t*)" valore: ");
            UART_putChar((uint8_t) testo[2+j]);
            UART_putChar((uint8_t) testo[2+j+1]);
            UART_putChar((uint8_t) testo[2+j+2]);
            UART_putString((uint8_t*)"\n");*/
          }
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
        }


			else{
          if(testo[0] == NULL) UART_putString((uint8_t*)"testo[0] == NULL\n");
					UART_putString((uint8_t*)"diobelloesimpatico\n");
					UART_putChar((uint8_t)testo[0]);
          UART_putChar((uint8_t)testo[1]);
          UART_putString((uint8_t*)"\n");
        }




    _delay_ms(500); // from delay.h, wait 0.5 sec
  }


}
