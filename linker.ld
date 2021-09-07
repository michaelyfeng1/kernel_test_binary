OUTPUT_ARCH(riscv)

ENTRY(_start)

BASE_ADDRESS = 0xf80200000;

SECTIONS
{
    __CommandQueueMemStart = .;
    PROVIDE(__commandBuffMem = .);
    
        
    . += 0x1000;
    __CommandQueueMemEnd = .;

    __LockMemStart = .;
    PROVIDE(__consoleLock = .);
    . += 8;
    PROVIDE(__queueMemLock = .);
    . += 8;
    __LockMemEnd = .;
    
    __PrinfBuffStart = .;
    . += 2048;
    . = ALIGN(4096);
    __HartInfoArray = .;
        

    . = BASE_ADDRESS;
    __BinStartAddr = .;
    __code_start = .;
    .text : {
        *(.text.init)
        *(.text) 
        *(.text.*)
    } 
    __code_end = .;
    . = ALIGN(4096);
    __code_end_aligned = .;

    . += 0x1000;
    _rodata_start = .;
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    . = ALIGN(4096);
    .data : {
        *(.data)
        *(.data.*)
    }

    . = ALIGN(4096);
    _bss_start = .;

    .bss : {
        *(.sbss)
        *(.bss) 
        *(.bss.*)
    }
    _bss_end = .;
    . = ALIGN(4096);
    __BinEndAddr = .;
}
