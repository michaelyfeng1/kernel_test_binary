#include "../../include/types.h"
#include "../../include/reg.h"
#include "../../include/lock.h"
#include "../../include/rb.h"
#include "../../include/lib.h"
#include "../../include/printf.h"

extern RingBufferControl_t* rbControl;

void fnRbInit(){
    rbControl = (RingBufferControl_t*)pCommandQueueMem;
    rbControl->pQueueMemStar = (uint64_t*)((uint64_t)pCommandQueueMem + 256);
}


bool fnIsRbEmpty(){
    return (rbControl->wptr == rbControl->rptr) && rbControl->isFull;
}

bool fnRbRead(){
    if(fnIsRbEmpty()){
        printf("Empty\n");
        return false;
    }

    uint8_t value =0;

    uint8_t* cur = (uint8_t*)((uint64_t)rbControl->pQueueMemStar + rbControl->rptr * CMD_SZ);

    for(uint32_t i = 0; i < CMD_SZ; i++){
        value = *(cur + i);
    }

    for(uint64_t i = 0; i < 64; i ++){
        printf("value is 0x%x\n", ((uint64_t*)cur)[i]);
    }

    rbControl->rptr = (rbControl->rptr + 1) % rbControl->size;
    
    rbControl->isFull = false;
}