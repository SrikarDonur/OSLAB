#include "syscall.h"

#define stdin 0
#define stdout 1

int main() {
    int pid;
    int i, j;
    int status;

    PrintString("\nIn parent process ---------");

    pid = Exec("../test/add");   // Nachos filename

    if (pid < 0) {
        Write("Exec failed: ", 14, stdout);
        PrintNum(pid);
    } else {
        for (i = 0; i < 1000; i++) {
            for (j = 0; j < 2000; j++); 
                PrintString("\nParent process");
            
        }
        status = Join(pid);
        PrintString("\nChild exited with status: ");
        PrintNum(status);
    }

    return 0;
}

