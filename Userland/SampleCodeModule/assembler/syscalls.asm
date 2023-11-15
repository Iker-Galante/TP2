section .text
GLOBAL sys_call

sys_call:
    push rbp
    mov rbp,rsp
    int 80h
    ;Desarmo Stack frame
    mov rsp,rbp
    pop rbp
    ret