#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./Elf64Gen.hpp"

const size_t START_TAB_CAPACITY = 10;
const size_t ALIGN              = 0x1000;

typedef struct
{
    uint64_t    offset      = 0;
    uint8_t *   buffer      = NULL;
    size_t      buffer_size = 0;
} CodeBuffer_t;

void CreateElf64File    (TOKEN_TABLE * token_table, const char * file_name);

int  code_buffer_init   (CodeBuffer_t * cb);
void code_buffer_free   (CodeBuffer_t * cb);
int  code_buffer_write  (CodeBuffer_t * cb, const uint8_t* bytes, size_t len);

void emit_call_func     (CodeBuffer_t * cb, const char * target, RELOC_TABLE * rel_t);

void emit_ja            (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jb            (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jae           (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jbe           (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_je            (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jge           (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jle           (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jg            (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jl            (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jne           (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);
void emit_jmp           (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t);

void emit_idiv_reg      (CodeBuffer_t * cb, REGISTERS dest_reg);

void emit_imul_reg_num  (CodeBuffer_t * cb, REGISTERS reg, int32_t num);
void emit_imul_reg_reg  (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg);

void emit_push_reg      (CodeBuffer_t * cb, REGISTERS dest_reg);
void emit_push_mem      (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp);
void emit_push_num      (CodeBuffer_t * cb, int32_t imm);

void emit_pop_reg       (CodeBuffer_t * cb, REGISTERS dest_reg);
void emit_pop_mem       (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp);

void emit_inc_reg       (CodeBuffer_t * cb, REGISTERS dest_reg);
void emit_shr_reg_imm   (CodeBuffer_t * cb, REGISTERS dest_reg, uint8_t shift);
void emit_syscall       (CodeBuffer_t * cb);
void emit_cqo           (CodeBuffer_t * cb);

void emit_sub_reg_reg   (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg);
void emit_sub_reg_num   (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t imm);
void emit_sub_r8_imm8   (CodeBuffer_t * cb, REGISTERS dest_reg, uint8_t imm) ;

void emit_add_reg_reg   (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg);
void emit_add_reg_num   (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t imm);

void emit_cmp_reg_reg   (CodeBuffer_t * cb, REGISTERS reg1, REGISTERS reg2);
void emit_cmp_reg_num   (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t imm);
void emit_cmp_r8_imm8   (CodeBuffer_t * cb, REGISTERS dest_reg, uint8_t imm);

void emit_lea_reg_addr  (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS base_reg, int32_t disp);
void emit_lea_reg_label (CodeBuffer_t * cb, REGISTERS reg, const char * label, RELOC_TABLE * rel_t);
void emit_xor_reg_reg   (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg);
void emit_neg_reg       (CodeBuffer_t * cb, REGISTERS dest_reg);
void emit_ret           (CodeBuffer_t * cb);

void emit_mov_reg_reg   (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg);
void emit_mov_mem_reg   (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp, REGISTERS src_reg);
void emit_mov_reg_mem   (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp, REGISTERS src_reg);
void emit_mov_reg_imm   (CodeBuffer_t * cb, REGISTERS dest_reg, int64_t imm);
void emit_mov_mem_num   (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp, int64_t imm);
void emit_mov_r8_imm8   (CodeBuffer_t * cb, REGISTERS reg, uint8_t imm);
void emit_mov_r8_mem    (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS base_reg, int32_t disp);

void SqrtByNewton       (CodeBuffer_t * cb, ADDR_T * for_rel_t);
void InputFunc          (CodeBuffer_t * cb, ADDR_T * for_rel_t);

void                Link2Files             (CodeBuffer_t * cb, const char * likable_file);


void                ApplyRelocation        (CodeBuffer_t * cb, ADDR_T * for_rel_t);
size_t              MakeSymArray           (SYMBOL_TABLE * sm_t);
void                AddRelocation          (RELOC_TABLE * rel_t, const char * target, Elf64_Addr offset);
void                AddLabel               (ADDR_T * for_rel_t, const char * label_name, int64_t address, char st_info, int64_t st_shndx);
void                ReadTreeBin            (TOKEN_TABLE * token_table, CodeBuffer_t * cb, CodeBuffer_t * cb_data, ADDR_T * for_rel_t);

void                ReadFuncArgsBin        (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t);
void                ReverseReadFuncArgsBin (NODE * node, CodeBuffer_t * cb, size_t * n_args, ADDR_T * for_rel_t);

NODE *              RecursyTreeReadBin     (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t);
void                GenOpCodeBin           (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t);
void                IfCodeGenBin           (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t);
void                WhileCodeGenBin        (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t);
NODE *              InitAsmFuncBin         (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t);

void                AsmOpsCompareBin       (NODE * node, CodeBuffer_t * cb, OPERATORS mode, int label, ENUM_IDENT_DATA_TYPE data_type, ADDR_T * for_rel_t);
void                AsmConditionalBin      (NODE * node, CodeBuffer_t * cb, OPERATORS mode, int label, ADDR_T * for_rel_t);
