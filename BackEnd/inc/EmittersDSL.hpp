#ifndef EMITTERS_DSL_HPP
#define EMITTERS_DSL_HPP

#include "emitters.hpp"

#define  EMIT_CALL_FUNC(target)                 emit_call_func (cb, target);                        // target

#define  JA(label)                              emit_ja (cb, label);                                // label             
#define  JB(label)                              emit_jb (cb, label);                                // label
#define  JAE(label)                             emit_jae (cb, label);                               // label
#define  JBE(label)                             emit_jbe (cb, label);                               // label
#define  JE(label)                              emit_je (cb, label);                                // label
#define  JGE(label)                             emit_jge (cb, label);                               // label
#define  JLE(label)                             emit_jle (cb, label);                               // label
#define  JG(label)                              emit_jg (cb, label);                                // label
#define  JL(label)                              emit_jl (cb, label);                                // label
#define  JNE(label)                             emit_jne (cb, label);                               // label
#define  JMP(label)                             emit_jmp (cb, label);                               // label

#define  IDIV_REG(dest_reg)                     emit_idiv_reg (cb, dest_reg);                       // dest_reg

#define  IMUL_REG_NUM(dest_reg, num)            emit_imul_reg_num (cb, dest_reg, num);                   // reg, num
#define  IMUL_REG_REG(dest_reg, src_reg)        emit_imul_reg_reg (cb, dest_reg, src_reg);          // dest_reg, src_reg

#define  PUSH_REG(dest_reg)                     emit_push_reg (cb, dest_reg);                       // dest_reg
#define  PUSH_MEM(dest_reg, disp)               emit_push_mem (cb, dest_reg, disp);                 // dest_reg, disp
#define  PUSH_NUM(imm)                          emit_push_num (cb, imm);                            // imm

#define  POP_REG(dest_reg)                      emit_pop_reg (cb, dest_reg);                        // dest_reg
#define  POP_MEM(dest_reg, disp)                emit_pop_mem (cb, dest_reg, disp);                  // dest_reg, disp

#define  INC_REG(dest_reg)                      emit_inc_reg (cb, dest_reg);                        // dest_reg
#define  SHR_REG_IMM(dest_reg, shift)           emit_shr_reg_imm (cb, dest_reg, shift);             // dest_reg, shift
#define  SYSCALL()                              emit_syscall (cb);
#define  CQO()                                  emit_cqo (cb);

#define  SUB_REG_REG(dest_reg, src_reg)         emit_sub_reg_reg (cb, dest_reg, src_reg);           // dest_reg, src_reg
#define  SUB_REG_NUM(dest_reg, imm)             emit_sub_reg_num (cb, dest_reg, imm);               // cb, dest_reg, imm
#define  SUB_R8_IMM8(dest_reg, imm)             emit_sub_r8_imm8 (cb, dest_reg, imm) ;              // cb, dest_reg, imm

#define  ADD_REG_REG(dest_reg, src_reg)         emit_add_reg_reg (cb, dest_reg, src_reg);           // dest_reg, src_reg
#define  ADD_REG_NUM(dest_reg, imm)             emit_add_reg_num (cb, dest_reg, imm);               // cb, dest_reg, imm

#define  CMP_REG_REG(reg1, reg2)                emit_cmp_reg_reg (cb, reg1, reg2);                  // reg1, reg2
#define  CMP_REG_NUM(dest_reg, imm)             emit_cmp_reg_num (cb, dest_reg, imm);               // cb, dest_reg, imm
#define  CMP_R8_IMM8(dest_reg, imm)             emit_cmp_r8_imm8 (cb, dest_reg, imm);               // cb, dest_reg, imm

#define  LEA_REG_LABEL(dest_reg, label)         emit_lea_reg_label (cb, dest_reg, label);           // dest_reg, label
#define  XOR_REG_REG(dest_reg, src_reg)         emit_xor_reg_reg (cb, dest_reg, src_reg);           // dest_reg, src_reg
#define  NEG_REG(dest_reg)                      emit_neg_reg (cb, dest_reg);                        // dest_reg
#define  RET()                                  emit_ret (cb);

#define  MOV_REG_REG(dest_reg, src_reg)         emit_mov_reg_reg (cb, dest_reg, src_reg);           // dest_reg, src_reg
#define  MOV_MEM_REG(dest_reg, disp, src_reg)   emit_mov_mem_reg (cb, dest_reg, disp, src_reg);     // dest_reg, disp, src_reg
#define  MOV_REG_MEM(dest_reg, disp, src_reg)   emit_mov_reg_mem (cb, dest_reg, disp, src_reg);     // dest_reg, disp, src_reg
#define  MOV_REG_IMM(dest_reg, imm)             emit_mov_reg_imm (cb, dest_reg, imm);               // dest_reg, imm)
#define  MOV_MEM_NUM(dest_reg, disp, imm)       emit_mov_mem_num (cb, dest_reg, disp, imm);         // dest_reg, disp, imm
#define  MOV_R8_IMM8(reg, imm)                  emit_mov_r8_imm8 (cb, reg, imm);                    // reg, num
#define  MOV_R8_MEM(dest_reg, base_reg, disp)   emit_mov_r8_mem (cb, dest_reg, base_reg, disp);     // dest_reg, base_reg, disp

#endif