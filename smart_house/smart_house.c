#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "eeprom.h"
#include <stdlib.h>
#include <stdio.h>
#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256

uint8_t buf[MAX_BUF];
char testo[256];
char name[256];
struct UART* uart;

void set_channel_name(char** channel_name, int* channel_value, int pwmOrAdc){
	char curr_arduino[16], curr_channel[16];
				int num_channel, len_arduino, len_channel;

				char testo[256];
				for (int i=0;i<MAX_BUF;i++){
					testo[i]=carattere(buf[i]);
				}

				//01Nname*channel_name
				num_channel = (testo[2] - '0');
				if(num_channel>7 || num_channel<0){
					UART_putString(uart,(uint8_t*)"Channel not valid\n\r");
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
					UART_putString(uart,(uint8_t*)"Tha's not my name\n\r");
				}
				else{
					//controllo se il nome di un canale è uguale al nome selezionato
					int flagchannel = 0;
					int j;
					for(int i =0; i<9; i++){
						j =0;
						while(curr_channel[j] != NULL && channel_name[i][j] != NULL){
							if(curr_channel[0] == 'N' && curr_channel[1] == 'O' && curr_channel[2] == 'N' && curr_channel[3] ==  'E'&& curr_channel[4] == NULL){
								flagchannel = 2;
								break;
							}
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

					if(flagchannel == 2){
						UART_putString(uart,(uint8_t*)"You shall NONE pass!\nNONE is a special name so you are not allowed to use it, maybe you are not special enough, think to something else\n\r");
					}

					else if(flagchannel == 1){
						UART_putString(uart,(uint8_t*)"Channel name already exist\n\r");
					}

					else{
						for(int i =0; i<16;i++){
							if(i<len_channel) channel_name[num_channel][i] = curr_channel[i];
							else channel_name[num_channel][i] = NULL;
						}

						//svuotamento channel_value relativo
						if(pwmOrAdc == 1){
							channel_value[num_channel] = 0;
							on_off_value(num_channel, 0);
						}

						UART_putString(uart,(uint8_t*)curr_arduino);
						UART_putString(uart,(uint8_t*)" -> switch_");
						UART_putChar(uart,(uint8_t) num_channel + '0');
						UART_putString(uart,(uint8_t*)" new name is: ");
						UART_putString(uart,(uint8_t*)channel_name[num_channel]);
						UART_putString(uart,(uint8_t*)"\n\r");
					}
				}
}

void on_off_value(int num_switch, int value){
    switch(num_switch){
      case 0 :
        OCR1B= value;
        break;

      case 1 :
        OCR1A= value;
        break;

      case 2 :
        OCR2A= value;
        break;

      case 3 :
        OCR4B= value;
        break;

      case 4 :
        OCR4A= value;
        break;

      case 5 :
        OCR3A= value;
        break;

      case 6 :
        OCR0B= value;
        break;

      case 7 :
        OCR3B= value;
        break;
      case 8:
        OCR0B= value;
        OCR4A= value;
        OCR4B= value;
        OCR2A= value;
        OCR1A= value;
        OCR1B= value;
        OCR3A= value;
        OCR3B= value;
        break;
      default:
      //diocane
        break;
    }
}



int main(void){

	init_pwm();
	init_adc();
	uart=UART_init(NULL, 19600);

	//allocazione variabile per nomi switch
	char** pwm_channel_name = (char**) malloc(sizeof(char*) * 9);
	char** adc_channel_name = (char**) malloc(sizeof(char*) * 9);
	for(int i = 0; i<9; i++){
		pwm_channel_name[i] = (char*) malloc(sizeof(char) * 16);
		adc_channel_name[i] = (char*) malloc(sizeof(char) * 16);
	}

	int pwm_channel_value[9];
L:
	//riempimento nomi e valori
	for(int i =0; i<9; i++){
		pwm_channel_value[i] = 0;
		pwm_channel_name[i][0]='N';
		pwm_channel_name[i][1]='O';
		pwm_channel_name[i][2]='N';
		pwm_channel_name[i][3]='E';

		adc_channel_name[i][0]='N';
		adc_channel_name[i][1]='O';
		adc_channel_name[i][2]='N';
		adc_channel_name[i][3]='E';
		for(int j = 4; j<16; j++){
			pwm_channel_name[i][j]= NULL;
			adc_channel_name[i][j]= NULL;
		}
	}
	pwm_channel_name[8][0]='A';
	pwm_channel_name[8][1]='L';
	pwm_channel_name[8][2]='L';
	pwm_channel_name[8][3]=NULL;
	adc_channel_name[8][0]='A';
	adc_channel_name[8][1]='L';
	adc_channel_name[8][2]='L';
	adc_channel_name[8][3]=NULL;


	for (int i=0;i<MAX_BUF;i++){
		name[i]=NULL;
	}
	EEPROM_read(name,0,16);
	char val[3];
	char ze[3]={'0','0','0'};
	char non[16];
	for (int i=0;i<16;i++){
		non[i]=NULL;
	}
	non[0]='N';
	non[1]='O';
	non[2]='N';
	non[3]='E';

	for (int j=0;j<8;j++){
		itoa(pwm_channel_value[j],val,10);
		EEPROM_read(pwm_channel_name[j],16+16*j,16);
		EEPROM_read(adc_channel_name[j],16*9+16*j,16);
		EEPROM_read(val,16*17+j*3,3);
		pwm_channel_value[j]=atoi(val);
		if(pwm_channel_name[j]==NULL)	EEPROM_write(16+16*j,non,16);
		if(adc_channel_name[j]==NULL) EEPROM_write(16*9+16*j,non,16);
		if(pwm_channel_name[j]==NULL) EEPROM_write(16*17+j*3,ze,3);

		EEPROM_read(val,16*17+j*3,3);
		EEPROM_read(pwm_channel_name[j],16+16*j,16);
		EEPROM_read(adc_channel_name[j],16*9+16*j,16);
		pwm_channel_value[j]=atoi(val);
	}
for (int q=0;q<8;q++)		{on_off_value(q,pwm_channel_value[q]);}
    while(1) {

		//lettura buffer e memorizzazione in altra variabile
		for (int i=0;i<MAX_BUF;i++){
			buf[i]=NULL;
			testo[i] = NULL;
		}
		_delay_ms(2000);
		UART_getString(buf, uart);

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
				EEPROM_write(0,name,16);
				UART_putString(uart,(uint8_t*)"My new name is: ");
				UART_putString(uart,(uint8_t*)name);
				UART_putString(uart,(uint8_t*)"\n\r");
			}
			if(testo[1] == '1'){//01 set_pwm_channel_name
				set_channel_name(pwm_channel_name, pwm_channel_value, 1);
			}
			if(testo[1] == '2'){//02 set_pwm_channel_value
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
				for(int i =0; i<9; i++){
					k =0;
					while(channel_sel[k] != NULL && pwm_channel_name[i][k] != NULL){
						if(channel_sel[0] == 'N' && channel_sel[1] == 'O' && channel_sel[2] == 'N' && channel_sel[3] ==  'E'&& channel_sel[5] == NULL){
							flag = 0;
							break;
						}
						if(channel_sel[k] == pwm_channel_name[i][k]){
							flag = 1;
						}

						if(channel_sel[k] != pwm_channel_name[i][k] || (channel_sel[k+1] != NULL && pwm_channel_name[i][k+1] == NULL) || (channel_sel[k+1] == NULL && pwm_channel_name[i][k+1] != NULL)){
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

				if(flag < 1) UART_putString(uart,(uint8_t*)"Channel name doesn't exist\n\r");

				else{
					value = (testo[2+j]-'0')*100 + (testo[2+j+1]-'0')*10 + (testo[2+j+2]-'0');
					pwm_channel_value[num_channel] = value;
					if(num_channel == 8){
						for(int i =0; i<8;i++){
							pwm_channel_value[i] = value;
						}
					}

					on_off_value(num_channel, value);


					if(num_channel != 8){
						UART_putString(uart,(uint8_t*)"switch_");
						UART_putChar(uart,(uint8_t) cnum_channel);
						UART_putString(uart,(uint8_t*)" value: ");
						UART_putChar(uart,(uint8_t) testo[2+j]);
						UART_putChar(uart,(uint8_t) testo[2+j+1]);
						UART_putChar(uart,(uint8_t) testo[2+j+2]);
						UART_putString(uart,(uint8_t*)"\n\r");
					}
					if(num_channel == 8){
						UART_putString(uart,(uint8_t*)"ALL channels are now setted to: ");
						UART_putChar(uart,(uint8_t) testo[2+j]);
						UART_putChar(uart,(uint8_t) testo[2+j+1]);
						UART_putChar(uart,(uint8_t) testo[2+j+2]);
						UART_putString(uart,(uint8_t*)"\n\r");
					}
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
					UART_putString(uart,(uint8_t*)"Tha's not my name\n\r");
				}
				else{
					UART_putString(uart,(uint8_t*)"Device name: ");
					UART_putString(uart,(uint8_t*)name);
					for(int i = 0; i<8; i++){
						UART_putString(uart,(uint8_t*)"\nPWM_");
						UART_putChar(uart,(uint8_t) (i + '0'));
						UART_putString(uart,(uint8_t*)"\t");
						UART_putString(uart,(uint8_t*) pwm_channel_name[i]);
						UART_putString(uart,(uint8_t*)"\t");
						int centinaia = pwm_channel_value[i]/100;
						UART_putChar(uart,(uint8_t*)(centinaia+'0'));
						int decina = (pwm_channel_value[i] - (centinaia*100))/10;
						UART_putChar(uart,(uint8_t*)(decina+'0'));
						int unita = (pwm_channel_value[i] - (centinaia*100) -(decina*10));
						UART_putChar(uart,(uint8_t*)(unita+'0'));
					}
					UART_putString(uart,(uint8_t*)"\n\r");
				}
			}
			if(testo[1] == '4')
					set_channel_name(adc_channel_name, pwm_channel_value, 0);
			if(testo[1] == '5'){
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
					UART_putString(uart,(uint8_t*)"Tha's not my name\n\r");
				}
				else{
					UART_putString(uart,(uint8_t*)"Device name: ");
					UART_putString(uart,(uint8_t*)name);
					for(int ch = 0; ch<8; ch++){
						UART_putString(uart,(uint8_t*) "\nADC_");
						UART_putChar(uart,(uint8_t*)(ch+'0'));
						UART_putString(uart,(uint8_t*)"\t");
						UART_putString(uart,(uint8_t*) adc_channel_name[ch]);
						UART_putString(uart,(uint8_t*)"\t");
						int valoreADC = read_analog(ch);
						int centinaia = valoreADC/100;
						UART_putChar(uart,(uint8_t*)(centinaia+'0'));
						int decina = (valoreADC - (centinaia*100))/10;
						UART_putChar(uart,(uint8_t*)(decina+'0'));
						int unita = (valoreADC - (centinaia*100) -(decina*10));
						UART_putChar(uart,(uint8_t*)(unita+'0'));
					}
					UART_putString(uart,(uint8_t*)"\n\r");
				}
			}
			if(testo[1] == '6'){
			UART_putString(uart,(uint8_t*)"Resetting.");
			int zero = 0;
			//reset name
			for (int i=0;i<16;i++){
				EEPROM_write(i,zero,1);}
			UART_putString(uart,(uint8_t*)".");
			for (int i=0;i<8;i++){
				EEPROM_write(16+16*i,non,16);
				EEPROM_write(16*9+16*i,non,16);
				EEPROM_write(16*17+i*3,ze,3);
				EEPROM_read(val,16*17+i*3,3);
				EEPROM_read(name,0,16);
				EEPROM_read(pwm_channel_name[i],16+16*i,16);
				EEPROM_read(adc_channel_name[i],16*9+16*i,16);
				UART_putString(uart,(uint8_t*)".");
			}
			for (int i=0;i<8;i++){
				pwm_channel_value[i] = 0;
				on_off_value(i,0);
				
			}
			UART_putString(uart,(uint8_t*)"\nReset done\n\r");
			}
			if(testo[1] == '7')
				UART_putString(uart,(uint8_t*)"Function not yet implemented\n\r");
			if(testo[1] == '8')
				UART_putString(uart,(uint8_t*)"Function not yet implemented\n\r");
			if(testo[1] == '9')
				UART_putString(uart,(uint8_t*)"Function not yet implemented\n\r");
        }
        else{
			UART_putString(uart,(uint8_t*)"Function not yet implemented\n\r");
        }
				for (int j=0;j<8;j++){
					itoa(pwm_channel_value[j],val,10);
					EEPROM_write(16+16*j,pwm_channel_name[j],16);
					EEPROM_write(16*9+16*j,adc_channel_name[j],16);
					EEPROM_write(16*17+j*3,val,3);
				}

     _delay_ms(500); // from delay.h, wait 0.5 sec
	}
}
