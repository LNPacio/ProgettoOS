#include "httpd.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int startArduinoServer(pid_t papi)
{
    serve_forever("12913", papi);
    return 0;
}

void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("<b>Hello! You are using %s </b>", request_header("User-Agent"));
    }
    
    ROUTE_GET("/bella")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("<b>Bella Raga </b>\n" "<button type=\"button\" method=\"ROUTE_POST(\"/hello\")\">Click Me!</button> ");
    }
    
     ROUTE_GET("/form")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("<form action=\"\" method=\"post\">Nome<br><input type=\"text\" name=\"nome\"><br><button name=\"bottone\" value=\"helloMoto\">POST</button></form>");
    }

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("Fetch the data using `payload` variable.");
    }
    
    ROUTE_POST("/form")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %s bytes. \r\n", payload);
        printf("Fetch the data using `payload` variable.");
    }
  
    ROUTE_END()
}
