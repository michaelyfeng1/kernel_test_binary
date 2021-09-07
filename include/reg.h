#ifndef _REG_H_
#define _REG_H_

void writeScratch(uint64_t value);
void writeStvec(uint64_t value);
void writeSstatus(uint64_t value);
void readSstatus(uint64_t* volatile retValue);
void readSie(uint64_t* volatile retValue);
void writeSie(uint64_t value);
void writeSip(uint64_t value);
void readSip(uint64_t* volatile retValue);
void readCcycle(uint64_t* volatile retvalue);

#endif