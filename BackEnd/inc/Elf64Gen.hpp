#ifndef ELF_64_GEN
#define ELF_64_GEN

#include "../../inc/Language.h"
#include "ELF64Flags.hpp"

#include <stdint.h>
#include <elf.h>
#include "emitters.hpp"

#include "../../inc/Utils.h"
#include "../../inc/Asserts.h"
#include "../../inc/Tree.h"

typedef struct {
    uint8_t  e_ident[16];    // ELF-идентификация
    uint16_t e_type;         // Тип файла
    uint16_t e_machine;      // Архитектура
    uint32_t e_version;      // Версия ELF
    uint64_t e_entry;        // Точка входа
    uint64_t e_phoff;        // Смещение заголовка программы
    uint64_t e_shoff;        // Смещение заголовка секции
    uint32_t e_flags;        // Флаги
    uint16_t e_ehsize;       // Размер ELF-заголовка
    uint16_t e_phentsize;    // Размер заголовка программы
    uint16_t e_phnum;        // Количество заголовков программы
    uint16_t e_shentsize;    // Размер заголовка секции
    uint16_t e_shnum;        // Количество заголовков секции
    uint16_t e_shstrndx;     // Индекс таблицы строк секции
} Elf64_Ehdr;

typedef struct {
    uint32_t p_type;         // Тип сегмента
    uint32_t p_flags;        // Флаги
    uint64_t p_offset;       // Смещение в файле
    uint64_t p_vaddr;        // Виртуальный адрес
    uint64_t p_paddr;        // Физический адрес
    uint64_t p_filesz;       // Размер в файле
    uint64_t p_memsz;        // Размер в памяти
    uint64_t p_align;        // Выравнивание
} Elf64_Phdr;

typedef struct {
    uint32_t sh_name;       // Индекс имени секции в .shstrtab
    uint32_t sh_type;       // Тип секции
    uint64_t sh_flags;      // Атрибуты
    uint64_t sh_addr;       // Виртуальный адрес в памяти
    uint64_t sh_offset;     // Смещение секции в файле
    uint64_t sh_size;       // Размер секции в байтах
    uint32_t sh_link;       // Ссылка на другую секцию
    uint32_t sh_info;       // Дополнительная информация
    uint64_t sh_addralign;  // Выравнивание адреса
    uint64_t sh_entsize;    // Размер элемента таблицы
} Elf64_Shdr;

const char SEPARATER = ' ';

typedef struct
{
    char * ident_name = NULL;
    size_t ident_num  = 0;
} IDENT_INFO;

TOKEN *             FillTokenTypes            (TOKEN_TABLE * table);
void                CountTokens               (TOKEN_TABLE * table);
ALL_OPS_DATAS *     OpsCtor                   (TOKEN_TABLE * table);
TOKEN_TABLE *       TableCtor                 (const char * file_with_data);

void    ReadTree            (TOKEN_TABLE * token_table);
NODE *  RecursyTreeRead     (NODE * node, FILE * file);
void    GenOpCode           (NODE * node, FILE * file);
void    IfCodeGen           (NODE * node, FILE * file);
void    WhileCodeGen        (NODE * node, FILE * file);
NODE *  InitAsmFunc         (NODE * node, FILE * file);

void AsmOpsCompare (NODE * node, FILE * file, OPERATORS mode, int label);
void AsmConditional (NODE * node, FILE * file, OPERATORS mode, int label);

#endif // ELF_64_GEN