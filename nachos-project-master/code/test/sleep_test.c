#include "syscall.h"

int main() {
    int before, after, slept;

    before = GetTicks();
    Sleep(2000);
    after = GetTicks();

    slept = after - before;

    PrintString("Requested ticks : 2000\n");
    PrintString("Slept for ticks : ");
    PrintNum(slept);
    PrintString("\n");

    Halt();
}
