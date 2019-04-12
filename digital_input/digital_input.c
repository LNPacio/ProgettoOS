#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../avr_common/uart.h" // this includes the printf and initializes it

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 

  // inizializzazione pin su array
  
  const uint8_t mask_0=(1<<0);
  const uint8_t mask_1=(1<<1);
  const uint8_t mask_2=(1<<2);
  const uint8_t mask_3=(1<<3);
  const uint8_t mask_4=(1<<4);
  const uint8_t mask_5=(1<<5);
  const uint8_t mask_6=(1<<6);
  const uint8_t mask_7=(1<<7);
  uint8_t array[]={mask_0, mask_1, mask_2, mask_3, mask_4, mask_5, mask_6, mask_7};
  
  for(int i=0; i<8; i++){
	DDRA &= ~array[i];
	PORTA |= array[i];
	}
	
	//contatore per luci accese/spente(1/0)
	int contatore[]={0,0,0,0,0,0,0,0};
	int key;
	uint8_t luce_selezionata;
  
W1: printf("Accendi/spegni una luce...\n");
	while(1){
	for(int i=0; i<8; i++){  
		key=(PINA&array[i])==0; // we extract the bit value of the 6th bit
		if(key){
			luce_selezionata = array[i];
			
			//luce già accesa
			if (contatore[i]){
				printf("Spegimento luce %d... \n", i);
				contatore[i]--;
				goto W2;
				break;
			}
			else{
				printf("Accensione luce %d... \n", i);
				contatore[i]++;
				goto W2;
				break;
			}
		}
		_delay_ms(200); // from delay.h, wait 0.2 sec
	}
  }

W2: printf("Luci accese: [%d %d %d %d %d %d %d %d]\n", contatore[0],contatore[1],contatore[2],contatore[3],contatore[4],contatore[5],contatore[6],contatore[7]);
	while(1){
	key=(PINA&luce_selezionata)==0;
		if(!key) goto W1;
	}
  }
  

