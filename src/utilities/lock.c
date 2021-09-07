
#include "../../include/types.h"
// #include "device.h"
#include "../../include/lock.h"
#include "../../include/printf.h"

extern uint64_t _lockMem; 

bool __acquireLock(){
    register unsigned int t0 asm("t0") = 0x1;
    register unsigned long long a0 asm("a0") = (uint64_t)&ConsoleLock;

    __asm__ volatile(
        "amoswap.w.aq t0, t0, (a0)\n \t");

    return (t0 == 1);
}

void __getLock(){
    while (__acquireLock());
}

void __releaseLock(){
    
    register unsigned long long a0 asm("a0") = (uint64_t)&ConsoleLock;
    __asm__ volatile(
        "amoswap.w.rl x0, x0, (a0)\n \t"
        ::"r"((uint64_t)&ConsoleLock));
}

bool __acquireCbLock(){
    register unsigned int t0 asm("t0") = 0x1;
    register unsigned long long a0 asm("a0") = (uint64_t)&QueueMemLock;

    __asm__ volatile(
        "amoswap.w.aq t0, t0, (a0)\n \t");

    return (t0 == 1);
}

void __getCbLock(){
    while (__acquireCbLock());
}

void __releaseCbLock(){
    register unsigned long long a0 asm("a0") = (uint64_t)&QueueMemLock;
    
    __asm__ volatile(
        "amoswap.w.rl x0, x0, (a0)\n \t"
        ::"r"((uint64_t)&QueueMemLock));
}