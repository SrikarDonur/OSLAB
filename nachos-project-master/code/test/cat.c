#include "syscall.h"
int main() {
    char buffer[20];
    int i;
    // Reads from stdin (0)
    Read(buffer, 12, 0); 
    // Writes to stdout (1)
    Write("Received: ", 10, 1);
    Write(buffer, 12, 1);
    Halt();
}
