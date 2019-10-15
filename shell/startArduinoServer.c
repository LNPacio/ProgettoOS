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
        printf("<html>");
		printf("<br>");
		printf("<br>");
        printf("<form action=\"/resp\" method=\"post\">");
        printf("<div><button name=\"\" value=\"\">Inizia</button><div>");
        printf("</form></html>");
    }
    
     ROUTE_GET("/form")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("<form action=\"/resp\" method=\"post\">");
        printf("<br> Seleziona Comando:");
        printf("<select name=\"command\">");
        printf("<option value=\"set_name\">Imposta il nome</option>");
        printf("<option value=\"get_pwm\">Leggi valori PWM</option>");
        printf("<option value=\"get_adc\">Leggi valori ADC</option>");
        printf("<option value=\"set_pwm_name\">Imposta nomi PWM</option>");
        printf("<option value=\"set_pwm_value\">Imposta valori PWM</option>");
        printf("<option value=\"set_pwm_adc\">Imposta nomi ADC</option>");
        printf("<option value=\"quit\">Chiudi</option>");
        printf("</select><br>");
        printf("<br>Nome<input type=\"text\" name=\"nome\"><br>");
        printf("Seleziona Canale:<select name=\"switch\">");
        printf("<option value=\"\">---</option>");
        printf("<option value=\"switch_0\">0</option>");
        printf("<option value=\"switch_1\">1</option>");
        printf("<option value=\"switch_2\">2</option>");
        printf("<option value=\"switch_3\">3</option>");
        printf("<option value=\"switch_4\">4</option>");
        printf("<option value=\"switch_5\">5</option>");
        printf("<option value=\"switch_6\">6</option>");
        printf("<option value=\"switch_7\">7</option>");
        printf("</select><br>");
        printf("<br>Valore o Nome<input type=\"text\" name=\"nome\"><br>");
        printf("<button name=\"\" value=\"\">INVIO</button>");
        printf("</form>");
    }

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("Fetch the data using `payload` variable.");
    }
    
    ROUTE_POST("/resp")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("<html>");
        printf("ARDUINO: <textarea rows=\"11\" cols=\"50\"> %s</textarea><br>", payload_read);
        printf("<form action=\"/form\" method=\"get\">");
        printf("<button name=\"\" value=\"\">Torna Indietro</button>");
        printf("</form></html>");
    }
  
    ROUTE_END()
}
