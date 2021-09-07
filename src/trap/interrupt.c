#include "../../include/types.h"
#include "../../include/trap.h"
#include "../../include/reg.h"
#include "../../include/handlers.h"
#include "../../include/printf.h"

extern void __trap_enter();

_intrHandler TrapVecTbl[InterruptUnknown];
_intrHandler ExcpVecTbl[ExceptionUnknown];

void fnBuildIntrVetTbl(){
    ExcpVecTbl[Breakpoint] = _handlerEbp;
    TrapVecTbl[SupervisorTimer] = _handlerSupervisorTimer;
    TrapVecTbl[SupervisorSoft] = _handlerSupervisorSoftInt;
}

void fnTrapInit(){
    fnBuildIntrVetTbl();

    writeScratch(0);
    
    uint64_t value = (uint64_t) __trap_enter | (uint64_t)DirectMode;

    writeStvec(value);

    value = 0;
    readSstatus(&value);
    value |= (1 << 1);
    writeSstatus(value);

    value = 0;
    readSie(&value);
    value |= (1 << 1);
    writeSie(value);
}

void fnDispatchInterrupt(Trapframe_t* _tf){
    switch(fnTrapTypeParse(_tf->sScaues)){
        case TrapException:
        {
            fnDispatchSoftInterrupt(_tf);
            break;
        }
        case TrapInterrupt:
        {
            fnDispatchExtrInterrupt(_tf);
            break;
        }
        default:
        {
            break;
        }
    }
}

// void _tfDump(Trapframe_t* __tf){
//     if (__tf != _nullptr){
//         for(uint8_t i = 0; i < REG_CONTEXT_NUM; i++){
//             printf("Reg Value: 0x%x\n", __tf->gpr[i]);
//         }

//         printf("Reg Value: 0x%x\n", __tf->sScaues);
//         printf("Reg Value: 0x%x\n", __tf->sSepc);
//         printf("Reg Value: 0x%x\n", __tf->sStval);

//     }
// }

uint8_t fnTrapTypeParse(uint64_t _sscauseValue){
    uint64_t trapCode = _sscauseValue & (1UL << 63);

    return trapCode >> 63;
}

void fnDispatchSoftInterrupt(Trapframe_t* _tf){
    ExcpVecTbl[_tf->sScaues & 0xF]((void*)_tf);
}

void fnDispatchExtrInterrupt(Trapframe_t* _tf){
    TrapVecTbl[_tf->sScaues]((void*)_tf);
}