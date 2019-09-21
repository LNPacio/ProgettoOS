#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include "server.h"

int shell_set_name(char **args);
int shell_set_pwm_value(char **args);
int shell_set_pwm_name(char **args);
int shell_get_adc(char **args);
int shell_set_adc_name(char **args);
int shell_get_pwm(char **args);
int shell_help(char **args);
int install_ssh(char **args);
int start_ssh(char **args);
int stop_ssh(char **args);
int reset(char **args);
int shell_quit(char **args);
void readFromArduino();

//dichiarazione variabili globali
char device_name[16];
char buffer[32];
struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
	int tty_fd;

	unsigned char c='D';
char *help_com[]={  "set_name\t\t\t\t\t will set a name to your device \neg: set_name irvin",
  "set_pwm_name\t\t\t\t\t will set a name to a PWM from 0 to 7 \neg: set_pwm_name irvin switch_3 lampada",
  "set_pwm_value\t\t\t\t\t will set a value to a switch from 0 to 255\neg: set_pwm_value lampada 125\t\t\t You can use ALL instead of NAME_SWITCH if you have the force\neg: set_pwm_value ALL 255",
  "set_adc_name\t\t\t\t\t will set a name to an ADC from 0 to 7\neg: set_adc_name irvin switch_0 temperatura",
  "get_adc\t\t\t\t\t\t will show you a list of couples ADC-VALUE \neg: get_adc irvin",
  "get_pwm\t\t\t\t\t\t will show you a list of couples PWM-VALUE\neg:get_pwm irvin",
  "install_ssh\t\t\t\t\t will install openssh-server on your linux machine so  you will be able to access the shell remotly",
  "start_ssh\t\t\t\t\t will start an ssh server on your inet address on port 22",
  "stop_ssh\t\t\t\t\t will interrupt the comunication with your remote controller",
  "help\t\t\t\t\t\t to see this mindblowing tutorial",
  "reset\t\t\t\t\t\t if you want to start as nothing ever happened (on your arduino device)",
  "quit\t\t\t\t\t\t will make us sad but thats life.."};
char *builtin_str[] = {
  "set_name",
  "set_pwm_name",
  "set_pwm_value",
  "set_adc_name",
  "get_adc",
  "get_pwm",
  "help",
  "install_ssh",
  "start_ssh",
  "stop_ssh",
  "reset",
  "quit"
};

int (*builtin_func[]) (char **) = {
  &shell_set_name,
  &shell_set_pwm_name,
  &shell_set_pwm_value,
  &shell_set_adc_name,
  &shell_get_adc,
  &shell_get_pwm,
  &shell_help,
  &install_ssh,
  &start_ssh,
  &stop_ssh,
  &reset,
  &shell_quit
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}
int shell_get_pwm(char **args){
	int ret;
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"get_pwm\" CONTRLLER_NAME\n");
  }
  else{
	  for(int i = 0; i < 32; i++){
		buffer[i] = NULL;
	  }

    //codice funzione
	  buffer[0] = '0';
	  buffer[1] = '3';

	  //inserimento nome nel buffer
	  int i = 0;
	  while(args[1][i] != NULL){
		  buffer[2+i] = args[1][i];
		  i++;
		}
		//invio dati
		ret = write(tty_fd, buffer, i+3);
		if(ret < 0) printf("Errore nella write buffer\n");

		readFromArduino();
  }
  return 1;
}
int shell_set_pwm_value(char **args){
	int ret;
  if (args[1] == NULL || args[2]==NULL || args[3]!=NULL) {
    fprintf(stderr, "shell: expected argument to \"set_pwm_value\" SWITCH_NAME VALUE_FRM_000_TO_255 \n");
  }
  else if ( args[2][0] == NULL || args[2][1]==NULL || args[2][2]==NULL || args[2][3]!=NULL ){
    fprintf(stderr, "The value is not valid, you have to insert a number between 000 and 255 \n");
  }
  else{


	  for(int i = 0; i < 32; i++){
		buffer[i] = NULL;
	  }

    //codice funzione
	  buffer[0] = '0';
	  buffer[1] = '2';

	  int i = 0;
		while(args[1][i] != NULL){
		buffer[2+i] = args[1][i];
		i++;
		}
		buffer[2+i] = args[2][0];
		buffer[2+i+1] = args[2][1];
		buffer[2+i+2] = args[2][2];

		ret = write(tty_fd, buffer, i+6);
		if(ret < 0) printf("Errore nella write buffer\n");


    readFromArduino();
    }
  return 1;
}
///server
int install_ssh(char **args){

  if (args[1] != NULL) {
    fprintf(stderr, "NO ARGUMENTS NEEDED\n");
  }
  else {
    sshinst();
  }
  return 1;
}
int start_ssh(char **args){

  if (args[1] != NULL) {
    fprintf(stderr, "NO ARGUMENTS NEEDED\n");
  }
  else {
    startserver();
  }
  return 1;
}
int stop_ssh(char **args){

  if (args[1] != NULL) {
    fprintf(stderr, "NO ARGUMENTS NEEDED\n");
  }
  else {
    stopserver();
  }
  return 1;
}
///end server

int shell_set_pwm_name(char **args){

	int ret, i=0;
	char code[2], num_switch;
	code[0] = '0';
	code[1] = '1';
  if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
    fprintf(stderr, "shell: expected arguments to \"set_pwm_name\" CONTROLLER_NAME switch_NUMBER \"SWITCH_NAME\" \n");
  } else {

		if(args[2][0]!= 's' || args[2][1]!= 'w'|| args[2][2]!= 'i' || args[2][3]!= 't' || args[2][4]!= 'c' || args[2][5]!= 'h' || args[2][6]!= '_'){
			fprintf(stderr, "shell: expected arguments to \"set_pwm_name\" CONTROLLER_NAME switch_NUMBER \"SWITCH_NAME\" (max length 16) \n");
		}
		else{

    num_switch = args[2][7];

    for(int i = 0; i < 32; i++){
		buffer[i] = NULL;
	  }

    //codice funzione
	  buffer[0] = '0';
	  buffer[1] = '1';
    buffer[2] = num_switch;
    //arg[1]= arduinoname arg[2]=switch arg[3]=channel name
	  int i = 0;
		while(args[1][i] != NULL){
		buffer[3+i] = args[1][i];
		i++;
		}
    buffer[3+i]='*';
    int j=0;
    while(args[3][j] != NULL){
		buffer[4+i+j] = args[3][j];
		j++;
		}
    ret = write(tty_fd, buffer, j+i+5);
		if(ret < 0) printf("Errore nella write code\n");

		readFromArduino();
    }


  }

  return 1;
}






int reset(char **args){
    int ret;
    //codice funzione
	  buffer[0] = '0';
	  buffer[1] = '6';

    ret = write(tty_fd, buffer, 3);
		if(ret < 0) printf("Errore nella write code\n");
    readFromArduino();

  return 1;
}
////ADC FUNCTIONS
int shell_get_adc(char **args){
	int ret;
  if (args[1] == NULL || args[2]!= NULL) {
    fprintf(stderr, "shell: expected argument to \"get_adc\" CONTROLLER_NAME \n");
  }

  else{


	  for(int i = 0; i < 32; i++){
		buffer[i] = NULL;
	  }

    //codice funzione
	  buffer[0] = '0';
	  buffer[1] = '5';

	  //inserimento nome nel buffer
	  int i = 0;
	  while(args[1][i] != NULL){
		  buffer[2+i] = args[1][i];
		  i++;
		}
		//invio dati
		ret = write(tty_fd, buffer, i+3);
		if(ret < 0) printf("Errore nella write buffer\n");

		readFromArduino();
    }
  return 1;
}

int shell_set_adc_name(char **args){

	int ret, i=0;
	char code[2], num_switch;
	code[0] = '1';       //non so ancora quali codici usiamo
	code[1] = '1';       //stesso problema
  if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
    fprintf(stderr, "shell: expected arguments to \"set_adc_name\" CONTROLLER_NAME switch_NUMBER \"SWITCH_NAME\" \n");
  } else {

		if(args[2][0]!= 's' || args[2][1]!= 'w'|| args[2][2]!= 'i' || args[2][3]!= 't' || args[2][4]!= 'c' || args[2][5]!= 'h' || args[2][6]!= '_'){
			fprintf(stderr, "shell: expected arguments to \"set_adc_name\" CONTROLLER_NAME switch_NUMBER \"SWITCH_NAME\" (max length 16) \n");
		}
		else{

    num_switch = args[2][7];

    for(int i = 0; i < 32; i++){
		buffer[i] = NULL;
	  }

    //codice funzione
	  buffer[0] = '0';
	  buffer[1] = '4';
    buffer[2] = num_switch;
    //arg[1]= arduinoname arg[2]=switch arg[3]=channel name
	  int i = 0;
		while(args[1][i] != NULL){
		buffer[3+i] = args[1][i];
		i++;
		}
    buffer[3+i]='*';
    int j=0;
    while(args[3][j] != NULL){
		buffer[4+i+j] = args[3][j];
		j++;
		}
    ret = write(tty_fd, buffer, j+i+5);
		if(ret < 0) printf("Errore nella write code\n");

		readFromArduino();
    }


  }

  return 1;
}
//END OF ADC FUNCTIONS

int shell_set_name(char **args){
	int ret;
	char code[2];
	code[0] = '0';
	code[1] = '0';
	if (args[1] == NULL) {
		fprintf(stderr, "shell: expected argument to \"set_name\" CONTRLLER_NAME\n");
	} else {

		ret = write(tty_fd, code, 2);
		if(ret < 0) printf("Errore nella write code\n");
		int i = 0;

    for(int j=0; j<16; j++){
      device_name[j]=NULL;
    }

    while(args[1][i] != NULL){
      device_name[i]=args[1][i];
			i++;
		}



		ret = write(tty_fd, args[1], i+1);
		if(ret < 0) printf("Errore nella write\n");

		readFromArduino();
	}
	return 1;
}


int shell_help(char **args)
{
  int i;
  printf(" _   _      _           \n");
  printf("| | | |    | |        _ \n");
  printf("| |_| | ___| |_ __   (_)\n");
  printf("|  _  |/ _ \\ | '_ \\     \n");
  printf("| | | |  __/ | |_) |  _ \n");
  printf("\\_| |_/\\___|_| .__/  (_)\n");
  printf("             | |        \n");
  printf("             |_|        \n");
  for (i = 0; i < shell_num_builtins(); i++) {
      printf("\n");
    printf("%s\n", help_com[i]);
  }

  return 1;
}


int shell_quit(char **args)
{
  tcsetattr(tty_fd, TCSANOW, &old_stdio);
  close(tty_fd);

  return 0;
}

int shell_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("shell");
  } else {

    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {

    return 1;
  }

  for (i = 0; i < shell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}

#define shell_RL_BUFSIZE 1024

char *shell_read_line(void)
{
  int bufsize = shell_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {

    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;


    if (position >= bufsize) {
      bufsize += shell_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define shell_TOK_BUFSIZE 64
#define shell_TOK_DELIM " \t\r\n\a"

char **shell_split_line(char *line)
{
  int bufsize = shell_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, shell_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += shell_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, shell_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


void shell_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
	//sleep(1);

    printf("smart_house >> ");
    line = shell_read_line();
    args = shell_split_line(line);
    status = shell_execute(args);



    free(line);
    free(args);
  } while (status);
}


void printGandalf(){
printf("                           ,---.\n                          /    |\n                         /     |\n You shall NONE pass!   /      |\n                       /       |\n          \\      ___,'         |\n                <  -'          :\n                 `-.__..--'``-,_\\_\n                    |o/ <o>` :,.)_`>\n                    :/ `     ||/)\n                    (_.).__,-` |\\ \n                    /( `.``   `| :\n                    \\\'`-.)  `  ; ;\n                    | `       /-<\n                    |     `  /   `.\n    ,-_-..____     /|  `    :__..-'\\ \n   /,'-.__\\\\  ``-./ :`      ;       \\ \n   `\\ `\\  `\\\\  \\ :  (   `  /  ,   `. \\ \n     \\` \\   \\\\   |  | `   :  :     .\\ \\ \n      \\ `\\_  ))  :  ;     |  |      ): :\n     (`-.-\'\ ||  |\\ \\   ` ;  ;       | |\n      \\-_   `;;._   ( `  /  /_       | |\n       `-.-.// ,'`-._\\__/_,'         ; |\n          \\:: :     /     `     ,   /  |\n           || |    (        ,' /   /   |\n           ||                ,'   /    |\n");
}
void readFromArduino(){
	int gand=0;
	while(read(tty_fd,&c,1)<1);
		printf("\nARDUINO: ");
		printf("%c", c);
		if(c=='Y'){
			gand=1;

		}
		while(c != '\r' ){
			if(read(tty_fd,&c,1)>0){
				printf("%c", c);
			}
		}
		if(gand){
		printGandalf();}
		printf("\n");


	/*if(read(tty_fd,&c,1)>0){
		printf("\nARDUINO: ");
		printf("%c", c);
		while(read(tty_fd,&c,1)>0){
			printf("%c", c);
		}
		printf("\n");
	}*/
}

int main(int argc, char **argv){
    printf("\t   _____                      _     _    _                      \n");
    printf("\t  / ____|                    | |   | |  | |                     \n");
    printf("\t | (___  _ __ ___   __ _ _ __| |_  | |__| | ___  _   _ ___  ___ \n");
    printf("\t  \\___ \\| '_ ` _ \\ / _` | '__| __| |  __  |/ _ \\| | | / __|/ _ \\\n");
    printf("\t  ____) | | | | | | (_| | |  | |_  | |  | | (_) | |_| \\__ \\  __/\n");
    printf("\t |_____/|_| |_| |_|\\__,_|_|   \\__| |_|  |_|\\___/ \\__,_|___/\\___|\n");
    printf("\n");
    printf("\n");
    tcgetattr(STDOUT_FILENO,&old_stdio);

    //printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);

        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;

        tty_fd=open("/dev/ttyACM0", O_RDWR | O_NONBLOCK);
        cfsetospeed(&tio,B19200);            // 19200 baud
        cfsetispeed(&tio,B19200);            // 19200 baud

        tcsetattr(tty_fd,TCSANOW,&tio);





  shell_loop();

  close(tty_fd);
  tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);

  return EXIT_SUCCESS;
}
