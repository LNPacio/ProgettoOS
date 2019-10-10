#include <unistd.h>
#include <errno.h>
#include "common.h"


int readOneByOne(int fd, char* buf, char separator) {

    int ret;
    int bytes_read = 0;
    do {
        ret = read(fd, buf + bytes_read, 1);
        if (ret == -1 && errno == EINTR) continue;
        if (ret == -1) handle_error("Cannot read from FIFO");
        if (ret ==  0) handle_error("Process has closed the FIFO unexpectedly! Exiting...\n");
    } while(buf[bytes_read++] != separator);
    return bytes_read;
}

void writeMsg(int fd, char* buf, int size) {

    int ret;
    
    int bytes_sent = 0;
    while (bytes_sent < size) {
        ret = write(fd, buf + bytes_sent, size - bytes_sent);
        if (ret == -1 && errno == EINTR) continue;
        if (ret == -1) handle_error("Cannot write to FIFO");
        bytes_sent += ret;
    }
}
