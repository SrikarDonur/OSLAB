// test/write.c
#include "syscall.h"
int main() {
    Write("Hello from pipe!\n", 17, 1);
    Exit(0);
}
