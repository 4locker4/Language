#ifndef ELF_64_GEN
#define ELF_64_GEN

#include "../../inc/Language.h"
#include "ELF64Flags.hpp"

#include <stdint.h>

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

/*
 * Определяет область видимости символа.
 */
typedef enum {
    STB_LOCAL     = 0,   // Символ локальный
    STB_GLOBAL    = 1,   // Символ глобальный
    STB_WEAK      = 2,   // Слабый символ
    STB_LOOS      = 10,  // Зарезервировано для ОС-специфичных значений
    STB_HIOS      = 12,  // Верхняя граница ОС-специфичных значений
    STB_LOPROC    = 13,  // Зарезервировано для процессор-специфичных значений
    STB_HIPROC    = 15   // Верхняя граница процессор-специфичных значений
} Elf64_SymBind;

/*
 * Определяет природу символа (функция, данные и т.д.).
 */
typedef enum {
    STT_NOTYPE    = 0,   // Тип не указан
    STT_OBJECT    = 1,   // Символ представляет данные (глобальная переменная)
    STT_FUNC      = 2,   // Символ представляет функцию
    STT_SECTION   = 3,   // Символ связан с секцией
    STT_FILE      = 4,   // Имя исходного файла
    STT_LOOS      = 10,  // Зарезервировано для ОС-специфичных значений
    STT_HIOS      = 12,  // Верхняя граница ОС-специфичных значений
    STT_LOPROC    = 13,  // Зарезервировано для процессор-специфичных значений
    STT_HIPROC    = 15   // Верхняя граница процессор-специфичных значений
} Elf64_SymType;

#endif // ELF_64_GEN