GLOBAL cpuVendor

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
  	mov rax, 0

  	in al, 0x60
 	movzx eax, al    
  	mov rsp, rbp
  	pop rbp
  	ret

	; Start Clock

clock:
	push rbp
	mov rbp, rsp

	mov al, dil
	out 70h, al
	xor rax, rax
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

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