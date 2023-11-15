
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _int80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL registers
GLOBAL excepRegs

GLOBAL save_og_regs

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallHandler

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exceptionHandler 1
	; Guarda el valor de RAX en la posición 1 de excepRegs
    mov [excepRegs + (1*8)], rax    ; rax
    ; Captura el valor de la pila (RSP) en RAX y lo guarda como RIP en excepRegs
    mov rax, [rsp]
    mov [excepRegs], rax            ; rip

    ; Guarda los valores de RBX, RCX, RDX, RSI, RDI, RBP en excepRegs
    mov [excepRegs + (2*8)], rbx
    mov [excepRegs + (3*8)], rcx
    mov [excepRegs + (4*8)], rdx
    mov [excepRegs + (5*8)], rsi
    mov [excepRegs + (6*8)], rdi
    mov [excepRegs + (7*8)], rbp
    
	; Captura el valor de la pila en RAX y lo guarda como RSP en excepRegs
	mov rax, [rsp + 24]  ; Se suman 24 bytes (8 bytes de RIP, 8 bytes de CS y 8 bytes de RFLAGS)
	mov [excepRegs + (8*8)], rax    ; rsp
    ; Guarda los valores de R8, R9, R10, R11, R12, R13, R14, R15 en excepRegs
    mov [excepRegs + (9*8)], r8
    mov [excepRegs + (10*8)], r9
    mov [excepRegs + (11*8)], r10 
    mov [excepRegs + (12*8)], r11 
    mov [excepRegs + (13*8)], r12 
    mov [excepRegs + (14*8)], r13 
    mov [excepRegs + (15*8)], r14 
    mov [excepRegs + (16*8)], r15 
    ; Captura el valor de RFLAGS en RAX y lo guarda en excepRegs
    mov rax, [rsp+16] ; Valor de RFLAGS (empujado al ocurrir una interrupción).
    mov [excepRegs + (17*8)], rax    ; rflags

    ; Prepara el argumento para la llamada a la función exceptionDispatcher
    mov rdi, %1 ; Paso de parámetro
    mov rsi, excepRegs
    call exceptionDispatcher

	;Restoring original registers
	mov rbx, [ogRegs+8*1]	; RBX
	mov rbp, [ogRegs+8*2]	; RBP
	mov r12, [ogRegs+8*3]	; R12
	mov r13, [ogRegs+8*4]	; R13
	mov r15, [ogRegs+8*5]	; R15
	
	mov rax, [ogRegs+8*7]	;RIP
	mov [rsp], rax

	mov rax, [ogRegs+8*6]	;RSP
	add rax, 10h
	mov [rsp+24], rax

    iretq ; SS, RSP, RFLAGS, CS, RIP
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

saveRegisters:
; Movimiento de registros a la región reservada de memoria 'registers'
	mov [registers + (1*8)], rbx ; Se almacena el contenido de RBX
	mov [registers + (2*8)], rcx ; Se almacena el contenido de RCX
	mov [registers + (3*8)], rdx ; Se almacena el contenido de RDX
	mov [registers + (4*8)], rsi ; Se almacena el contenido de RSI
	mov [registers + (5*8)], rdi ; Se almacena el contenido de RDI
	mov [registers + (6*8)], rbp ; Se almacena el contenido de RBP
	mov [registers + (8*8)], r8  ; Se almacena el contenido de R8
	mov [registers + (9*8)], r9  ; Se almacena el contenido de R9
	mov [registers + (10*8)], r10 ; Se almacena el contenido de R10
	mov [registers + (11*8)], r11 ; Se almacena el contenido de R11
	mov [registers + (12*8)], r12 ; Se almacena el contenido de R12
	mov [registers + (13*8)], r13 ; Se almacena el contenido de R13
	mov [registers + (14*8)], r14 ; Se almacena el contenido de R14
	mov [registers + (15*8)], r15 ; Se almacena el contenido de R15

	; Almacenamiento de RSP y RIP después de operaciones específicas
	mov rax, [rsp + 18*8]       ; Se carga RSP+18*8 en RAX
	mov [registers + (7*8)], rax ; Se almacena el contenido de RAX en [registers + (7*8)] (RSP)

	mov rax, [rsp + 15*8]        ; Se carga RSP+15*8 en RAX
	mov [registers + (16*8)], rax ; Se almacena el contenido de RAX en [registers + (16*8)] (RIP)

	mov rax, [rsp + 14*8] ; Se carga RSP+14*8 en RAX (RAX)
	mov [registers], rax  ; Se almacena el contenido de RAX en el primer registro
	
	; Finalización de la interrupción con un comando de Fin de Interrupción (EOI)
	mov al, 20h      ; Se carga el valor 20h en AL
	out 20h, al      ; Se envía el valor de AL al puerto de EOI (End of Interrupt)

	popState         ; Operación para restaurar el estado previo
	iretq            ; Instrucción para retornar desde la interrupción (SS, RSP, RFLAGS, CS, RIP)


	save_og_regs: ;BORRAR NUNCA LLEGA
	; Funcion para salvar los registros RBX, RBP, R12, R13, R15, RSP, RIP
	mov [ogRegs+8*1], rbx
	mov [ogRegs+8*2], rbp
	mov [ogRegs+8*3], r12
	mov [ogRegs+8*4], r13
	mov [ogRegs+8*5], r15
	mov [ogRegs+8*6], rsp	 ; RSP
	mov rax, [rsp]   		 ; RSP contiene la direccion de retorno, para obtener el RIP
	mov [ogRegs+8*7], rax
	ret


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	pushState

	in al, 0x60
	cmp al, 0x1D ; Left Control key
	je saveRegisters

	mov rdi, 1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Syscalls
_int80Handler:
	pushState
	call syscallHandler
	popState
	iretq

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcde Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	registers resq 17 ; registros para pantalla
	excepRegs resq 18 ; registros para excepciones
	ogRegs resq 17		;Vector orignial de registros