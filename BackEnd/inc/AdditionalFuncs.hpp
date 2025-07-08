#pragma once

#include "../../inc/Language.h"

const size_t SIZE_OF_INPUT_BUFFER = 32;

/* Thies function needs an INPUT_BUFFER into .data section. Size of INPUT_BUFFER must be SIZE_OF_INPUT_BUFFER bytes. */

#define FPRINTF_INPUT_FUNC(file) fprintf (file, "input_func:\n"                                                                    \         
                                                "\t\tmov rax, 0\n"                                                                 \             
                                                "\t\tmov rdi, 0\n"                                                                 \             
                                                "\t\tlea rsi, INPUT_BUFFER  ; buffer to store input\n"                             \                                                 
                                                "\t\tmov rdx, 32            ; num bytes to read\n"                                 \                                             
                                                "\t\tsyscall\n"                                                                    \         
                                                "\n"                                                                               \ 
                                                "; +==============================================================+\n"             \                                                                 
                                                "; | RAX <-> stores result                                        |\n"             \                                                                 
                                                "; +--------------------------------------------------------------+\n"             \                                                                 
                                                "; | RBX <-> stores temp storage                                  |\n"             \                                                                 
                                                "; +--------------------------------------------------------------+\n"             \                                                                 
                                                "; | RSI <-> stores BUFFER address                                |\n"             \                                                                 
                                                "; +==============================================================+\n\n"           \                                                                     
                                                "\t\txor rax, rax\n"                                                               \                 
                                                "\t\txor rbx, rbx\n"                                                               \   
                                                "\t\txor r8, r8\n"                                                                 \           
                                                "\t\tlea rsi, INPUT_BUFFER\n\n"                                                    \
                                                "\t\tmov bl, [rsi]\n"                                                              \
                                                "\t\tcmp bl, '-'\n"                                                                \
                                                "\t\tjne .loop\n"                                                                  \
                                                "\t\tmov r8, 1\n"                                                                  \
                                                "\t\tinc rsi\n"                                                                    \
                                                "\n"                                                                               \ 
                                                ".loop:                     ; convert ASCII digits to number\n"                    \                                                         
                                                "\t\tmov bl, [rsi]\n"                                                              \                 
                                                "\t\tcmp bl, 0\n"                                                                  \             
                                                "\t\tje .success\n"                                                                \             
                                                "\t\tcmp bl, 10             ; check for newline\n"                                 \                                             
                                                "\t\tje .success\n"                                                                \             
                                                "\t\tsub bl, '0'\n"                                                                \             
                                                "\t\tcmp bl, 9\n"                                                                  \             
                                                "\t\tja .error\n"                                                                  \             
                                                "\t\timul rax, 10\n"                                                               \                 
                                                "\t\tadd rax, rbx\n"                                                               \                 
                                                "\t\tinc rsi\n"                                                                    \         
                                                "\t\tjmp .loop\n"                                                                  \             
                                                "\n"                                                                               \ 
                                                ".success:\n"                                                                      \
                                                "\t\tcmp r8, 1\n"                                                                  \
                                                "\t\tjne .posit\n"                                                                 \
                                                "\t\tneg rax\n"                                                                    \
                                                ".posit\n"                                                                         \
                                                "\t\tpop rbx\n              ; save ret address\n"                                  \                                             
                                                "\t\tpush rax\n"                                                                   \             
                                                "\t\tpush rbx\n"                                                                   \             
                                                "\t\tret\n"                                                                        \     
                                                "\n"                                                                               \ 
                                                ".error:\n"                                                                        \     
                                                "\t\tmov rax, -1\n"                                                                \             
                                                "\t\tret\n");

#define FPRINTF_SQRT_BY_NEWTON(file) fprintf (file, "sqrt_by_Newton:\n"               \
                                                    "\t\tpop r9\n"                    \  
                                                    "\t\tpop rax\n"                   \
                                                                                      \              
                                                    "\t\tmov r8, rax\n"               \      
                                                    "\t\tmov rcx, rax\n"              \          
                                                    "\t\t.newton_loop:\n"             \          
                                                    "\t\tmov rax, r8\n"               \      
                                                    "\t\txor rdx, rdx\n"              \          
                                                    "\t\tdiv rcx\n"                   \  
                                                    "\t\tadd rax, rcx\n"              \          
                                                    "\t\tshr rax, 1\n"                \      
                                                    "\t\tcmp rax, rcx\n"              \          
                                                    "\t\tjae .done\n"                 \      
                                                    "\t\tmov rcx, rax\n"              \          
                                                    "\t\tjmp .newton_loop\n"          \              
                                                    "\t\t.done:\n"                    \  
                                                    "\t\tpush rcx\n"                  \      
                                                    "\t\tpush r9\n"                   \  
                                                    "\t\tret\n");                     