// test/read.c
#include "syscall.h"
int main() {
    char buffer[50];
    int bytesRead = Read(buffer, 17, 0); // Read from stdin (0)
    if (bytesRead > 0) {
        Write("Received: ", 10, 1);
        Write(buffer, bytesRead, 1);
    }
    Exit(0);
}
