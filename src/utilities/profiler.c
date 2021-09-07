/*
Enables profiling using the -finstrument-functions function from gcc
Example can be found here:
http://hacktalks.blogspot.com/2013/08/gcc-instrument-functions.html
*/

// #include "os.h"

#include "../../include/types.h"
#include "../../include/printf.h"
#include "../../include/profiler.h"
// #include "log.h"

// #ifdef PROFILE_ENABLE

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));

struct LogMgr {
    uint64_t w_index;
    uint64_t r_intdex;
};

extern uint64_t pLogMem;
extern uint64_t pLogMemMgr;
extern uint64_t log_lock_mem;

__attribute__((no_instrument_function))
uint64_t rdinstrret(){
    register uint64_t cycle asm("x1");
    asm __volatile__ ("rdinstret x1");
    return cycle;
}
__attribute__((no_instrument_function))
uint64_t rdcycle(){
    register uint64_t cycle asm("x1");
    asm __volatile__ ("rdcycle x1");
    return cycle;
}

// struct ProfilerLogEntry* pLogBuffer = (struct ProfilerLogEntry*)&pLogMem;

#define LOG_BUFFER_SZ 512
struct ProfilerLogEntry _tmp[3][LOG_BUFFER_SZ];
int current_logging_buf_idx = 0;
// struct ProfilerLogEntry* current_logging_buffer = _tmp[0];
// struct ProfilerLogEntry* current_logging_buffer = (struct ProfilerLogEntry*)&pLogMem;
uint32_t sizeof_logging_buffer = LOG_BUFFER_SZ;
uint32_t log_idx = 0;
// struct LogMgr* pLogMgr = (struct LogMgr*)&pLogMemMgr;

//currently not implemented for kernel mode yet

// void record_profiling_data(struct ProfilerLogEntry* entries, size_t count){
//     register unsigned int id asm("x17") = 91;
//     register struct ProfilerLogEntry* _entries asm("x11") = entries;
//     register size_t _count asm("x12")=count;

//     asm __volatile__("ecall");
// }

/*
This function is called when the logging buffer is full.
This function needs to do something to save all the log entries
 and return with a new empty logging buffer
*/
void handle_full_logging_buffer(){
    // LOG_TRACE("Flipping log buffer\n");

    // struct ProfilerLogEntry* old_log_buffer = current_logging_buffer;
    // size_t old_size = log_idx;

    // //flip logging buffer
    // current_logging_buf_idx += 1;
    // current_logging_buffer = _tmp[current_logging_buf_idx];
    log_idx = 0;
    
    // sizeof_logging_buffer = LOG_BUFFER_SZ;
    
    // sendIpiNotification(0b01);
    
    // get_lock((uint64_t)&log_lock_mem);
    // pLogMgr->w_index += 1;

    // if (pLogMgr->w_index == 3){
    //     pLogMgr->w_index = 0;
    // }

    // current_logging_buffer = (struct ProfilerLogEntry*)&pLogMem + (pLogMgr->w_index * 512);
    
    // release_lock((uint64_t)&log_lock_mem);
}

/*
we need to disable profiling within the profile function
otherwise we would cause a recursive stack overflow
*/
// bool disable_profiling = false;

/*
This function is called everytime a function is entered
*/
void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
    // if (disable_profiling)
    //     return;

    // disable_profiling = true;

    // #ifdef KERNEL_MODE
    //     rs64InterruptDisable();
    // #endif
    // printf("log in\n");
    if(log_idx == sizeof_logging_buffer){
        log_idx = 0;

        handle_full_logging_buffer();
        
        // sw_assert1(log_idx < sizeof_logging_buffer, "Out of logging buffer memory");
    }

    // struct ProfilerLogEntry* curEntry = &current_logging_buffer[log_idx];

    // curEntry->log_type = LOG_TYPE_FN_ENTRY;
    // curEntry->caller_addr = (uint64_t)call_site;
    // curEntry->function_addr = (uint64_t)this_fn;
    printf("----------------log enter----------------\n");
    printf("Callder address is 0x%x%x\n", (uint64_t)call_site >> 32, (uint64_t)call_site);
    printf("Current function address is 0x%x%x\n", (uint64_t)this_fn >> 32, (uint64_t)this_fn);
    printf("Current Cycle is 0x%x\n", rdcycle());
    printf("Current instruction count is 0x%x\n", rdinstrret());
    // curEntry->instr_count = rdinstrret();

    // log_idx++;

    // #ifdef KERNEL_MODE
    //     rs64InterruptEnable();
    // #endif

    // disable_profiling = false;
}

/*
This function is called everytime a function is exited
*/
void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
    // if (disable_profiling)
    //     return;

    // disable_profiling = true;

    // uint64_t instr_count = rdinstrret();
    // uint64_t cycle = rdcycle(); //store cycle first, for more accurate data

    // #ifdef KERNEL_MODE
    //     rs64InterruptDisable();
    // #endif

    // if(log_idx == sizeof_logging_buffer){
        
    //     handle_full_logging_buffer();
    //     sw_assert1(log_idx<sizeof_logging_buffer, "Out of logging buffer memory");
    // }

    // struct ProfilerLogEntry* curEntry = &current_logging_buffer[log_idx];

    // curEntry->log_type = LOG_TYPE_FN_EXIT;
    // curEntry->caller_addr = (uint64_t)call_site;
    // curEntry->function_addr = (uint64_t)this_fn;
    // curEntry->cycle = cycle;
    // curEntry->instr_count = instr_count;

    // LOG_INFO("~~~~~~~~~~~~~~~~~~\n");
    // LOG_INFO("Log Type : %d\n", curEntry->log_type);
    // LOG_INFO("Function Caller : 0x%x\n", curEntry->caller_addr);
    // LOG_INFO("Function Address: 0x%x\n", curEntry->function_addr);
    // LOG_INFO("Cycle Count : 0x%x\n", curEntry->cycle);
    // LOG_INFO("Iinstruction Count : 0x%x\n", curEntry->instr_count);
    // LOG_INFO("~~~~~~~~~~~~~~~~~~\n");
    // sendIpiNotification(0b01);
    printf("----------------log exit----------------\n");
    printf("Callder address is 0x%x%x\n", (uint64_t)call_site >> 32, (uint64_t)call_site);
    printf("Current function address is 0x%x%x\n", (uint64_t)this_fn >> 32, (uint64_t)this_fn);
    printf("Current Cycle is 0x%x\n", rdcycle());
    printf("Current instruction count is 0x%x\n", rdinstrret());
    log_idx++;
    
    // #ifdef KERNEL_MODE
    //     rs64InterruptEnable();
    // #endif

    // disable_profiling = false;
}
// #endif //PROFILE_ENABLE

// #ifndef KERNEL_MODE
// void display_profiling_data(int64_t thread_id, uint64_t log_idx){
//     register unsigned int id asm("x17") = 92;
//     register int64_t _thread_id asm("x11") = thread_id;
//     register uint64_t _log_idx asm("x12") = log_idx;

//     asm __volatile__("ecall");
// }
// #endif

