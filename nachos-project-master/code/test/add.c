/* add.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main() {
    // result = Add(42, 23);
    int result;
    int i, j;
    for (i = 0; i < 1000; i++) {
    	for (j = 0; j < 2000; j++);
	PrintString("Child process");
    }

    Halt();
    /* not reached */
}
