#include "syscall.h"
int main() {
    // Writes "Hello World" to stdout
    Write("Hello World\n", 12, 1); 
    Halt();
}
