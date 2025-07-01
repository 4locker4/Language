#include "../inc/emitters.hpp"

int code_buffer_init (CodeBuffer_t* cb, FILE* file, uint64_t base_addr)
{
    cb->file = file;
    cb->base_addr = base_addr;
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

void code_buffer_free (CodeBuffer_t* cb)
{
    free (cb->buffer);
    cb->buffer_size = 0;
}

int code_buffer_write (CodeBuffer_t* cb, const uint8_t* bytes, size_t len)
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
//====================== Emitter for func call ======================//
void emit_call_func (CodeBuffer_t* cb, int offset)
{
    uint8_t rex = 0x48;                                 // REX.W
    uint8_t opcode = 0xE8;                              // 0x0F - for extended instructions | 0xAF - specific opcode for imul
    uint8_t modrm = (uint8_t) (offset);
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 4);
}

//========================= Emitter for ja =========================//
void emit_ja (CodeBuffer_t* cb, uint64_t offset)
{
    uint8_t opcode[] = {0x0F, 0x87};                              // opcode: jmp with 32-bit relative offset
    int32_t rel_offset = (int32_t) (offset - (cb->base_addr + cb->offset + 5));
    code_buffer_write (cb, opcode, 2);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//========================= Emitter for jb =========================//
void emit_jb (CodeBuffer_t* cb, uint64_t offset)
{
    uint8_t opcode[] = {0x0F, 0x82};                              // opcode: jmp with 32-bit relative offset
    int32_t rel_offset = (int32_t) (offset - (cb->base_addr + cb->offset + 5));
    code_buffer_write (cb, opcode, 2);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//========================= Emitter for jae ========================//
void emit_jae (CodeBuffer_t* cb, uint64_t offset)
{
    uint8_t opcode[] = {0x0F, 0x83};                              // opcode: jmp with 32-bit relative offset
    int32_t rel_offset = (int32_t) (offset - (cb->base_addr + cb->offset + 5));
    code_buffer_write (cb, opcode, 2);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//========================= Emitter for jbe ========================//
void emit_jbe (CodeBuffer_t* cb, uint64_t offset)
{
    uint8_t opcode[] = {0x0F, 0x86};                              // opcode: jmp with 32-bit relative offset
    int32_t rel_offset = (int32_t) (offset - (cb->base_addr + cb->offset + 5));
    code_buffer_write (cb, opcode, 2);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//========================= Emitter for je ========================//
void emit_je (CodeBuffer_t* cb, uint64_t offset)
{
    uint8_t opcode[] = {0x0F, 0x84};                              // opcode: jmp with 32-bit relative offset
    int32_t rel_offset = (int32_t) (offset - (cb->base_addr + cb->offset + 5));
    code_buffer_write (cb, opcode, 2);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//==================== Emitter for imul reg, reg ====================//
void emit_imul_reg_reg (CodeBuffer_t* cb, int dest_reg, int src_reg)
{
    uint8_t rex = 0x48;                                 // REX.W
    uint8_t opcode[] = { 0x0F, 0xAF };                  // 0x0F - for extended instructions | 0xAF - specific opcode for imul
    uint8_t modrm = (uint8_t) (0xC0 | (dest_reg << 3) | src_reg);
    uint8_t bytes[] = { rex, opcode[0], opcode[1], modrm };
    code_buffer_write (cb, bytes, 4);
}

//====================== Emitter for push r_x ======================//
void emit_push_r_x (CodeBuffer_t* cb, int dest_reg, bool is_stdt_reg)
{
    uint8_t bytes = -1;

    if (is_stdt_reg)
    {
        uint8_t bytes = 0x50 + dest_reg;
        code_buffer_write (cb, &bytes, 1);
    }
    else
    {
        uint8_t bytes = 0x4150 + dest_reg;
        code_buffer_write (cb, &bytes, 2);
    }
}

//====================== Emitter for push r_x ======================//
void emit_pop_r_x (CodeBuffer_t* cb, int dest_reg, bool is_stdt_reg)
{
    uint8_t bytes = -1;

    if (is_stdt_reg)
    {
        uint8_t bytes = 0x58 + dest_reg;
        code_buffer_write (cb, &bytes, 1);
    }
    else
    {
        uint8_t bytes = 0x4158 + dest_reg;
        code_buffer_write (cb, &bytes, 2);
    }
}

//====================== Emitter for push r_x ======================//
void emit_push_mem (CodeBuffer_t* cb, int dest_reg, bool is_stdt_reg)
{
    uint8_t opcode = dest_reg;
    
}

//==================== Emitter for sub reg, reg ====================//
void emit_sub_reg_reg (CodeBuffer_t* cb, int dest_reg, int src_reg)
{
    uint8_t rex = 0x48;                                 // REX.W
    uint8_t opcode = 0x2B;                              // opcode: sub when subtracting a register from a register.
    uint8_t modrm = (uint8_t) (0xC0 | (src_reg << 3) | dest_reg);
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

//==================== Emitter for add reg, reg ====================//
void emit_add_reg_reg (CodeBuffer_t* cb, int dest_reg, int src_reg)
{
    uint8_t rex = 0x48;                                 // REX.W
    uint8_t opcode = 0x01;                              // opcode: add register to register
    uint8_t modrm = (uint8_t) (0xC0 | (src_reg << 3) | dest_reg);
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
}

//==================== Emitter for cmp reg, imm && jle ====================//
void emit_cmp_jle (CodeBuffer_t* cb, int reg_num, int32_t imm, uint64_t offset)
{
    uint8_t rex = 0x48;                                 // REX.W
    uint8_t opcode = 0x81;                              // opcode: cmp with immediate.
    uint8_t modrm = (uint8_t) (0xF8 | reg_num);
    uint8_t bytes[] = { rex, opcode, modrm };
    code_buffer_write (cb, bytes, 3);
    code_buffer_write (cb, (uint8_t*)&imm, 4);

    int64_t rel_offset = (int64_t) (offset - (cb->base_addr + cb->offset + 6));
    uint8_t jle_opcode[] = { 0x0F, 0x8E };              // opcode: jle (jump if less than or equal).
    code_buffer_write (cb, jle_opcode, 2);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//==================== Emitter for jmp ====================//
void emit_jmp (CodeBuffer_t* cb, uint64_t offset)
{
    uint8_t opcode = 0xE9;                              // opcode: jmp with 32-bit relative offset
    int32_t rel_offset = (int32_t) (offset - (cb->base_addr + cb->offset + 5));
    code_buffer_write (cb, &opcode, 1);
    code_buffer_write (cb, (uint8_t*)&rel_offset, 4);
}

//==================== Emitter for ret ====================//
void emit_ret (CodeBuffer_t* cb)
{
    uint8_t opcode = 0xC3;                              // opcode: ret
    code_buffer_write (cb, &opcode, 1);
}

//============================== EMITTERS ZONE END ==============================//