# Task 1) Abs System Call Implementation in NachOS

## Overview
Implemented the `Abs` system call in NachOS that computes the absolute value of an integer passed from a user program running inside the NachOS MIPS simulator.

---

## Details

**Name:** Srikar Vilas Donur  
**Roll No:** CS23B049  

---

## Files Modified

| File | Change |
|------|--------|
| `code/userprog/syscall.h` | Added SC_Abs number and declaration |
| `code/userprog/ksyscall.h` | Added SysAbs kernel implementation |
| `code/userprog/exception.cc` | Added handler function and switch case |
| `code/test/start.S` | Added MIPS assembly stub |
| `code/test/Makefile` | Added build rules for abs |
| `code/test/abs.c` | New test program |

---

## Implementation

### 1. `code/userprog/syscall.h`
```c
#define SC_Abs 55

int Abs(int val);
```

### 2. `code/userprog/ksyscall.h`
```c
#include <stdint.h>

int SysAbs(int val) { return val < 0 ? -val : val; }
```

### 3. `code/userprog/exception.cc`
```cpp
void handle_SC_Abs() {
    int val = (int)kernel->machine->ReadRegister(4);
    int result = SysAbs(val);
    kernel->machine->WriteRegister(2, result);
    return move_program_counter();
}
```
In the switch-case inside `ExceptionHandler()`:
```cpp
case SC_Abs:
    return handle_SC_Abs();
```

### 4. `code/test/start.S`
```asm
	.globl Abs
	.ent	Abs
Abs:
	addiu $2,$0,SC_Abs
	syscall
	j	$31
	.end Abs
```

### 5. `code/test/Makefile`
Added `abs` to `PROGRAMS`, then:
```makefile
abs.o: abs.c
	$(CC) $(CFLAGS) -c abs.c

abs: abs.o start.o
	$(LD) $(LDFLAGS) start.o abs.o -o abs.coff
	$(COFF2NOFF) abs.coff abs
```

### 6. `code/test/abs.c`
```c
#include "syscall.h"

int main() {
    int result = Abs(-49);
    PrintNum(result);
    Halt();
}
```

---

## Output
```
49Machine halting!
```

`Abs(-49)` returns `49` — system call works end-to-end.

---

# Task2) Priority Scheduler Implementation in NachOS

## Overview
Replaced the default FIFO scheduler with a priority-based scheduler.
Threads with higher priority values run first.

---

## Files Modified

| File | Change |
|------|--------|
| `code/threads/thread.h` | Added `priority` field, `getPriority()`, `setPriority()` |
| `code/threads/thread.cc` | Initialized `priority = 0` in constructor |
| `code/threads/scheduler.h` | Changed `List` to `SortedList` |
| `code/threads/scheduler.cc` | Added `PriorityCompare`, used `SortedList`, changed `Append` to `Insert` |
| `code/threads/kernel.cc` | Added `PriorityTestThread` and test in `ThreadSelfTest()` |

---

## Implementation

### 1. `code/threads/thread.h`
```cpp
int priority;

void setPriority(int p) { priority = p; }
int getPriority() { return priority; }
```

### 2. `code/threads/thread.cc`
```cpp
priority = 0;
```

### 3. `code/threads/scheduler.h`
```cpp
SortedList<Thread*>* readyList;
```

### 4. `code/threads/scheduler.cc`
```cpp
static int PriorityCompare(Thread* a, Thread* b) {
    if (a->getPriority() > b->getPriority()) return -1;
    if (a->getPriority() < b->getPriority()) return 1;
    return 0;
}
```

```cpp
readyList = new SortedList<Thread*>(PriorityCompare);
```

```cpp
readyList->Insert(thread);
```

### 5. `code/threads/kernel.cc`
```cpp
static void PriorityTestThread(int which) {
    printf("*** thread priority %d running\n", which);
}

printf("\n--- Priority Scheduler Test ---\n");

IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);

Thread* t1 = new Thread("low-priority");
t1->setPriority(1);
t1->Fork((VoidFunctionPtr)PriorityTestThread, (void*)1);

Thread* t2 = new Thread("medium-priority");
t2->setPriority(5);
t2->Fork((VoidFunctionPtr)PriorityTestThread, (void*)5);

Thread* t3 = new Thread("high-priority");
t3->setPriority(10);
t3->Fork((VoidFunctionPtr)PriorityTestThread, (void*)10);

kernel->interrupt->SetLevel(oldLevel);
currentThread->Yield();

printf("--- Priority Scheduler Test Done ---\n");
```

---

## Output
```
--- Priority Scheduler Test ---
*** thread priority 10 running
*** thread priority 5 running
*** thread priority 1 running
--- Priority Scheduler Test Done ---
```

---

# Task 3: Sleep System Call Implementation in NachOS

## Overview
Implemented a `Sleep(int ticks)` system call that suspends the calling thread for a specified number of timer ticks. The thread blocks and is woken up by the timer interrupt handler.

---

## Files Modified

| File | Change |
|------|--------|
| `code/userprog/syscall.h` | Added `SC_Sleep 56`, `SC_GetTicks 57` |
| `code/threads/alarm.h` | Added `SleepEntry` struct and sleepList |
| `code/threads/alarm.cc` | Implemented `WaitUntil()` and `CallBack()` |
| `code/userprog/ksyscall.h` | Added `SysSleep()` and `SysGetTicks()` |
| `code/userprog/exception.cc` | Added handlers |
| `code/test/start.S` | Added syscall stubs |
| `code/test/Makefile` | Added sleep_test |
| `code/test/sleep_test.c` | New test program |

---

## Implementation

### 1. `code/userprog/syscall.h`
```c
#define SC_Sleep    56
#define SC_GetTicks 57

void Sleep(int when);
int  GetTicks();
```

### 2. `code/userprog/ksyscall.h`
```cpp
void SysSleep(int ticks) {
    if (ticks <= 0) {
        kernel->currentThread->Yield();
        return;
    }
    kernel->alarm->WaitUntil(ticks);
}

int SysGetTicks() {
    return kernel->stats->totalTicks;
}
```

### 3. `code/userprog/exception.cc`
```cpp
void handle_SC_Sleep() {
    int ticks = (int)kernel->machine->ReadRegister(4);
    SysSleep(ticks);
    kernel->machine->WriteRegister(2, 0);
    return move_program_counter();
}

void handle_SC_GetTicks() {
    kernel->machine->WriteRegister(2, SysGetTicks());
    return move_program_counter();
}
```

---

### 4. `code/test/sleep_test.c`
```c
#include "syscall.h"

int main() {
    int before, after;

    before = GetTicks();
    Sleep(2000);
    after = GetTicks();

    PrintString("Requested ticks : 2000\n");
    PrintString("Slept for ticks : ");
    PrintNum(after - before);
    PrintString("\n");

    Halt();
}
```

---

## Output
```
Requested ticks : 2000
Slept for ticks : 2074
Machine halting!
```

---

## Verification

| Metric | Value | Meaning |
|--------|-------|---------|
| Requested sleep | 2000 ticks | Argument passed |
| Actual sleep | 2074 ticks | Wake after interrupt |
| Overshoot | 74 ticks | < 100 ticks ✅ |

---

## How to Run
```bash
cd nachos-project-master/code/test
make

cd ../build.linux
make

./nachos -x ../test/abs
./nachos -K
./nachos -x ../test/sleep_test
```# OSLAB
