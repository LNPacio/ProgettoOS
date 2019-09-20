#include <stdio.h>
#include <stdlib.h>

void ifconf(){
  FILE *pp;
  pp = popen("/sbin/ifconfig", "r");
  if (pp != NULL) {
    while (1) {
      char *line;
      char buf[1000];
      line = fgets(buf, sizeof buf, pp);
      if (line == NULL) break;
      if (line[0] ==' ' && line[2] ==' ' && line[3] ==' ' && line[4] == ' ' && line[5] == ' ' && line[6] == ' ' && line[7] == ' ' && line[8] == 'i' && line[9] == 'n' && line[10] == 'e' && line[11] == 't' && line[12] ==' ')
      { printf("%s", line);}
    }
    pclose(pp);
  }
}

void sshinst(){
  FILE *pp;
  pp = popen("sudo apt install openssh-server", "r");
  if (pp != NULL) {
    while (1) {
      char *line;
      char buf[1000];
      line = fgets(buf, sizeof buf, pp);
      if (line == NULL) break;
      if (line[0] !=NULL)
      { printf("%s", line);}
    }
    pclose(pp);
  }
}

void startserver(){
  FILE *pp;
  pp = popen("service ssh start", "r");
  if (pp != NULL) {
    while (1) {
      char *line;
      char buf[1000];
      line = fgets(buf, sizeof buf, pp);
      if (line == NULL) break;
      if (line[0] !=NULL)
      { printf("%s", line);}
    }
    printf("started ssh connection on port 22 of your inet\n");
    ifconf();
    pclose(pp);
  }
}
void stopserver(){
  FILE *pp;
  pp = popen("service ssh stop", "r");
  if (pp != NULL) {
    while (1) {
      char *line;
      char buf[1000];
      line = fgets(buf, sizeof buf, pp);
      if (line == NULL) break;
      if (line[0] !=NULL)
      { printf("%s", line);}
    }
    pclose(pp);
  }
}
