#include "syscall.h"

int main() {
    int result = Abs(-49);
    PrintNum(result);   // should print 49
    Halt();
}
