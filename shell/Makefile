all: shell

clean:
	@rm -rf *.o
	@rm -rf shell

shell: startArduinoServer.o httpd.o shell_3.o rw.o
	gcc -o shell $^

startArduinoServer.o: startArduinoServer.c httpd.h
	gcc -c -o startArduinoServer.o startArduinoServer.c

httpd.o: httpd.c httpd.h rw.o
	gcc -c -o httpd.o httpd.c
	
shell_3.o: shell_3.c server.h rw.o
	gcc -c -o shell_3.o shell_3.c

rw.o: rw.c common.h
	gcc -c -o rw.o rw.c

