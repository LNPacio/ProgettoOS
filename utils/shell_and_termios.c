#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>


int shell_set_name(char **args);
int shell_set_channel_value(char **args);
int shell_set_channel_name(char **args);
int shell_query_channels(char **args);
int shell_help(char **args);
int shell_exit(char **args);
void readFromArduino();

struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
	int tty_fd;
	
	unsigned char c='D';


char *builtin_str[] = {
  "set_name",
  "set_channel_name",
  "set_channel_value",
  "query_channels",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &shell_set_name,
  &shell_set_channel_name,
  &shell_set_channel_value,
  &shell_query_channels,
  &shell_help,
  &shell_exit
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}
int shell_query_channels(char **args){
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"query_channels\" CONTRLLER_NAME\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("too many arguments");
    }
  }
  return 1;
}
int shell_set_channel_value(char **args){
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"set_channel_value\" SWITCH_NAME VALUE_FRM_0_TO_100 \n");
  } else {
    if (chdir(args[1]) != 1) {
      perror("shell: expected argument to \"set_channel_value\" SWITCH_NAME VALUE_FRM_0_TO_100 \n");
    }
  }
  return 1;
}

int shell_set_channel_name(char **args){
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected arguments to \"set_channel_name\" CONTROLLER_NAME switch_NUMBER \"SWITCH_NAME\" \n");
  } else {
    if (chdir(args[1]) != 2) {
      perror("shell: expected arguments to \"set_channel_name\" CONTROLLER_NAME switch_NUMBER \"SWITCH_NAME\" \n");
    }
  }
  return 1;
}

int shell_set_name(char **args){
	int ret;
	if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"set_name\" CONTRLLER_NAME\n");
  } else {
    if (chdir(args[1]) != 0) {
		int i = 0; 
		while(args[1][i] != NULL){
			i++;
		}
		ret = write(tty_fd, args[1], i+1);
		if(ret < 0) printf("Errore nella write\n");
		
    }
  }
  return 1;
}


int shell_help(char **args)
{
  int i;
  printf("Smart Home shell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in functions:\n");

  for (i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
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
	sleep(1);
	readFromArduino();
    printf("smart_house ");
    line = shell_read_line();
    args = shell_split_line(line);
    status = shell_execute(args);

    free(line);
    free(args);
  } while (status);
}

void readFromArduino(){
	if(read(tty_fd,&c,1)>0){
		printf("\nARDUINO: ");
		printf("%c", c);
		while(read(tty_fd,&c,1)>0){
			printf("%c", c);
		}
		printf("\n");
	}
}

int main(int argc, char **argv){
	
	
    tcgetattr(STDOUT_FILENO,&old_stdio);

    //printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
   /* memset(&stdio,0,sizeof(stdio));
	stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
        stdio.c_cc[VMIN]=1;
        stdio.c_cc[VTIME]=0;
        tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking*/

        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
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
