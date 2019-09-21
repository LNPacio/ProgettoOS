
 _____                      _       _   _                      
/  ___|                    | |     | | | |                     
\ `--. _ __ ___   __ _ _ __| |_    | |_| | ___  _   _ ___  ___ 
 `--. \ '_ ` _ \ / _` | '__| __|   |  _  |/ _ \| | | / __|/ _ \
/\__/ / | | | | | (_| | |  | |_    | | | | (_) | |_| \__ \  __/
\____/|_| |_| |_|\__,_|_|   \__|   \_| |_/\___/ \__,_|___/\___|
                                                               
                                                               

Per avviare il programma è necessario (in ordine):
1-caricare su arduinoMEGA2560 il contenuto della cartella "smart_house" (make smart_house.hex da terminale);
2-compilare il contenuto della cartella "shell" (gcc -o NOME shell_3.c) e poi eseguire il file.

Le funzioni che ci permettono di controllare il dispositvo da terminale nel migliore
dei modi sono le seguenti:

>> set_name NOME_DA_ASSEGNARE_ALLA_CENTRALINA
Viene assegnato un nome alla centralina in modo da controllarla con piu semplicita.

>> set_pwm_name NOME_ARDUINO switch_N NOME_DEL_CANALE
Funzione con lo scopo di assegnare un nome a uno switch, N è un numero
compreso tra 0 e 7 , il NOME_ARDUINO è quello impostato con la funzione
set_name e NOME_DEL_CANALE è il nome da assegnare a quello switch.

>> set_adc_name NOME_ARDUINO switch_N NOME_DEL_CANALE
Funzione con lo scopo di assegnare un nome a uno switch, N è un numero
compreso tra 0 e 7 , il NOME_ARDUINO è quello impostato con la funzione
set_name e NOME_DEL_CANALE è il nome da assegnare a quello switch.

>> set_pwm_value NOME_DEL_PWM 255
Assegnazione di un valore compreso tra 000 e 255, in modo da impostare a quel
canale al quale abbiamo assegnato il nome in precedenza, quella precisa uscita; è
possibile inoltre usare, al posto del nome del canale, la stringa speciale ALL che
imposta il valore a tutti i canali di tutti i dispositivi collegati.

>> get_pwm NOME_ARDUINO
Restituisce una lista con tutti i pwm compresa di nomi e valori ad essi assegnati.

>> get_adc NOME_ARDUINO
Restituisce una lista con tutti gli adc compresa di nomi e valori in input.

>> reset
Resetta la memoria della smart_house cancellando qualsiasi nome e valore precedentemente inserito.

>> install_ssh
Installa le dipendenze per OpenSSH-server nella tua macchina linux cosi da permetterti in futuro 
di collegarti alla shell da telefono.
Momentaneamente usiamo ssh puro (senza tunnel per non intasare i pc di nuovi utenti) ma in futuro verra 
collegato un ESP8266 che fara da host wifi con la sua pagina html per controllare il dispositivo senza
la necessita di un computer con linux installato sopra.

>> start_ssh
Apre la porta 22 nel computer cosi da permettere il collegamente ad altri eventuali dispositivi mobili collegati
alla stessa rete wifi.

>> stop_ssh
Chiude la porta 22 per non lasciar entrare piu i dispositivi.

>> quit
In modo da chiudere correttamente la comunicazione tra i due dispositivi

>> help
Un aiuto nella shell con tutti i comandi built-in.

I seguenti switch con i propri PIN sull’arduino :
____________________________________________________________
SWITCH:      |          PWM:         |         ADC:         |
             |=======================|======================|
switch_0     |         PIN-12        |        PIN-00        |
switch_1     |         PIN-11        |        PIN-00        |
switch_2     |         PIN-10        |        PIN-00        |
switch_3     |         PIN-07        |        PIN-00        |
switch_4     |         PIN-06        |        PIN-00        |  
switch_5     |         PIN-05        |        PIN-00        |
switch_6     |         PIN-04        |        PIN-00        |
switch_7     |         PIN-02        |        PIN-00        |
_____________|_______________________|______________________|


                                                                                                      Bella.
