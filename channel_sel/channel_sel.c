#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h" // this includes the printf and initializes it

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 

  // we connect the switch to pin 12
  // that is the bit 6 of port b
  
  const uint8_t mask_0=(1<<0);
  const uint8_t mask_1=(1<<1);
  const uint8_t mask_2=(1<<2);
  const uint8_t mask_3=(1<<3);
  const uint8_t mask_4=(1<<4);
  const uint8_t mask_5=(1<<5);
  const uint8_t mask_6=(1<<6);
  const uint8_t mask_7=(1<<7);
  uint8_t array[]={mask_0, mask_1, mask_2, mask_3, mask_4, mask_5, mask_6, mask_7};
  // we configure the pin as input
  // we enable pullup resistor on that pin
  for(int i=0; i<8; i++){
	DDRF &= ~array[i];
	PORTF |= array[i];
	}
	int key;
	uint8_t canale;
  
W1: printf("Ricerca canale...\n");
	while(1){
	for(int i=0; i<8; i++){  
		key=(PINF&array[i])==0; // we extract the bit value of the 6th bit
		if(key){
			canale = array[i];
			printf("Canale selezionato %d \n", i);
			goto W2;
		}
		_delay_ms(200); // from delay.h, wait 1 sec
	}
  }

W2: while(1){
	key=(PINF&canale)==0;
		if(!key){
			printf("Chiusura canale...\n"); 
			goto W1;
		}
	}
  }
  

