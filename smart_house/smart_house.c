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

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8-bit dataa */
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
  // initialize timer0,timer1,timer2,timer4 in PWM mode
  TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);//|(1<<CS00);
  TCCR1A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);
  TCCR2A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);
  TCCR4A |= (1<<WGM00)|(1<<COM0A1)|(1<<COM0B1)|(1<<WGM01);

  TCCR0B |= (1<<CS00);
  TCCR1B |= (1<<CS00);
  TCCR2B |= (1<<CS00);
  TCCR4B |= (1<<CS00);

  // make sure to make OC.. pin as output pin
  DDRB |= (1<<7); //pin 13
  DDRB |= (1<<6); //pin 12
  DDRB |= (1<<5); //pin 11
  DDRB |= (1<<4); //pin 10
  DDRG |= (1<<5); //pin 04
  DDRH |= (1<<6); //pin 09
  DDRH |= (1<<4); //pin 07
  DDRH |= (1<<3); //pin 06

  OCR0A=0x00; //pin 13 -- switch_0
  OCR1B=0x00; //pin 12 -- switch_1
  OCR1A=0x00; //pin 11 -- switch_2
  OCR2A=0x00; //pin 10 -- switch_3
  OCR2B=0x00; //pin 09 -- switch_4
  OCR4A=0x00; //pin 07 -- switch_5
  OCR4B=0x00; //pin 06 -- switch_6
  OCR0B=0x00; //pin 04 -- switch_7

}


void on_off_value(int num_switch, int value){
    switch(num_switch){
      case 0 :
        OCR0A= 0x4C;    //alto
        OCR1B= 0x4C;    //basso
        OCR1A= 0x4C;    //basso
        OCR2A= 0x4C;    //alto
        OCR2B= 0x4C;    //alto
        OCR4A= 0x4C;    //basso
        OCR4B= 0x4C;    //basso
        OCR0B= 0x4C;    //alto
        break;

      case 1 :
        OCR1B= 0x4C;
        break;

      case 2 :
        OCR1A= 0x4C;
        break;

      case 3 :
        OCR2A= 0x4C;
        break;

      case 4 :
        OCR2B= 0x4C;
        break;

      case 5 :
        OCR4A= 0x4C;
        break;

      case 6 :
        OCR4B= 0x4C;
        break;

      case 7 :
        OCR0B= 0x4C;
        break;

      default:
        break;
    }
}



int main(void){

	init_pwm();
	UART_init();
	uint8_t buf[MAX_BUF];
  char testo[256];

  //allocazione variabile per nomi switch
  char** channel_name = (char**) malloc(sizeof(char*) * 8);
  for(int i = 0; i<8; i++){
    channel_name[i] = (char*) malloc(sizeof(char) * 16);
  }

  int channel_value[8];

  //riempimento nomi e valori
  for(int i =0; i<8; i++){
	  channel_value[i] = 0;
	  channel_name[i][0]='N';
	  channel_name[i][1]='O';
	  channel_name[i][2]='N';
	  channel_name[i][3]='E';
	  for(int j = 4; j<16; j++)
	  channel_name[i][j]= NULL;
  }

  char name[MAX_BUF];
  for (int i=0;i<MAX_BUF;i++){
  name[i]=NULL;
  }

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
          num_channel = (testo[2] - '0');
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

          //controllo che il nome inserito sia uguale al nome del dispositivo
          int i =0;
          int flagarduino=1;
          while(curr_arduino[i] != NULL || name[i] != NULL){
            if(curr_arduino[i] != name[i]){
              flagarduino = 0;
              break;
            }
            i++;
          }



          if(flagarduino < 1){
			  UART_putString((uint8_t*)"Tha's not my name");
          }
          else{

          //controllo se il nome di un canale è uguale al nome selezionato
          int flagchannel = 0;
          int j;
          for(int i =0; i<8; i++){
			  j =0;

            while(curr_channel[j] != NULL && channel_name[i][j] != NULL){

				if(curr_channel[j] == channel_name[i][j]){
					flagchannel = 1;
				}

				if(curr_channel[j] != channel_name[i][j] || (curr_channel[j+1] != NULL && channel_name[i][j+1] == NULL) || (curr_channel[j+1] == NULL && channel_name[i][j+1] != NULL)){
					flagchannel = 0;
					break;
				}

				j++;
			}
			if(flagchannel > 0){
				 break;
			 }
          }

          if(flagchannel > 0){
			  UART_putString((uint8_t*)"Channel name already exist");
		  }
		  else{


			for(int i =0; i<16;i++){
				if(i<len_channel) channel_name[num_channel][i] = curr_channel[i];
				else channel_name[num_channel][i] = NULL;
			}

			//svuotamento channel_value relativo
			channel_value[num_channel] = 0;

			UART_putString((uint8_t*)curr_arduino);
			UART_putString((uint8_t*)" -> switch_");
			UART_putChar((uint8_t) testo[2]);
			UART_putString((uint8_t*)" new name is: ");
			UART_putString((uint8_t*)channel_name[num_channel]);
			UART_putString((uint8_t*)"\n");
			}

		}
	}if(testo[1] == '2'){//02 set_channel_value

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


          int k;
          for(int i =0; i<8; i++){
			  k =0;

            while(channel_sel[k] != NULL && channel_name[i][k] != NULL){

				if(channel_sel[k] == channel_name[i][k]){
					flag = 1;
				}

				if(channel_sel[k] != channel_name[i][k] || (channel_sel[k+1] != NULL && channel_name[i][k+1] == NULL) || (channel_sel[k+1] == NULL && channel_name[i][k+1] != NULL)){
					flag = 0;
					break;
				}

				k++;
			}
			if(flag > 0){
				num_channel = i;
				break;
			 }
          }

          char cnum_channel = '0';
          cnum_channel += num_channel;

          if(flag < 1) UART_putString((uint8_t*)"Channel name doesn't exist");

          else{

          value = (testo[2+j]-'0')*100 + (testo[2+j+1]-'0')*10 + (testo[2+j+2]-'0');

          channel_value[num_channel] = value;

          on_off_value(num_channel, value);



            UART_putString((uint8_t*)"switch_");
            UART_putChar((uint8_t) cnum_channel);
            UART_putString((uint8_t*)" valore: ");
            UART_putChar((uint8_t) testo[2+j]);
            UART_putChar((uint8_t) testo[2+j+1]);
            UART_putChar((uint8_t) testo[2+j+2]);
            UART_putString((uint8_t*)"\n");
          }
        }
        if(testo[1] == '3'){//03 query_channels
			char curr_arduino[16];
			int len_arduino = 0;

			for(int i=0; i<16; i++){
            curr_arduino[i] = NULL;
            }

			while(testo[len_arduino+2] != NULL){
            curr_arduino[len_arduino] =  testo[len_arduino+2];
            len_arduino++;
			}

			int i =0;
			int flagarduino=1;
			while(curr_arduino[i] != NULL || name[i] != NULL){
				if(curr_arduino[i] != name[i]){
					flagarduino = 0;
					break;
				}
				i++;
			}



          if(flagarduino < 1){
			  UART_putString((uint8_t*)"Tha's not my name");
          }
          else{
			  UART_putString((uint8_t*)"Device name: ");
			  UART_putString((uint8_t*)name);
			  for(int i = 0; i<8; i++){
				  UART_putString((uint8_t*)"\nSwitch_");
				  UART_putChar((uint8_t) (i + '0'));
				  UART_putString((uint8_t*)" Name: ");
				  UART_putString((uint8_t*) channel_name[i]);
				  UART_putString((uint8_t*)"\tvalue: ");
				  int centinaia = channel_value[i]/100;
				  UART_putChar((uint8_t*)(centinaia+'0'));
				  int decina = (channel_value[i] - (centinaia*100))/10;
				  UART_putChar((uint8_t*)(decina+'0'));
				  int unita = (channel_value[i] - (centinaia*100) -(decina*10));
				  UART_putChar((uint8_t*)(unita+'0'));
				  }
			}
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
