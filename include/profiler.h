#pragma once
// #ifdef ENABLE_PROFILNG

#include "types.h"

// #ifdef PROFILE_ENABLE
enum ProfilerLogType {
    LOG_TYPE_FN_ENTRY = 1,
    LOG_TYPE_FN_EXIT = 2
};

struct ProfilerLogEntry{
    uint64_t cycle;
    uint64_t caller_addr;
    uint64_t function_addr;
    uint32_t log_type;
    uint64_t instr_count;
};
// #endif

//not implemented for kernel mode yet
#ifndef KERNEL_MODE
    #ifdef PROFILE_ENABLE
    /*
    Send profiler log data to OS
    */
    void record_profiling_data(struct ProfilerLogEntry* entries, size_t count);
    #endif

/*
Display a profiling report for thread_id
*/
void display_profiling_data(int64_t thread_id, uint64_t log_idx);
#endif
