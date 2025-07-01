#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    FILE *      file;
    uint64_t    base_addr;
    uint64_t    offset;
    uint8_t *   buffer;
    size_t      buffer_size;
} CodeBuffer_t;

int     code_buffer_init    (CodeBuffer_t* cb, FILE* file, uint64_t base_addr);
void    code_buffer_free   (CodeBuffer_t* cb);
int     code_buffer_write   (CodeBuffer_t* cb, const uint8_t* bytes, size_t len);

void    emit_imul_reg_reg  (CodeBuffer_t* cb, int dest_reg, int src_reg);
void    emit_sub_reg_reg   (CodeBuffer_t* cb, int dest_reg, int src_reg);
void    emit_add_reg_reg   (CodeBuffer_t* cb, int dest_reg, int src_reg);
void    emit_cmp_jle       (CodeBuffer_t* cb, int reg_num, int32_t imm, uint64_t target_addr);
void    emit_jmp           (CodeBuffer_t* cb, uint64_t target_addr);
void    emit_ret           (CodeBuffer_t* cb);