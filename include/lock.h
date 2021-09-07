#ifndef _LOCK_H_
#define _LOCK_H_

extern void __getLock();
extern void __releaseLock();
extern void __getCbLock();
extern void __releaseCbLock();

extern void __CommandQueueMemStart();
extern void __LockMemStart();
extern void __PrinfBuffStart();
extern void __HartInfoArray();

extern uint64_t __consoleLock;
extern uint64_t __queueMemLock;

#define pCommandQueueMem __CommandQueueMemStart
#define pLockMemStart __LockMemStart
#define ConsoleLock __consoleLock
#define QueueMemLock __queueMemLock

#define acquireLock(...) __getLock()
#define releaseLock(...) __releaseLock()
#define acquireCbLock(...) __getCbLock()
#define releaseCbLock(...) __releaseCbLock()

#define wfi(...) ({ \
    __asm__ volatile ("wfi");   \
})

#endif
