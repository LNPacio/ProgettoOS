#include "httpd.h"

int main(int c, char** v)
{
    serve_forever("12913");
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
        printf("<b>Bella Raga </b>\n" "<button type=\"button\" method=\"ROUTE_POST(\"/hello\")\">Click Me!</button> ");
    }

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("Fetch the data using `payload` variable.");
    }
    
    ROUTE_POST("/hello")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("Fetch the data using `payload` variable.");
    }
  
    ROUTE_END()
}
