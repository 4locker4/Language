global _start

section .note.GNU-stack noexec
section .text

_start:
		call .entry
		hlt
.entry:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  rbx               ;    |--> создаем сегмент переменных
		add  rbx, 0            ;----
		mov r13, rbx           ;------
		sub  r13, 8           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


; EQUALSE

		push rbx
		push r14
		push 4

		call .factorial

		pop rax
		push rsp
		pop r15
		add r15, 1
		push r15
		pop rsp
		pop r14
		pop rbx
		push rax
		pop qword [rbx + 0]
; end EQUALS

; RETURN

		push qword [rbx + 0]
		push r14
		ret
; end RETURN


		push r14                ; адрес возврата
		ret

.factorial:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  rbx               ;    |--> создаем сегмент переменных
		add  rbx, 8            ;----
		mov r13, rbx           ;------
		sub  r13, 24           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


; EQUALSE

		push word [rbx + 0]           ; переменная: num
		pop qword [rbx + 8]
; end EQUALS

; Start IF code

; ========================= START IF CONDITIONAL =========================
		push word [rbx + 0]           ; переменная: num
		push 1
		pop r11
		pop r12
		cmp r12, r11
		jbe .if_1

; ========================= END IF CONDITIONAL =========================

; EQUALSE


; Make: SUB

		push word [rbx + 0]           ; переменная: num
		push 1
		pop r11
		pop r12
		sub r12, r11
		push r12
; end SUB

		pop qword [rbx + 0]
; end EQUALS

; EQUALSE

		push rbx
		push r14
		push word [rbx + 0]           ; переменная: num

		call .factorial

		pop rax
		push rsp
		pop r15
		add r15, 3
		push r15
		pop rsp
		pop r14
		pop rbx
		push rax
		pop qword [rbx + 16]
; end EQUALS

; EQUALSE


; Make: MUL

		push word [rbx + 8]           ; переменная: mnogo
		push word [rbx + 16]           ; переменная: mnojit
		pop r11
		pop rax
		mul r11
		push rax

; end MUL

		pop qword [rbx + 8]
; end EQUALS

	.if_1:
	.end_if_0:
; end IF


; RETURN

		push qword [rbx + 8]
		push r14
		ret
; end RETURN


		push r14                ; адрес возврата
		ret

section .data

	mem: 

 		 times 8 dq 0