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
        printf("<form action=\"/form\" method=\"get\">");
        printf("<div><button name=\"\" value=\"\">Inizia</button><div>");
        printf("</form></html>");
    }

     ROUTE_GET("/form")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("<!doctype html><html><head> <meta charset=\"UTF-8\"> <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js\"></script></head><body> <script type=\"text/javascript\">window.onload=function(){$(\"#sw\").prop(\"disabled\", true); $(\"#vn\").prop(\"disabled\", true); var selezione=\"set_name\"; an();}; function co(){if ($(\"#nome\").val()==\"\"){alert(\"inserire nome\"); return false;}selezione=$(\"#command\").val(); if ((selezione==\"set_pwm_name\" || selezione==\"set_adc_name\" || selezione==\"set_pwm_value\")&& $(\"#sw\").val()==\"\"){alert(\"inserisci lo switch\");return false;}else if ((selezione==\"set_pwm_name\" || selezione==\"set_adc_name\" )&& $(\"#vn\").val()==\"\"){alert(\"inserisci il nome\");return false;}; return true;}; function an(){selezione=$(\"#command\").val(); if (selezione==\"set_pwm_name\" || selezione==\"set_adc_name\" || selezione==\"set_pwm_value\"){$(\"#sw\").prop(\"disabled\", false); $(\"#vn\").prop(\"disabled\", false); $(\"#sw\").css(\"visibility\", \"visible\"); $(\"#vn\").css(\"visibility\", \"visible\"); $(\"#a\").css(\"visibility\", \"visible\"); $(\"#b\").css(\"visibility\", \"visible\"); $(\"#num\").prop(\"disabled\", true); $(\"#num\").css(\"visibility\", \"hidden\"); if (selezione==\"set_pwm_name\" || selezione==\"set_adc_name\"){$(\"#b\").html(\"Nome \"); $(\"#vn\").prop(\"readonly\", false); $(\"#num\").prop(\"disabled\", true); $(\"#num\").css(\"visibility\", \"hidden\"); $(\"#vn\").val('');}else{$(\"#b\").html(\"Valore \"); $(\"#vn\").prop(\"readonly\", true); $(\"#num\").prop(\"disabled\", false); $(\"#num\").css(\"visibility\", \"visible\"); $(\"#vn\").val('0');}}else{$(\"#sw\").prop(\"disabled\", true); $(\"#vn\").prop(\"disabled\", true); $(\"#sw\").css(\"visibility\", \"hidden\"); $(\"#vn\").css(\"visibility\", \"hidden\"); $(\"#a\").css(\"visibility\", \"hidden\"); $(\"#b\").css(\"visibility\", \"hidden\"); $(\"#num\").prop(\"disabled\", true); $(\"#num\").css(\"visibility\", \"hidden\");}}; function agg(){if ($(\"#num\").val().length==1){$(\"#vn\").val('00'+$(\"#num\").val());}else if ($(\"#num\").val().length==2){$(\"#vn\").val('0'+$(\"#num\").val());}else $(\"#vn\").val($(\"#num\").val());}; </script> <form action=\"/resp\" method=\"post\"> <br>Seleziona Comando: <select name=\"command\" id=\"command\" onchange=\"an()\"> <option value=\"set_name\">Imposta il nome</option> <option value=\"get_pwm\">Leggi valori PWM</option> <option value=\"get_adc\">Leggi valori ADC</option> <option value=\"set_pwm_name\">Imposta nomi PWM</option> <option value=\"set_pwm_value\">Imposta valori PWM</option> <option value=\"set_adc_name\">Imposta nomi ADC</option> <option value=\"quit\">Chiudi</option> </select> <br><br>Nome <input type=\"text\" id=\"nome\" name=\"nome\"> <br><a id=\"a\">Seleziona Canale:</a> <select name=\"switch\" id=\"sw\"> <option value=\"\">---</option> <option value=\"switch_0\">0</option> <option value=\"switch_1\">1</option> <option value=\"switch_2\">2</option> <option value=\"switch_3\">3</option> <option value=\"switch_4\">4</option> <option value=\"switch_5\">5</option> <option value=\"switch_6\">6</option> <option value=\"switch_7\">7</option> </select> <br><br><a id=\"b\">Valore o Nome</a> <input type=\"text\" name=\"nome\" id=\"vn\"> <br><input type=\"range\" id=\"num\" onchange=\"agg()\" min=\"0\" max=\"255\" value=\"0\"> <br><br><button name=\"\" value=\"\" id=\"bo\" onclick=\"return co()\">INVIO</button> </form></body></html>");
        /*printf("<form action=\"/resp\" method=\"post\">");
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
        printf("</form>");*/
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
