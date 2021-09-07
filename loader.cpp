#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"


int main(void){
    FILE* elfHandle = fopen("testbench.elf", "rb");
    FILE* output = fopen("testdump.txt", "w");

    char* sectionNameArr;
    Elf64_Ehdr elfFileHeader = {0};

    uint64_t entryAddress = 0;

    fread(&elfFileHeader, sizeof(Elf64_Ehdr), 1, elfHandle);

    // sectionNameArr = (char*)malloc(elfFileHeader.e_shnum * 10);

    //Get section names
    unsigned int strTblIndex = elfFileHeader.e_shstrndx;
    unsigned long long symbolOffset = 0;
    // printf("String table index is 0x%x\n", strTblIndex);

    Elf64_Shdr elfStrTblSec = {0}, tmpSecHdr = { 0 };
    Elf64_Shdr symTab= {0}, shText = {0}, shIrq = {0}, roData = {0}, data = {0}, sdata = {0};
    unsigned int symbolSize;
    //Get Section Table
    fseek(elfHandle, (unsigned long)((Elf64_Shdr*)elfFileHeader.e_shoff + strTblIndex), SEEK_SET);
    fread(&elfStrTblSec, sizeof(Elf64_Shdr), 1, elfHandle);

    entryAddress = elfFileHeader.e_entry;

    printf("Entry Address is 0x%llx\n", entryAddress);

    // printf("0x%x\n",elfStrTblSec.sh_size);
    // printf("0x%x\n",elfStrTblSec.sh_offset);

    sectionNameArr = (char*)malloc((elfStrTblSec.sh_size + 1));
    fseek(elfHandle, (elfStrTblSec.sh_offset), SEEK_SET);
    fread(sectionNameArr, elfStrTblSec.sh_size, 1, elfHandle);

    fseek(elfHandle, elfFileHeader.e_shoff, SEEK_SET);
    for(int i = 0; i < elfFileHeader.e_shnum; i++)
    {
    	fread(&tmpSecHdr, sizeof(Elf64_Shdr), 1, elfHandle);
        if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".text", 5))
        {
        	shText = tmpSecHdr;
            // status = true;
        }
        if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".intr_handler", 13))
		{
			shIrq = tmpSecHdr;
		}
        if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".strtab", 7))
        {
        	symbolSize = tmpSecHdr.sh_size;
        	symbolOffset = tmpSecHdr.sh_offset;
        }
		if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".symtab", 7))
		{
			symTab = tmpSecHdr;
            // printf("hit\n");
		}
		if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".rodata", 7))
		{
			roData = tmpSecHdr;
		}
		if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".data", 7))
		{
			data = tmpSecHdr;
		}
		if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".sdata", 7))
		{
			sdata = tmpSecHdr;
		}
        if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".bss", 4))
		{
			sdata = tmpSecHdr;
		}
    }

    uint32_t numSymTblEntries = symTab.sh_size / sizeof(Elf64_Sym);
    uint32_t *dmemTmp = NULL;
    uint32_t dataSize = 0;
    Elf64_Sym  tmpSymEnt;
    uint64_t dataStartAddr = 0;
    uint64_t dataEndAddr = 0;
    uint64_t codeStarAddr = 0;
    uint64_t codeEndAddr = 0;
    uint32_t codeSize = 0;
    uint64_t boundryAddr = 0;
    uint64_t binAddrStart = 0;
    uint64_t binAddrEnd = 0;
    uint64_t binSizeInPage = 0;
    uint64_t binCodeBoundryAddr = 0;

    char *symbolNames = NULL;
    symbolNames = (char*)malloc(symbolSize + 1);
    fseek(elfHandle, symbolOffset, SEEK_SET);
    fread(symbolNames, symbolSize, 1, elfHandle);

    fseek(elfHandle, symTab.sh_offset, SEEK_SET);
    for(int i = 0; i< numSymTblEntries; i++){
        fread(&tmpSymEnt, sizeof(Elf64_Sym), 1, elfHandle);
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "__code_start", 12))
        {
            codeStarAddr = tmpSymEnt.st_value;
            // printf("hit\n");
        }
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "__code_end", 10))
        {
            codeEndAddr = tmpSymEnt.st_value;
            // printf("hit\n");
        }
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "_rodata_start", 13))
        {
        	dataStartAddr = tmpSymEnt.st_value;
            // printf("hit\n");
        }
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "_bss_end", 8))
        {
        	dataEndAddr = tmpSymEnt.st_value;
            // printf("hit\n");
        }
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "__code_data_boundry", 19))
		{
			boundryAddr = tmpSymEnt.st_value;
            // printf("hit\n");
		}
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "__BinStartAddr", 14))
		{
			binAddrStart = tmpSymEnt.st_value;
            // printf("hit\n");
		}
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "__BinEndAddr", 12))
		{
			binAddrEnd = tmpSymEnt.st_value;
            // printf("hit\n");
		}
        if(!strncmp(&symbolNames[tmpSymEnt.st_name], "__code_end_aligned", 18))
        {
            binCodeBoundryAddr = tmpSymEnt.st_value;
        }
    }

    codeSize = codeEndAddr - codeStarAddr;
    binSizeInPage = (binAddrEnd - binAddrStart) / 4096;

    fprintf(output, "#ifndef _BIN_H_\n");
    fprintf(output, "#define _BIN_H_\n\n");

    fprintf(output, "#define BIN_START_VA 0xf80200000\n\n");

    printf("code segment size is 0x%x\n", codeSize);

    fprintf(output, "#define UCODE_SIZE 0x%x\n", codeSize);
    fprintf(output, "#define UCODE_START_VA 0x%llx\n", codeStarAddr);
    fprintf(output, "#define UCODE_BOUNDRY_ADDR 0x%llx\n", binCodeBoundryAddr);
    fprintf(output, "#define UCODE_ENTRY_ADDR 0x%llx\n", entryAddress);
    fprintf(output, "#define UCODE_SIZE_IN_PAGE 0x%x\n\n", binSizeInPage);
    fprintf(output, "\n");
    
    fprintf(output, "uint32_t UCODE[]={\n");
	//-- Extract microcode from ELF
	fseek(elfHandle, shText.sh_offset, SEEK_SET);
    unsigned int instr;
	for(int i = 0; i < (shText.sh_size/sizeof(unsigned int)); i++)
    {
		fread(&instr, sizeof(unsigned int), 1, elfHandle);
		fprintf(output, "0x%.8x,\n", instr);
    }

	fprintf(output, "};\n\n\n");

    dataSize = dataEndAddr - dataStartAddr;

    printf("data segment size is 0x%x\n", dataSize);
    // printf("haha\n");
    dmemTmp = (uint32_t*)malloc(dataSize);
    // printf("haha\n");
    char ch;

    for(int i = 0; i < dataSize / sizeof(uint32_t); i++) dmemTmp[i] = 0;
    // printf("haha\n");
    char* dmemTmpChar = (char*)dmemTmp;
    unsigned int dmemOffset = 0;
    if(roData.sh_size != 0)
    {
    	dmemOffset = (unsigned int)(roData.sh_addr - dataStartAddr);
        fseek(elfHandle, roData.sh_offset, SEEK_SET);
        for(int i = 0; i<(roData.sh_size); i++)
        {
        	fread(&ch, sizeof(char), 1, elfHandle);
            dmemTmpChar[i+dmemOffset] = ch;
        }
    }

    // printf("haha\n");
    if(data.sh_size != 0)
    {
    	dmemOffset = (unsigned int)(data.sh_addr - dataStartAddr);
        fseek(elfHandle, data.sh_offset, SEEK_SET);
        for(int i = 0; i<( data.sh_size); i++)
        {
        	fread(&ch, sizeof(char), 1, elfHandle);
            dmemTmpChar[i+dmemOffset] = ch;
        }
    }
    if(sdata.sh_size != 0)
    {
    	dmemOffset = (unsigned int)(sdata.sh_addr - dataStartAddr);
        fseek(elfHandle, sdata.sh_offset, SEEK_SET);
        for(int i=0; i < (sdata.sh_size); i++)
        {
        	fread(&ch, sizeof(char), 1, elfHandle);
            dmemTmpChar[i+dmemOffset] = ch;
        }
    }

    fprintf(output, "#define UDATA_SIZE 0x%x\n", dataSize);
    fprintf(output, "#define UDATA_START_VA 0x%llx\n", dataStartAddr);
    fprintf(output, "\n");

    fprintf(output, "uint32_t UDATA[]={\n");
	//-- Extract microcode from ELF
	for(int i = 0; i < (dataSize >> 2); i++)
    {
		// fread(&instr, sizeof(unsigned int), 1, elfHandle);}
		fprintf(output, "0x%.8x,\n", dmemTmp[i]);
    }
	fprintf(output, "};\n\n\n");

    fprintf(output, "#endif\n");
    return 0;
}