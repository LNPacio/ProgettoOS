all: shell startArduinoServer

clean:
	@rm -rf *.o
	@rm -rf shell startArduinoServer

shell:  shell_3.o rw.o
	gcc -w -o shell $^

startArduinoServer: startArduinoServer.c httpd.h httpd.o 
	gcc -w -o startArduinoServer httpd.o rw.o startArduinoServer.c

httpd.o: httpd.c httpd.h rw.o
	gcc -w -c -o httpd.o httpd.c
	
shell_3.o: shell_3.c server.h rw.o
	gcc -w -c -o shell_3.o shell_3.c

rw.o: rw.c common.h
	gcc -w -c -o rw.o rw.c

