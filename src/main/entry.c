#include "../../include/types.h"
#include "../../include/lib.h"
#include "../../include/debug.h"
#include "../../include/trap.h"
#include "../../include/rb.h"
#include "../../include/printf.h"
#include "../../include/lock.h"
#include "../../include/ipimem.h"

extern uint8_t PrintBuf[];
RingBufferControl_t* rbControl;
/**
 * All funtins aruments passphrease use
 * x^2 + y^2 = 4
*/

void _start(){
    fnStdoutInit(PrintBuf, fnPutCharWrap);
    fnRbInit();
    printf("Hello World\n");
    fnTrapInit();
    unsigned int i = 0, j = 0;

    i = 10;
    j = 20;

    // register unsigned long long eid __asm__("x17") = 1;
    // register unsigned long long __ch __asm__("x10") = 80; 
    // __asm__ volatile (
    //     "ecall"
    // );
    // acquireCbLock();
    while(1){
        // __asm__ ("wfi");
        i = 0;
        acquireCbLock();
        // printf(" 1");
        while (i < 100000000) i = i + 1;
        fnDebugPrint();
        releaseCbLock();

    }
}

void fnReceiveCommand(){
    uint64_t* pCmdBuffer = (uint64_t*)pCommandQueueMem;

    for(int i = 0; i < 50; i++){
        printf("Value read back is 0x%x\n", *(pCmdBuffer + 1));
    }
}

// void _start(){
//     unsigned long long *ptr = 0x0;
//     while(1){
//     // register unsigned long long eid __asm__("x17") = 1;
//     // register unsigned long long __ch __asm__("x10") = 80; 
//     // __asm__ volatile (
//     //     "ecall"
//     // );
//         *(ptr) = 0xBEEFBEEF;
//         *(ptr + 512) = 0xDEADBEEF;

//         *(ptr + 256) = 0xBEEFBEEF;

//         *(ptr + 1024) = 0xBEEFBEEF;
//     }
//     // unsigned long long addr = 0xFFFFFFFF80263030UL;
//     // while(1) *((unsigned long long*)addr) = 0xDEADBEEF;
// }
