#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"

int main(int argc, char *argv[]){
    if(argc == 2){
        printf("The target file name is %s\n", argv[1]);
    }

    char filename[20];

    Elf64_Ehdr elfFileHeader = {0};

    FILE* elfHandle = fopen("testbench.elf", "rb");

    fread(&elfFileHeader, sizeof(Elf64_Ehdr), 1, elfHandle);

    unsigned int strTblIndex = elfFileHeader.e_shstrndx;
    unsigned long long symbolOffset = 0;

    Elf64_Shdr elfStrTblSec = {0}, tmpSecHdr = { 0 };
    Elf64_Shdr symTab= {0};

    unsigned int symbolSize;
    char* sectionNameArr;

    //Get Section Table
    fseek(elfHandle, (unsigned long)((Elf64_Shdr*)elfFileHeader.e_shoff + strTblIndex), SEEK_SET);
    fread(&elfStrTblSec, sizeof(Elf64_Shdr), 1, elfHandle);

    sectionNameArr = (char*)malloc((elfStrTblSec.sh_size + 1));
    fseek(elfHandle, (elfStrTblSec.sh_offset), SEEK_SET);
    fread(sectionNameArr, elfStrTblSec.sh_size, 1, elfHandle);

    fseek(elfHandle, elfFileHeader.e_shoff, SEEK_SET);
    
    for(int i = 0; i < elfFileHeader.e_shnum; i++)
    {
        fread(&tmpSecHdr, sizeof(Elf64_Shdr), 1, elfHandle);

        if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".symtab", 7))
		{
			symTab = tmpSecHdr;
		}

        if(!strncmp(&sectionNameArr[tmpSecHdr.sh_name], ".strtab", 7))
        {
        	symbolSize = tmpSecHdr.sh_size;
        	symbolOffset = tmpSecHdr.sh_offset;
        }
    }

    unsigned int numSymTblEntries = symTab.sh_size / sizeof(Elf64_Sym);
    char *symbolNames = NULL;
    Elf64_Sym  tmpSymEnt;
    
    symbolNames = (char*)malloc(symbolSize + 1);

    fseek(elfHandle, symbolOffset, SEEK_SET);
    fread(symbolNames, symbolSize, 1, elfHandle);

    fseek(elfHandle, symTab.sh_offset, SEEK_SET);
    for(int i = 0; i< numSymTblEntries; i++){
        fread(&tmpSymEnt, sizeof(Elf64_Sym), 1, elfHandle);
        printf("Symbol name is 0%x\n", tmpSymEnt.st_name);
    }


    return 0;
}
