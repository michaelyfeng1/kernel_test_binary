#include "../../include/types.h"
#include "../../include/reg.h"
#include "../../include/trap.h"
#include "../../include/reg.h"
#include "../../include/printf.h"
#include "../../include/rb.h"
#include "../../include/handlers.h"

static inline void _clearSuperVisorSoftwareIntr(){
    uint64_t sip;

    readSip(&sip);
    // rs64RegReadSip(&sip);
    sip &= ~(1UL << 1UL);//clear ssip
    // rs64RegWriteSip(sip);
    writeSip(sip);
}

void _handlerEbp(void* _tf, ...){
    printf("hit");
    ((Trapframe_t*)_tf)->sSepc += 2;
}

void _handlerSupervisorTimer(void* _tf, ...){
    // readCcyle(TimerControl.cycle);
 
    // if (TimerControl.tick == 100) TimerControl.tick = 0;
  
    // TimerControl.tick += 1;
    
    // fnSetTimerInterval();
}

void _handlerSupervisorSoftInt(void* _tf, ...){
    _clearSuperVisorSoftwareIntr();
    printf("Receive Software Interrupt, and clear\n");
extern void fnReceiveCommand();    
    // fnReceiveCommand();
    // fnRbRead();
}
