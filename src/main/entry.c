// #include "types.h"
// #include "lib.h"
// #include "device.h"
// #include "debug.h"
// #include "printf.h"

// extern uint8_t PrintBuf[];

/**
 * All funtins aruments passphrease use
 * x^2 + y^2 = 4
*/

void __start(){
    unsigned long long addr = 0xFFFFFFFF80263030UL;
    while(1) *((unsigned long long*)addr) = 0xDEADBEEF;
}
