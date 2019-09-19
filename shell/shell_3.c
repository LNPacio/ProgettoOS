#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>


int shell_set_name(char **args);
int shell_set_pwm_value(char **args);
int shell_set_pwm_name(char **args);
int shell_get_adc_value(char **args);
int shell_set_adc_name(char **args);
int shell_query_channels(char **args);
int shell_help(char **args);
int shell_exit(char **args);
void readFromArduino();

//dichiarazione variabili globali
char device_name[16];
char buffer[32];
struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
	int tty_fd;

	unsigned char c='D';
char *help_com[]={  "set_name\t\t to set a name for your device \neg: set_name cucina",
  "set_pwm_name\t to set a name for a PWM from 0 to 7 \neg: set_pwm_name cucina switch_3 microonde",
  "set_pwm_value\t to set a value for a switch from 0 to 255\neg: set_pwm_value microonde 125\nYou can use ALL instead of NAME_SWITCH \neg: set_pwm_value ALL 255",
  "set_adc_name\t to set a name for an ADC from 0 to 7\n eg: set_adc_name cucina temperatura",
  "get_adc_value\t to get a value from the adc \neg: get_adc_value cucina temperatura",
  "query_channels\t to see all the switches names and values from a device\neg:query_channels microonde",
  "help\t\t\t to see this easy tutorial",
  "exit\t\t\t to exit from the shell"};
char *builtin_str[] = {
  "set_name",
  "set_pwm_name",
  "set_pwm_value",
  "set_adc_name",
  "get_adc_value",
  "query_channels",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &shell_set_name,
  &shell_set_pwm_name,
  &shell_set_pwm_value,
  &shell_set_adc_name,
  &shell_get_adc_value,
  &shell_query_channels,
  &shell_help,
  &shell_exit
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}
int shell_query_channels(char **args){
	int ret;
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"query_channels\" CONTRLLER_NAME\n");
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
////ADC FUNCTIONS
int shell_get_adc_value(char **args){
	int ret;
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"get_adc_value\"  \n");
  }

  else{


	  for(int i = 0; i < 32; i++){
		buffer[i] = NULL;
	  }

    //codice funzione
	  buffer[0] = '1';
	  buffer[1] = '2';

	  int i = 0;
		while(args[1][i] != NULL){
		buffer[2+i] = args[1][i];
		i++;
		}
		buffer[2+i] = args[2][0];

		ret = write(tty_fd, buffer, i+6);
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
	  buffer[0] = '1';
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

  printf("Use the man command for information on other programs.\n");
  return 1;
}


int shell_exit(char **args)
{
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
