#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include "../avr_common/uart.h" // this includes the printf and initializes it


//funzione per stampare la matrice di luci accese/spente
void stampa_matrice(int** matrix){
	printf("\n\nLUCI ACCESE:\n\n");
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++)
			printf("%d ",matrix[i][j]);
		printf("\n");
	}
	printf("\n\n");
}

//funzione per la gestione di digital input
int di_on_off(uint8_t* array, int** matrix, int ch_sel, uint8_t canale){
	
	int  key;
	uint8_t luce_selezionata;
	
	//ciclo che scansiona tutte le porte 
	for(int i=0; i<8; i++){  
		key=(PINA&array[i])==0;
		if(key){
			
			luce_selezionata = array[i];
			
			//luce già accesa
			if (matrix[ch_sel][i]){
				printf("Spegimento luce %d, canale %d... \n", i, ch_sel);
				matrix[ch_sel][i]--;
				stampa_matrice(matrix);
				break;
			}
			
			//luce non ancora accesa
			else{
				printf("Accensione luce %d, canale %d... \n", i, ch_sel);
				matrix[ch_sel][i]++;
				stampa_matrice(matrix);
				break;
			}
		}
		_delay_ms(50); // from delay.h, wait 0.05 sec
	}
	
	//ciclo che attende il rilascio del pin
	while(1){
	key=(PINA&luce_selezionata)==0;
		if(!key) return 1;
	
	//in caso di chiusura canale inaspettata
	int key_ch=(PINF&canale)==0;
		if(!key_ch){
			return 0;
	}
  }
}

int main(void){
  
  // this initializes the printf/uart thingies
  printf_init(); 

  // inizializzazione dei pin (ci serviranno sia i primi 8 PINF che PINA)
  const uint8_t mask_0=(1<<0);
  const uint8_t mask_1=(1<<1);
  const uint8_t mask_2=(1<<2);
  const uint8_t mask_3=(1<<3);
  const uint8_t mask_4=(1<<4);
  const uint8_t mask_5=(1<<5);
  const uint8_t mask_6=(1<<6);
  const uint8_t mask_7=(1<<7);
  
  //array per i pin
  uint8_t *array = (uint8_t*) malloc (8*sizeof(uint8_t));
  array[0]=mask_0;
  array[1]=mask_1;
  array[2]=mask_2;
  array[3]=mask_3;
  array[4]=mask_4;
  array[5]=mask_5;
  array[6]=mask_6;
  array[7]=mask_7;
  
  
	// we configure the pin as input
	// we enable pullup resistor on that pin
	for(int i=0; i<8; i++){
		DDRF &= ~array[i];
		PORTF |= array[i];
		DDRA &= ~array[i];
		PORTA |= array[i];
		}
		
	//allocamento matrice contatore
	//delle luci accese o spente
	int **matrix = (int**) malloc (8*sizeof(int*));
	for(int i=0; i<8; i++)
		matrix[i] = (int*) malloc(8*sizeof(int));
	//inizializzazioe matrice contatore
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++)
			matrix[i][j] = 0;
	
	int key;
	uint8_t canale;
	
	//variabile per passaggio numero canale
	//alla funzione di_on_of
	int channel_sel;
	
  //pin channel in ascolto per
  //selezione channel
W1: printf("Ricerca canale...\n");
	while(1){
	for(int i=0; i<8; i++){  
		key=(PINF&array[i])==0; 
		if(key){
			canale = array[i];
			channel_sel= i;
			printf("Canale selezionato %d \n", i);
			goto W2;
		}
	}
  }

	//in ascolto per il rilascio del canale
W2: while(1){
	key=(PINF&canale)==0;
		if(!key){
			printf("Chiusura canale...\n"); 
			goto W1;
		}
		else{
			if(!di_on_off(array, matrix, channel_sel, canale)){
				//in caso di chiusura canale
				printf("Chiusura canale...\n");
				goto W1;
			}
	}
	_delay_ms(50); // from delay.h, wait 0.05 sec
  }
}

