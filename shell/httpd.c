#include "httpd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"
#define CONNMAX 1000



static int listenfd, clients[CONNMAX];
static void error(char *);
static void startServer(const char *);
static void respond(int);

typedef struct { char *name, *value; } header_t;
static header_t reqhdr[17] = { {"\0", "\0"} };
static int clientfd;

static char *buf;
//fifo
int echo_fifo, client_fifo;
char bufFIFO[1024];
char* quit_command = QUIT_COMMAND;
size_t quit_command_len;

int readOneByOne(int fd, char* bufFIFO, char separator);
void writeMsg(int fd, char* bufFIFO, int size);

const char *ADD = "192.168.43.160";



void serve_forever(const char *PORT)
{
	int ret;
    quit_command_len = strlen(quit_command);


    

    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;
	clientaddr.sin_addr.s_addr = inet_addr(ADD);
    int slot=0;

    printf(
            "Server started %shttp://%s:%s%s\n",
            "\033[92m",ADD,PORT,"\033[0m"
            );

    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;
    startServer(PORT);

    // Ignore SIGCHLD to avoid zombie threads
    signal(SIGCHLD,SIG_IGN);

    // ACCEPT connections
    while (1)
    {
		echo_fifo = open(ECHO_FIFO_NAME, O_WRONLY);
		if(echo_fifo == -1) handle_error("Cannot open Echo FIFO for writing");
		client_fifo = open(CLNT_FIFO_NAME, O_RDONLY);
		if(client_fifo == -1) handle_error("Cannot open Client FIFO for reading");
		
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot]<0)
        {
            perror("accept() error");
        }
        else
        {
            if ( fork()==0 )
            {
                respond(slot);
                int ret = close(echo_fifo);
				if(ret) handle_error("Cannot close Echo FIFO");
				ret = close(client_fifo);
				if(ret) handle_error("Cannot close Client FIFO");
                exit(0);
                
            }

        }

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
    }
		ret = close(echo_fifo);
		if(ret) handle_error("Cannot close Echo FIFO");
		ret = close(client_fifo);
		if(ret) handle_error("Cannot close Client FIFO");
}

//start server
void startServer(const char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0)
    {
        perror ("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p!=NULL; p=p->ai_next)
    {
        int option = 1;
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}


// get request header
char *request_header(const char* name)
{
    header_t *h = reqhdr;
    while(h->name) {
        if (strcmp(h->name, name) == 0) return h->value;
        h++;
    }
    return NULL;
}

//client connection
void respond(int n)
{
    int rcvd, fd, bytes_read;
    char *ptr;

    buf = malloc(65535);
    rcvd=recv(clients[n], buf, 65535, 0);

    if (rcvd<0)    // receive error
        fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    // receive socket closed
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    // message received
    {
        buf[rcvd] = '\0';

        method = strtok(buf,  " \t\r\n");
        uri    = strtok(NULL, " \t");
        prot   = strtok(NULL, " \t\r\n");

        fprintf(stderr, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);

        if (qs = strchr(uri, '?'))
        {
            *qs++ = '\0'; //split URI
        } else {
            qs = uri - 1; //use an empty string
        }

        header_t *h = reqhdr;
        char *t, *t2;
        while(h < reqhdr+16) {
            char *k,*v,*t;
            k = strtok(NULL, "\r\n: \t"); if (!k) break;
            v = strtok(NULL, "\r\n");     while(*v && *v==' ') v++;
            h->name  = k;
            h->value = v;
            h++;
            fprintf(stderr, "[H] %s: %s\n", k, v);
            t = v + 1 + strlen(v);
            if (t[1] == '\r' && t[2] == '\n') break;
        }
        t = strtok(NULL, "\r\n"); // now the *t shall be the beginning of user payload
        t2 = request_header("Content-Length"); // and the related header if there is
        payload = t;
        payload_size = t2 ? atol(t2) : (rcvd-(t-buf));

      if(method[0] == 'P'){
			payload[payload_size]= '\n';
			sprintf(bufFIFO,payload,quit_command);
			writeMsg(echo_fifo, bufFIFO, strlen(bufFIFO));

			int bytes_read = readOneByOne(client_fifo, bufFIFO, '\r');
		  	bufFIFO[bytes_read] = '\0';
		  	sprintf(payload_read,bufFIFO,'\r');
		}

        // bind clientfd to stdout, making it easier to write
        clientfd = clients[n];
        dup2(clientfd, STDOUT_FILENO);
        close(clientfd);

        // call router
        route();


        // tidy up
        fflush(stdout);
        shutdown(STDOUT_FILENO, SHUT_WR);
        close(STDOUT_FILENO);
    }

    //Closing SOCKET
    shutdown(clientfd, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clientfd);
    clients[n]=-1;
}
