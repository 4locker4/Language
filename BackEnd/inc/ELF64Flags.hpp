#ifndef ELF_64_FLAGS
#define ELF_64_FLAGS

typedef enum {
    PT_NULL    = 0,         // Неиспользуемая запись
    PT_LOAD    = 1,         // Загружаемый сегмент (код/данные)
    PT_DYNAMIC = 2,         // Таблицы динамической линковки
    PT_INTERP  = 3,         // Путь к интерпретатору (например, /lib64/ld-linux-x86-64.so.2)
    PT_NOTE    = 4,         // Секции с заметками (метаданные)
    PT_SHLIB   = 5,         // Зарезервировано (не используется)
    PT_PHDR    = 6          // Таблица заголовков программы (Program Headers)
} Elf64_SegmentType;

typedef enum {
    PF_X        = 0x1,      // Сегмент исполняемый (Execute)
    PF_W        = 0x2,      // Сегмент доступен для записи (Write)
    PF_R        = 0x4,      // Сегмент доступен для чтения (Read)
} Elf64_SegmentFlags;

#endif //ELF_64_FLAGS