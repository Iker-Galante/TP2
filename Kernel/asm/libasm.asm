GLOBAL cpuVendor 
GLOBAL getKey
GLOBAL clock
GLOBAL inb
GLOBAL outb

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


getKey:
	push rbp
	mov rbp, rsp

	in al, 0x60

	mov rsp, rbp
	pop rbp
	ret


; Start Clock

clock:
    push rbp        ; Save the base pointer
    mov rbp, rsp    ; Set up the stack frame

    mov al, dil     ; Move the value from the lower byte of the parameter dil into al
    out 70h, al     ; Output the value of al to the CMOS address port (70h)

    xor rax, rax    ; Clear rax (setting it to 0)
    in al, 71h      ; Read the value from the CMOS data port (71h) into al

    mov rsp, rbp    ; Restore the stack pointer
    pop rbp         ; Restore the base pointer
    ret             ; Return from the function

; End Clock 


; Used for sound
inb:
	push rbp
	mov rbp, rsp
	mov rdx, rdi
	mov rax, 0
    in al, dx
	mov rsp, rbp
	pop rbp
	ret

outb:
	push rbp
	mov rbp, rsp
	mov rax, rsi
	mov rdx, rdi
	out dx, al
	mov rsp, rbp
	pop rbp
	ret