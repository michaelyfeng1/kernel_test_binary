#include "../../include/types.h"
#include "../../include/reg.h"

void writeScratch(uint64_t value){
    __asm__ volatile (
        "csrw sscratch, %0"
        :
        : "r"(value)
        : "memory"
    );
}

void writeStvec(uint64_t value){
    __asm__ volatile (
        "csrw stvec, %0"
        :
        : "r"(value)
        : "memory"
    );
}

void writeSstatus(uint64_t value){
    __asm__ volatile (
        "csrw sstatus, %0"
        :
        : "r"(value)
        :"memory"
    );
}

void readSstatus(uint64_t* volatile retValue){
    __asm__ volatile (
        "csrr %0, sstatus"
        : "=r"(*retValue)
        :
        :"memory"
    );
}

void readSie(uint64_t* volatile retValue){
    __asm__ volatile (
        "csrr %0, sie"
        : "=r"(*retValue)
        :
        :"memory"
    );
}

void writeSie(uint64_t value){
    __asm__ volatile (
        "csrw sie, %0"
        :
        : "r"(value)
        : "memory"
    );
}

void writeSip(uint64_t value){
    __asm__ volatile (
        "csrw sip, %0"
        :
        : "r"(value)
        :"memory"
    );
}

void readSip(uint64_t* volatile retValue){
    __asm__ volatile (
        "csrr %0, sip"
        : "=r"(*retValue)
        :
        :"memory"
    );
}

void readCcycle(uint64_t* volatile retvalue){
    __asm__ volatile (
        // "csrrs %0, cycle, x0"
        "rdtime %0"
            : "=r"(*retvalue)
            :
            :"memory"
    );
}
