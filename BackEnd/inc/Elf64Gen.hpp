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

typedef struct
{
    const char * label_name = NULL;
    int64_t      address    = 0;
} SYMBOL;

typedef struct
{
    const char * target = NULL;
    int patch_offset    = 0;
    bool relative       = 0;
} RELOCATION;

typedef struct
{
    RELOCATION * relocation = NULL;

    size_t free_box = 0;
    size_t capacity = 0;
} RELOC_TABLE;

typedef struct
{
    SYMBOL * labels = NULL;

    size_t free_box = 0;
    size_t capacity = 0;

    size_t labels_len = 0;
} SYMBOL_TABLE;

#endif // ELF_64_GEN