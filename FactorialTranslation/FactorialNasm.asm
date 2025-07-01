global main

section .note.GNU-stack noexec
section .text

main:
		call .entry
		jmp endprog
.entry:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  r15               ;    |--> создаем сегмент переменных
		add  r15, -8            ;----
		mov r13, r15           ;------
		sub  r13, 8           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


; EQUALSE

		push r15
		push r14
		push rsp
		push 4

		call .factorial

		pop rax
		push qword [r15 + 8]
		pop rsp
		pop r14
		pop r15
		push rax
		pop qword [r15 - 0]
; end EQUALS

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


.factorial:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  r15               ;    |--> создаем сегмент переменных
		add  r15, 0            ;----
		mov r13, r15           ;------
		sub  r13, 24           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


; EQUALSE

		push qword [r15 - 0]           ; переменная: num
		pop qword [r15 - 8]
; end EQUALS

; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 0]           ; переменная: num
		push 1
		pop r11
		pop r12
		cmp r12, r11
		jbe .if_1

; ========================= END IF CONDITIONAL =========================

; EQUALSE


; Make: SUB

		push qword [r15 - 0]           ; переменная: num
		push 1
		xor r11, r11
		xor r12, r12
		pop r11
		pop r12
		sub r12, r11
		push r12
; end SUB

		pop qword [r15 - 0]
; end EQUALS

; EQUALSE

		push r15
		push r14
		push rsp
		push qword [r15 - 0]           ; переменная: num

		call .factorial

		pop rax
		push qword [r15 + 8]
		pop rsp
		pop r14
		pop r15
		push rax
		pop qword [r15 - 16]
; end EQUALS

; EQUALSE


; Make: MUL

		push qword [r15 - 8]           ; переменная: mnogo
		push qword [r15 - 16]           ; переменная: mnojit
		pop r11
		pop rax
		mul r11
		push rax

; end MUL

		pop qword [r15 - 8]
; end EQUALS

	.if_1:
	.end_if_0:
; end IF


; RETURN

		push qword [r15 - 8]
		push r14
		ret
; end RETURN


endprog:
		mov rax, 60
		mov rdi, 0
		syscall

section .data

	mem: 

 		 times 8 dq 0