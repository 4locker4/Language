#include "../inc/emitters.hpp"

int code_buffer_init (CodeBuffer_t * cb)
{
    cb->offset = 0;
    cb->buffer_size = 1024;

    cb->buffer = (uint8_t*) calloc (cb->buffer_size, sizeof (uint8_t));
    if (cb->buffer == NULL)
    {
        fprintf (stderr, "Error: failed to allocate cb buffer\n");
        exit (1);
    }

    return 0;
}

void code_buffer_free (CodeBuffer_t * cb)
{
    free (cb->buffer);
    cb->buffer_size = 0;
}

int code_buffer_write (CodeBuffer_t * cb, const uint8_t* bytes, size_t len)
{
    while (cb->offset + len > cb->buffer_size)
    {
        cb->buffer_size *= 2;
        cb->buffer = (uint8_t*) realloc (cb->buffer, cb->buffer_size);
        if (cb->buffer == NULL)
        {
            fprintf (stderr, "Error: failed to realloc code buffer\n");
            exit (1);
        }
    }

    memcpy (cb->buffer + cb->offset, bytes, len);

    cb->offset += len;

    return 0;
}

void emit_syscall (CodeBuffer_t * cb) 
{
    uint8_t opcode[] = {0x0F, 0x05};
    code_buffer_write(cb, opcode, 2);
}

void emit_cqo (CodeBuffer_t * cb) 
{
    uint8_t opcode[] = {0x48, 0x99};
    code_buffer_write(cb, opcode, 2);
}
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ START EMMITERS ZONE =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ //

//====================== Emitter for func call ======================//
void emit_call_func (CodeBuffer_t * cb, const char * target, RELOC_TABLE * rel_t)           
{
    uint8_t opcode = 0xE8;
    uint8_t bytes[] = { 0, 0, 0, 0 };

    code_buffer_write (cb, &opcode, 1);

    AddRelocation (rel_t, target, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for ja =========================//
void emit_ja (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x87};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);

    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jb =========================//
void emit_jb (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x82};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jae ========================//
void emit_jae (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x83};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);

    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jbe ========================//
void emit_jbe (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x86};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);

    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for je ========================//
void emit_je (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x84};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jge ========================//
void emit_jge (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x8D};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jle ========================//
void emit_jle (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x8E};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jg ========================//
void emit_jg (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x8F};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for jl ========================//
void emit_jl (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x8C};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}

void emit_jne (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode[] = {0x0F, 0x85};                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, opcode, 2);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}
//==================== Emitter for jmp ====================//
void emit_jmp (CodeBuffer_t * cb, const char * label, RELOC_TABLE * rel_t)
{
    uint8_t opcode = 0xE9;                              // opcode: jmp with 32-bit relative offset
    uint8_t bytes[] = { 0, 0, 0, 0 };
    code_buffer_write (cb, &opcode, 1);
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write (cb, bytes, 4);
}
/*
                +=================================+
                |   7  6 |  5  4  3 |  2  1  0    |
                |   [mod]| [ reg ]  |  [ r/m ]    |
                +=================================+ 
*/
//==================== Emitter for idiv ====================//

void emit_idiv_reg (CodeBuffer_t * cb, REGISTERS dest_reg)
{
    uint8_t rex = 0x48;

    if (dest_reg >= 8) rex |= 0x01;

    uint8_t opcode = 0xF7;

    uint8_t mod = 0x03;
    uint8_t reg = 0x07;             // для idiv такой
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

//==================== Emitter for imul ====================//
void emit_imul_reg_num (CodeBuffer_t * cb, REGISTERS reg, int32_t num)
{
    uint8_t rex = 0x48;                                         // REX.W 64-битный режим

    if (reg >= 8) rex |= 0x4;                                   // REX.R для reg

    uint8_t opcode = 0x69;                                      // 0x0F - for extended instructions | 0xAF - specific opcode for imul
    uint8_t modrm = 0xC0 | ((reg & 0x7) << 3) | 0x05;
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);

    code_buffer_write(cb, reinterpret_cast<const uint8_t*>(&num), 4);
}

void emit_imul_reg_reg (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg)
{
    uint8_t rex = 0x48;                                 // REX.W

    if (dest_reg >= 8) rex |= 0x4;
    if (src_reg  >= 8) rex |= 0x1;

    uint8_t opcode[] = { 0x0F, 0xAF };                  // 0x0F - for extended instructions | 0xAF - specific opcode for imul

    uint8_t modrm = 0xC0;                               // mod=11 регистр-регистр
    modrm |= (dest_reg & 0x7) << 3;                     // reg поле dest
    modrm |= (src_reg & 0x7);                           // r/m поле src

    uint8_t bytes[] = { rex, opcode[0], opcode[1], modrm };
    code_buffer_write (cb, bytes, 4);
}

//====================== Emitter for push r_x ======================//
void emit_push_reg (CodeBuffer_t * cb, REGISTERS dest_reg)
{
    if (dest_reg < 8)
    {
        uint8_t bytes = 0x50 + (dest_reg & 0x07);
        code_buffer_write (cb, &bytes, 1);
    }
    else
    {
        uint32_t rex = 0x49;
        uint32_t opcode = 0x50 + (dest_reg & 0x07);
        uint8_t bytes[] = { rex, opcode };
        code_buffer_write (cb, bytes, 2);
    }
}

void emit_push_mem (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp)
{
    uint32_t rex = 0x48;
    
    if (dest_reg >= 8) rex |= 0x01;

    uint32_t opcode = 0xFF;

    uint8_t mod = 0x00;
    uint8_t reg = 0x00;
    uint8_t rm  = 0x00;

    if (disp == 0 && dest_reg != RBP)                       // (dest_reg & 0x7) != 0x5 - это регистр RBP, который требует обязательного смещения.
    {
        mod = 0x00  << 6;       // mod=00 без смещения
        reg = 0b110 << 3;       // reg=110 (6 для PUSH)
        rm  = dest_reg & 0x7;
    }
    else if (disp >= INT8_MIN && disp <= INT8_MAX)
    {
        mod = 0x01 << 6;        // mod=01 8 битное смещение
        reg = 0b110 << 3;
        rm  = dest_reg & 0x7;
    }
    else
    {
        mod = 0x02 << 6;        // mod=02 32 битное смещение
        reg = 0b110 << 3;
        rm = dest_reg & 0x07;
    }

    uint8_t modrm = mod | reg | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);

// вычисляем смещение, если есть
    if (modrm >= 0b01110000) 
    {
        if (modrm < 0xB0)   
// 8 битное смещение
        {
            int8_t disp8 = (int8_t) disp;
            code_buffer_write (cb, (uint8_t *) &disp8, 1);
        } 
        else
// 32 битное смещение
            code_buffer_write (cb, (uint8_t *) &disp, 4);
    }
}

void emit_push_num (CodeBuffer_t * cb, int32_t imm)
{
    if (imm >= INT8_MIN && imm <= INT8_MAX)
// 8-битное значение
    {
        uint8_t opcode = 0x6A;
        int8_t imm8 = (int8_t) imm;
        uint8_t bytes[] = { opcode, *(uint8_t *) &imm8 };
        code_buffer_write (cb, bytes, 2);
    } 
    else 
// 32-битное значение
    {
        uint8_t opcode = 0x68;
        uint8_t bytes[] = { opcode };
        code_buffer_write (cb, bytes, 1);
        code_buffer_write (cb, (uint8_t*)&imm, 4);
    }
}

//====================== Emitter for push r_x ======================//
void emit_pop_reg (CodeBuffer_t * cb, REGISTERS dest_reg)
{
    if (dest_reg < 8)
    {
        uint8_t bytes = 0x58 + dest_reg;
        code_buffer_write (cb, &bytes, 1);
    }
    else
    {
        uint8_t rex = 0x49;
        uint8_t opcode = 0x58 + (dest_reg & 0x07);
        uint8_t bytes[] = { rex, opcode };
        code_buffer_write (cb, bytes, 2);
    }
}

void emit_pop_mem (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp)
{
    uint8_t rex = 0x48;

    if (dest_reg >= 8) rex |= 0x1;

    uint8_t opcode = 0x8F;

    uint8_t mod = 0x00;
    uint8_t reg = 0x00;         // 0 для POP
    uint8_t rm  = dest_reg & 0x07;

    if (disp == 0 && (dest_reg & 0x07) != 0x05)
// [reg] без смещения и не rbp, как в push
        mod = 0x00;
    else if (disp >= INT8_MIN && disp <= INT8_MAX) 
// 8 битное смещение
        mod = 0x01;
    else
// 32 битное смещение
        mod = 0x02;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);

    if (mod == 0x01)
    {
        int8_t disp8 = (int8_t) disp;
        code_buffer_write (cb, (uint8_t *) &disp8, 1);
    } 
    else if (mod == 0x02) 
        code_buffer_write (cb, (uint8_t *) &disp, 4);
}

//======================== Emitter for inc =========================//

void emit_inc_reg (CodeBuffer_t * cb, REGISTERS dest_reg)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x01;

    uint8_t opcode = 0xFF;

    uint8_t mod = 0x03;
    uint8_t reg = 0x00;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

void emit_shr_reg_imm (CodeBuffer_t * cb, REGISTERS dest_reg, uint8_t shift)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x04;

    uint8_t opcode = 0xC1;

    uint8_t mod = 0x03;
    uint8_t reg = 0x05;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;
    uint8_t bytes[] = {rex, opcode, modrm, shift};
    code_buffer_write(cb, bytes, 4);
}

void emit_neg_reg (CodeBuffer_t * cb, REGISTERS dest_reg)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x04;

    uint8_t opcode = 0xF7;

    uint8_t mod = 0x03;
    uint8_t reg = 0x03;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };

    code_buffer_write (cb, bytes, 3);
}
//==================== Emitter for sub reg, reg ====================//
void emit_sub_reg_reg (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg)
{
    uint8_t rex = 0x48;                                 // REX.W
    if (dest_reg >= 8) rex |= 0x01;
    if (src_reg  >= 8) rex |= 0x04;

    uint8_t opcode = 0x2B;                              // opcode: sub when subtracting a register from a register.
    uint8_t modrm = (uint8_t) (0xC0 | (src_reg << 3) | (dest_reg & 7));
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

void emit_sub_reg_num (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t imm)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x01;

    uint8_t mod = 0x03;
    uint8_t reg = 0x05;     // 5 для sub
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    if (dest_reg < 8 && imm >= INT8_MIN && imm <= INT8_MAX)
// Для 8 битного числа
    {
        uint8_t opcode = 0x83;
        int8_t imm8 = (int8_t) imm;
        uint8_t bytes[] = { rex, opcode, modrm, * (uint8_t *) &imm8 };
        code_buffer_write (cb, bytes, 4);
    }
    else 
// Для 32 битного числа
    {
        uint8_t opcode = 0x81;
        uint8_t bytes[] = { rex, opcode, modrm };
        code_buffer_write (cb, bytes, 3);
        code_buffer_write (cb, (uint8_t * )&imm, 4);
    }
}

void emit_sub_r8_imm8 (CodeBuffer_t * cb, REGISTERS dest_reg, uint8_t imm) 
{
    uint8_t opcode = 0x80;

    uint8_t mod = 0x03;
    uint8_t reg = 0x05;     // 5 для sub
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { opcode, modrm, imm };
    code_buffer_write (cb, bytes, 3);
}

//==================== Emitter for add reg, reg ====================//
void emit_add_reg_reg (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg)
{
    uint8_t rex = 0x48;                                 // REX.W
    if (dest_reg >= 8) rex |= 0x01;
    if (src_reg  >= 8) rex |= 0x04;

    uint8_t opcode = 0x01;                              // opcode: add register to register
    uint8_t modrm = (uint8_t) (0xC0 | (src_reg << 3) | (dest_reg & 7));
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

void emit_add_reg_num (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t imm)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x01;

    uint8_t mod = 0x03;
    uint8_t reg = 0x00;     // 0 для add
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    if (dest_reg < 8 && imm >= INT8_MIN && imm <= INT8_MAX)
// Для 8 битного числа
    {
        uint8_t opcode = 0x83;
        int8_t imm8 = (int8_t) imm;
        uint8_t bytes[] = { rex, opcode, modrm, * (uint8_t *) &imm8 };
        code_buffer_write (cb, bytes, 4);
    }
    else 
// Для 32 битного числа
    {
        uint8_t opcode = 0x81;
        uint8_t bytes[] = { rex, opcode, modrm };
        code_buffer_write (cb, bytes, 3);
        code_buffer_write (cb, (uint8_t * )&imm, 4);
    }
}

//===================== Emitter for cmp =====================//

void emit_cmp_reg_reg (CodeBuffer_t * cb, REGISTERS reg1, REGISTERS reg2)
{
    uint8_t rex = 0x48;
    if (reg1 >= 8) rex |= 0x04;
    if (reg2 >= 8) rex |= 0x01;

    uint8_t opcode = 0x39;
    
    uint8_t mod = 0x03;
    uint8_t reg = reg1 & 0x07;
    uint8_t rm  = reg2 & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

void emit_cmp_reg_num (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t imm)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x01;

    uint8_t opcode = 0x81;

    uint8_t mod = 0x03;
    uint8_t reg = 0x07;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;
    uint8_t bytes[] = {rex, opcode, modrm};
    code_buffer_write (cb, bytes, 3);
    code_buffer_write (cb, (uint8_t *) &imm, 4);
}

void emit_cmp_r8_imm8 (CodeBuffer_t * cb, REGISTERS dest_reg, uint8_t imm)
// только для AL - DL
{
    uint8_t opcode = 0x80;

    uint8_t mod = 0x03;
    uint8_t reg = 0x07;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = {opcode, modrm, imm};
    code_buffer_write (cb, bytes, 3);
}

void emit_lea_reg_label (CodeBuffer_t * cb, REGISTERS reg, const char * label, RELOC_TABLE * rel_t) 
{
    uint32_t rip_offset_placeholder = 0;

    uint8_t rex = 0x48;
    if (reg >= 8) rex |= 0x4;

    uint8_t opcode = 0x8D;
    uint8_t modrm = ((reg & 0x7) << 3) | 0x05;

    uint8_t bytes[] = { rex, opcode, modrm };
    
    int offset_pos = cb->offset;
    code_buffer_write(cb, bytes, sizeof(bytes));
    
    uint8_t nul_bytes[] { 0x00, 0x00, 0x00, 0x00 };
    
    AddRelocation (rel_t, label, cb->offset);
    code_buffer_write(cb, nul_bytes, sizeof(nul_bytes));
}

void emit_lea_reg_addr (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS base_reg, int32_t disp)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x04;
    if (base_reg >= 8) rex |= 0x01;

    uint8_t opcode = 0x8D;

    uint8_t mod = 0x00;
    uint8_t reg = dest_reg & 0x07;
    uint8_t rm  = base_reg & 0x07;

    if (disp == 0 && (dest_reg & 0x07) != 0x05)
    // [reg] без смещения и не rbp, как в push
            mod = 0x00;
    else if (disp >= INT8_MIN && disp <= INT8_MAX) 
    // 8 битное смещение
            mod = 0x01;
    else
    // 32 битное смещение
            mod = 0x02;
    
            uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = {rex, opcode, modrm};
    code_buffer_write (cb, bytes, 3);

    if (mod == 0x01)
    // 8 битное смещение
        {
            int8_t disp8 = (int8_t) disp;
            code_buffer_write (cb, (uint8_t *) disp8, 1);
        } 
    // 32 битное смещение
        else if (mod == 0x02)
            code_buffer_write (cb, (uint8_t *) &disp, 4);
}

void emit_xor_reg_reg (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x04;
    if (src_reg >= 8)  rex |= 0x01;

    uint8_t opcode = 0x31;

    uint8_t mod = 0x03;
    uint8_t reg = src_reg  & 0x07;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = {rex, opcode, modrm};
    code_buffer_write (cb, bytes, 3);
}

//==================== Emitter for ret ====================//
void emit_ret (CodeBuffer_t * cb)
{
    uint8_t opcode = 0xC3;                              // opcode: ret
    code_buffer_write (cb, &opcode, 1);
}

//==================== Emitter for mov ====================//

void emit_mov_reg_reg (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS src_reg)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x04;
    if (src_reg >= 8)  rex |= 0x01;
    
    uint8_t opcode = 0x89;

    uint8_t mod = 0x03;
    uint8_t reg = src_reg  & 0x07;
    uint8_t rm  = dest_reg & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

void emit_mov_mem_reg (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp, REGISTERS src_reg)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x01;
    if (src_reg >= 8)  rex |= 0x04;
    
    uint8_t opcode = 0x89;

    uint8_t mod = 0x00;
    uint8_t reg = src_reg  & 0x07;
    uint8_t rm  = dest_reg & 0x07;

    if (disp == 0 && (dest_reg & 0x7) != 0x5) 
// Без смещения и не rbp
        mod = 0x00;
    else if (disp >= INT8_MIN && disp <= INT8_MAX)
// 8 битное смещение
        mod = 0x01;
    else
// 32 битное смещение
        mod = 0x02;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);

    if (mod == 0x01)
// 8 битное смещение
    {
        int8_t disp8 = (int8_t) disp;
        code_buffer_write (cb, (uint8_t *) disp8, 1);
    } 
// 32 битное смещение
    else if (mod == 0x02)
        code_buffer_write (cb, (uint8_t *) &disp, 4);
}

void emit_mov_reg_mem (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp, REGISTERS src_reg)
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x04;
    if (src_reg >= 8)  rex |= 0x01;
    
    uint8_t opcode = 0x8B;

    uint8_t mod = 0x00;
    uint8_t reg = dest_reg & 0x07;
    uint8_t rm  = src_reg  & 0x07;

    if (disp == 0 && (dest_reg & 0x7) != 0x5) 
// Без смещения и не rbp
        mod = 0x00;
    else if (disp >= INT8_MIN && disp <= INT8_MAX)
// 8 битное смещение
        mod = 0x01;
    else
// 32 битное смещение
        mod = 0x02;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);

    if (mod == 0x01)
// 8 битное смещение
    {
        int8_t disp8 = (int8_t) disp;
        code_buffer_write (cb, (uint8_t *) disp8, 1);
    } 
// 32 битное смещение
    else if (mod == 0x02)
        code_buffer_write (cb, (uint8_t *) &disp, 4);
}

void emit_mov_reg_imm (CodeBuffer_t * cb, REGISTERS dest_reg, int64_t imm) 
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x4;

    uint8_t opcode = 0xC7;

    uint8_t mod = 0x03;
    uint8_t reg = 0x00;
    uint8_t rm  = dest_reg  & 0x07;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
    code_buffer_write (cb, (uint8_t *) &imm, 4);
}

void emit_mov_mem_num (CodeBuffer_t * cb, REGISTERS dest_reg, int32_t disp, int64_t imm) 
{
    uint8_t rex = 0x48;
    if (dest_reg >= 8) rex |= 0x1;

    uint8_t opcode = 0xC7;

    uint8_t mod = 0x00;
    uint8_t reg = 0x00;
    uint8_t rm  = dest_reg  & 0x07;

    if (disp == 0 && (dest_reg & 0x7) != 0x5) 
    // Без смещения и не rbp
            mod = 0x00;
    else if (disp >= INT8_MIN && disp <= INT8_MAX)
    // 8 битное смещение
            mod = 0x01;
    else
    // 32 битное смещение
            mod = 0x02;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);

    if (mod == 0x01)
    // 8 битное смещение
        {
            int8_t disp8 = (int8_t) disp;
            code_buffer_write (cb, (uint8_t *) disp8, 1);
        } 
    // 32 битное смещение
        else if (mod == 0x02)
            code_buffer_write (cb, (uint8_t *) &disp, 4);

    int32_t imm32 = (int32_t) imm;
    code_buffer_write(cb, (uint8_t *) &imm32, 4);
}

void emit_mov_r8_imm8 (CodeBuffer_t * cb, REGISTERS reg, uint8_t imm)
// только для AL-DL
{
    uint8_t opcode = 0xB0 + reg;
    uint8_t bytes[] = {opcode, imm};
    code_buffer_write(cb, bytes, 2);
}

void emit_mov_r8_mem (CodeBuffer_t * cb, REGISTERS dest_reg, REGISTERS base_reg, int32_t disp) 
// только для AL-DL
{
    uint8_t rex = 0x40;

    uint8_t opcode = 0x8A;

    uint8_t mod = 0x00;
    uint8_t reg = dest_reg & 0x07;
    uint8_t rm  = base_reg & 0x07;

    if (disp == 0) 
    // Без смещения
            mod = 0x00;
    else if (disp >= INT8_MIN && disp <= INT8_MAX)
    // 8 битное смещение
            mod = 0x01;
    else
    // 32 битное смещение
            mod = 0x02;

    uint8_t modrm = (mod << 6) | (reg << 3) | rm;

    uint8_t bytes[] = {rex, opcode, modrm};
    code_buffer_write(cb, bytes, 3);

    if (mod == 0x01)
    // 8 битное смещение
        {
            int8_t disp8 = (int8_t) disp;
            code_buffer_write (cb, (uint8_t *) disp8, 1);
        } 
    // 32 битное смещение
        else if (mod == 0x02)
            code_buffer_write (cb, (uint8_t *) &disp, 4);
}
//============================== EMITTERS ZONE END ==============================//