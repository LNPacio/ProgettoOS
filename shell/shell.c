#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int shell_set_name(char **args);
int shell_set_channel_value(char **args);
int shell_set_channel_name(char **args);
int shell_query_channels(char **args);
int shell_help(char **args);
int shell_exit(char **args);


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
    fprintf(stderr, "shell: expected argument to \"set_channel_value\" SWITCH_NAME BOOLEAN_VALUE \n");
  } else {
    if (chdir(args[1]) != 1) {
      perror("shell: expected argument to \"set_channel_value\" SWITCH_NAME BOOLEAN_VALUE \n");
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
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"set_name\" CONTRLLER_NAME\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("too many arguments");
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
    printf("> ");
    line = shell_read_line();
    args = shell_split_line(line);
    status = shell_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv)
{
  shell_loop();


  return EXIT_SUCCESS;
}
