section .note.GNU-stack noexec
section .text

global _start

extern MyPrintf

_start:
		call .main

		mov rax, 60
		mov rdi, 0
		syscall

.main:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  r15               ;    |--> создаем сегмент переменных
		add  r15, -8            ;----
		mov r13, r15           ;------
		sub  r13, 32           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================

		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_7
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax
		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_12
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax
		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_17
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; EQUALSE

; Start INPUT

		push rbx
		push r14
		push rsi
		push rdi
		push rdx
		call input_func
		pop rax
		pop rdx
		pop rdi
		pop rsi
		pop r14
		pop rbx
		push rax
; end

		pop qword [r15 - 0]
; end EQUALS

; EQUALSE

; Start INPUT

		push rbx
		push r14
		push rsi
		push rdi
		push rdx
		call input_func
		pop rax
		pop rdx
		pop rdi
		pop rsi
		pop r14
		pop rbx
		push rax
; end

		pop qword [r15 - 8]
; end EQUALS

; EQUALSE

; Start INPUT

		push rbx
		push r14
		push rsi
		push rdi
		push rdx
		call input_func
		pop rax
		pop rdx
		pop rdi
		pop rsi
		pop r14
		pop rbx
		push rax
; end

		pop qword [r15 - 16]
; end EQUALS
		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_34
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax
		push r15
		push r14
		push rsp
		push qword [r15 - 16]           ; переменная: third_alko
		push qword [r15 - 8]           ; переменная: second_alko
		push qword [r15 - 0]           ; переменная: first_alko
		lea rsi, STRI_NUM_39
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 40
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax
		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_50
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 0]           ; переменная: first_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_1

		push qword [r15 - 8]           ; переменная: second_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_1

		push qword [r15 - 16]           ; переменная: third_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_1

; ========================= END IF CONDITIONAL =========================
		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_70
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_1:
	.end_if_0:
; end IF


; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 0]           ; переменная: first_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_3

; ========================= END IF CONDITIONAL =========================
		push r15
		push r14
		push rsp
		push qword [r15 - 8]           ; переменная: second_alko
		push qword [r15 - 16]           ; переменная: third_alko

		call .without_krepkoe

		pop rax
		push qword [r15 + 8]
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_3:
	.end_if_2:
; end IF


; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 16]           ; переменная: third_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_5

; ========================= END IF CONDITIONAL =========================
		push r15
		push r14
		push rsp
		push qword [r15 - 0]           ; переменная: first_alko
		push qword [r15 - 8]           ; переменная: second_alko

		call .without_krepkoe

		pop rax
		push qword [r15 + 8]
		pop rsp
		pop r14
		pop r15
		push rax
		push r15
		push r14
		push rsp
		push 0
		lea rsi, STRI_NUM_111
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 24
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_5:
	.end_if_4:
; end IF


; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 8]           ; переменная: second_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_7

; ========================= END IF CONDITIONAL =========================

; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 16]           ; переменная: third_alko
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jle .if_9

; ========================= END IF CONDITIONAL =========================
		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_136
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_9:
	.end_if_8:
; end IF


; EQUALSE


; Start taking the square root


; Make: DIV


; Make: MUL

		push -1
		push qword [r15 - 16]           ; переменная: third_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		push qword [r15 - 0]           ; переменная: first_alko
		pop r11
		pop rax
		cqo
		idiv r11
		push rax

; end DIV

		call sqrt_by_Newton
; end SQRT

		pop qword [r15 - 24]
; end EQUALS
		push r15
		push r14
		push rsp
		push qword [r15 - 24]           ; переменная: resultat
		push qword [r15 - 24]           ; переменная: resultat
		lea rsi, STRI_NUM_158
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 32
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_7:
	.end_if_6:
; end IF

		push r15
		push r14
		push rsp
		push qword [r15 - 0]           ; переменная: first_alko
		push qword [r15 - 8]           ; переменная: second_alko
		push qword [r15 - 16]           ; переменная: third_alko

		call .discriminator

		pop rax
		push qword [r15 + 8]
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


.without_krepkoe:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  r15               ;    |--> создаем сегмент переменных
		add  r15, 8            ;----
		mov r13, r15           ;------
		sub  r13, 24           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


; EQUALSE


; Make: MUL


; Make: DIV

		push qword [r15 - 8]           ; переменная: third_alko
		push qword [r15 - 0]           ; переменная: second_alko
		pop r11
		pop rax
		cqo
		idiv r11
		push rax

; end DIV

		push -1
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		pop qword [r15 - 16]
; end EQUALS
		push r15
		push r14
		push rsp
		push qword [r15 - 16]           ; переменная: resultat
		lea rsi, STRI_NUM_202
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 24
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


.discriminator:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  r15               ;    |--> создаем сегмент переменных
		add  r15, 16            ;----
		mov r13, r15           ;------
		sub  r13, 48           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


; EQUALSE


; Make: SUB

		push qword [r15 - 8]           ; переменная: second_alko
		push 2
; exponentiation

		pop rcx
		pop rdx
		mov rax, 1
	.loop_0:
		imul rax, rdx
		loop .loop_0
		push rax
		; end POW


; Make: MUL


; Make: MUL

		push 4
		push qword [r15 - 0]           ; переменная: first_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		push qword [r15 - 16]           ; переменная: third_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		xor r11, r11
		xor r12, r12
		pop r11
		pop r12
		sub r12, r11
		push r12
; end SUB

		pop qword [r15 - 24]
; end EQUALS
		push r15
		push r14
		push rsp
		push qword [r15 - 24]           ; переменная: itogo
		lea rsi, STRI_NUM_235
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 24
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; EQUALSE

		push 0
		pop qword [r15 - 32]
; end EQUALS

; EQUALSE

		push 0
		pop qword [r15 - 40]
; end EQUALS

; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 24]           ; переменная: itogo
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jle .if_11

; ========================= END IF CONDITIONAL =========================

; EQUALSE


; Make: DIV


; Make: ADD


; Make: MUL

		push -1
		push qword [r15 - 8]           ; переменная: second_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL


; Start taking the square root

		push qword [r15 - 24]           ; переменная: itogo
		call sqrt_by_Newton
; end SQRT

		xor r11, r11
		xor r12, r12
		pop r11
		pop r12
		add r12, r11
		push r12
; end ADD


; Make: MUL

		push 2
		push qword [r15 - 0]           ; переменная: first_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		pop r11
		pop rax
		cqo
		idiv r11
		push rax

; end DIV

		pop qword [r15 - 32]
; end EQUALS

; EQUALSE


; Make: DIV


; Make: SUB


; Make: MUL

		push -1
		push qword [r15 - 8]           ; переменная: second_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL


; Start taking the square root

		push qword [r15 - 24]           ; переменная: itogo
		call sqrt_by_Newton
; end SQRT

		xor r11, r11
		xor r12, r12
		pop r11
		pop r12
		sub r12, r11
		push r12
; end SUB


; Make: MUL

		push 2
		push qword [r15 - 0]           ; переменная: first_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		pop r11
		pop rax
		cqo
		idiv r11
		push rax

; end DIV

		pop qword [r15 - 40]
; end EQUALS
		push r15
		push r14
		push rsp
		push qword [r15 - 40]           ; переменная: secs_res
		push qword [r15 - 32]           ; переменная: firt_res
		lea rsi, STRI_NUM_299
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 32
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_11:
	.end_if_10:
; end IF


; Start IF code

; ========================= START IF CONDITIONAL =========================
		push qword [r15 - 24]           ; переменная: itogo
		push 0
		pop r11
		pop r12
		cmp r12, r11
		jne .if_13

; ========================= END IF CONDITIONAL =========================

; EQUALSE


; Make: DIV


; Make: MUL

		push -1
		push qword [r15 - 8]           ; переменная: second_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL


; Make: MUL

		push 2
		push qword [r15 - 0]           ; переменная: first_alko
		pop r11
		pop rax
		imul rax, r11
		push rax

; end MUL

		pop r11
		pop rax
		cqo
		idiv r11
		push rax

; end DIV

		pop qword [r15 - 32]
; end EQUALS
		push r15
		push r14
		push rsp
		push qword [r15 - 32]           ; переменная: firt_res
		push qword [r15 - 32]           ; переменная: firt_res
		lea rsi, STRI_NUM_335
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 32
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


	.if_13:
	.end_if_12:
; end IF

		push r15
		push r14
		push rsp
		lea rsi, STRI_NUM_348
		push rsi

		call MyPrintf

		pop rax
		push rsp
		pop rbx
		add rbx, 16
		push rbx
		pop rsp
		pop r14
		pop r15
		push rax

; RETURN

		push qword [r15 - 0]
		push r14
		ret
; end RETURN


sqrt_by_Newton:
		pop r9
		pop rax
		mov r8, rax
		mov rcx, rax
		.newton_loop:
		mov rax, r8
		xor rdx, rdx
		div rcx
		add rax, rcx
		shr rax, 1
		cmp rax, rcx
		jae .done
		mov rcx, rax
		jmp .newton_loop
		.done:
		push rcx
		push r9
		ret
input_func:
		mov rax, 0
		mov rdi, 0
		lea rsi, INPUT_BUFFER  ; buffer to store input
		mov rdx, 32            ; num bytes to read
		syscall

; +==============================================================+
; | RAX <-> stores result                                        |
; +--------------------------------------------------------------+
; | RBX <-> stores temp storage                                  |
; +--------------------------------------------------------------+
; | RSI <-> stores BUFFER address                                |
; +==============================================================+

		xor rax, rax
		xor rbx, rbx
		xor r8, r8
		lea rsi, INPUT_BUFFER

		mov bl, [rsi]
		cmp bl, '-'
		jne .loop
		mov r8, 1
		inc rsi

.loop:                     ; convert ASCII digits to number
		mov bl, [rsi]
		cmp bl, 0
		je .success
		cmp bl, 10             ; check for newline
		je .success
		sub bl, '0'
		cmp bl, 9
		ja .error
		imul rax, 10
		add rax, rbx
		inc rsi
		jmp .loop

.success:
		cmp r8, 1
		jne .posit
		neg rax
.posit
		pop rbx
              ; save ret address
		push rax
		push rbx
		ret

.error:
		mov rax, -1
		ret
section .data

	mem: 

 		 times 8 dq 0
	INPUT_BUFFER: times 32 dq 0
	STRI_NUM_7: db 'Enter coefficients ', 0
	STRI_NUM_12: db 'of the equation (a * x', 0
	STRI_NUM_17: db ' ^ 2 + b * x + c): ', 0
	STRI_NUM_34: db 'Your equation was:', 0
	STRI_NUM_39: db ' %d*x^2 + %d*x + %d; ', 0
	STRI_NUM_50: db 'and the answer is ', 0
	STRI_NUM_70: db 'Infitity roots ', 0
	STRI_NUM_111: db ', %d ', 0
	STRI_NUM_136: db 'no roots ', 0
	STRI_NUM_158: db '%d and -%d ', 0
	STRI_NUM_202: db '%d ', 0
	STRI_NUM_235: db '< %d - discriminant > ', 0
	STRI_NUM_299: db '%d and %d ', 0
	STRI_NUM_335: db '%d and %d ', 0
	STRI_NUM_348: db 'no roots ', 0
