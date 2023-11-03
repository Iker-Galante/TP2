#include <syscalls.h>
//Debo castear a uint64_t los parametros para que no se rompa el stack
extern uint64_t sys_call(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t , uint64_t);

uint64_t sys_print(unsigned int fd, char* buffer, unsigned int size) {
    return sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}
//Revisar pq puede estar mal el color
uint64_t sys_print_color(unsigned int fd,  char* buffer, unsigned int size, uint64_t color) {
    /* uint64_t c = color.b;
    c <<= 2;
    c += color.g;
    c <<= 2;
    c += color.r; */
    return sys_call((uint64_t) 2, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, color, (uint64_t) 0);
}

uint64_t sys_clean_screen(){
    return sys_call((uint64_t)6, (uint64_t)0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}