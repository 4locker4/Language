section .text

global _start

extern MyPrintf

_start:
		call .entry

		mov rax, 60
		mov rdi, 0
		syscall

.entry:

		pop  r14               ; r14 - сохраняем адрес возврата
		push rsp               ;----
		pop  r15               ;    |--> создаем сегмент переменных
		add  r15, -8            ;----
		mov r13, r15           ;------
		sub  r13, 24           ;      |--> новый адрес стека
		push r13               ;      |
		pop  rsp               ;------

; ========================== START FUNC ==========================


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

		push r15
		push r14
		push rsp
		push qword [r15 - 0]           ; переменная: fact_num

		call .factorial

		pop rax
		push qword [r15 + 8]
		pop rsp
		pop r14
		pop r15
		push rax
		pop qword [r15 - 8]
; end EQUALS

; EQUALSE

		lea rsi, STRI_NUM_18
		push rsi
		pop qword [r15 - 0]
; end EQUALS

; RETURN

		push qword [r15 - 8]
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
		jle .if_1

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
		imul rax, r11
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
	STRI_NUM_18: db 'durachok', 0
