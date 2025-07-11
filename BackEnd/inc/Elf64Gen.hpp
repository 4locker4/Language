#ifndef ELF_64_GEN
#define ELF_64_GEN

#include "../../inc/Language.h"

#include <stdint.h>
#include <elf.h>

const int _START_ADDRESS = 0x1000;

typedef enum
{
    RAX = 0,
    RCX,
    RDX,
    RBX,
    RSP,
    RBP,
    RSI,
    RDI,

    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
} REGISTERS;

typedef enum
{
    TEXT_SEG = 1,
    DATA_SEG = 2,
    EXTERN_FUNC = SHN_UNDEF
} SEG_NUM;

typedef struct
{
    Elf64_Rela * relocation = NULL;
    char ** target_names    = NULL;

    size_t free_box = 0;
    size_t capacity = 0;
} RELOC_TABLE;

typedef struct
{
    Elf64_Sym * labels = NULL;
    
    size_t l_free_box = 0;
    size_t l_capacity = 0;
} SYMBOL_TABLE;

typedef struct 
{
    char * strtab     = NULL;

    size_t srt_id     = 0;
    size_t str_capacity = 0;
} STRTAB_TABLE;

typedef struct
{
    RELOC_TABLE  rel_t = {};
    SYMBOL_TABLE sm_t  = {};
    STRTAB_TABLE str_t = {};
} ADDR_T;

#endif // ELF_64_GEN