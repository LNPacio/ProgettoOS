Per avviare il programma è necessario (in ordine):
1-caricare su arduinoMEGA2560 il contenuto della cartella "smart_house" (make smart_house.hex da terminale);
2-compilare il contenuto della cartella "shell" (gcc -o NOME shell_3.c) e poi eseguire il file.

Le funzioni che ci permettono di controllare il dispositvo da terminale nel migliore
dei modi sono le seguenti:

>> set_name NOME_DA_ASSEGNARE_ALLA_CENTRALINA
Viene assegnato un nome alla centralina permettendo di gestirne diverse tramite
la stessa shell assegnando nomi differenti.

>> set_channel_name NOME_ARDUINO switch_N NOME_DEL_CANALE
Funzione con lo scopo di assegnare un nome a uno switch, N è un numero
compreso tra 0 e 7 , il NOME_ARDUINO è quello impostato con la funzione
set_name e NOME_DEL_CANALE è il nome da assegnare a quello switch.

>> set_channel_value NOME_DEL_CANALE 255
Assegnazione di un valore compreso tra 000 e 255, in modo da impostare a quel
canale al quale abbiamo assegnato il nome in precedenza, quella precisa uscita; è
possibile inoltre usare, al posto del nome del canale, la stringa speciale ALL che
imposta il valore a tutti i canali di tutti i dispositivi collegati.

>> exit
In modo da chiudere correttamente la comunicazione tra i due dispositivi

>> help
Un aiuto nella shell con tutti i comandi built-in.

I seguenti switch con i propri PIN sull’arduino :
switch_0 PIN-12
switch_1 PIN-11
switch_2 PIN-10
switch_3 PIN-07
switch_4 PIN-06
switch_5 PIN-05
switch_6 PIN-04
switch_7 PIN-02

