#include "include/usyscalls.h"
//Debo castear a uint64_t los parametros para que no se rompa el stack
extern uint64_t sys_call(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t , uint64_t);

uint64_t sys_read(unsigned int fd, char * buffer, unsigned int size){
    return sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);    
}
    
uint64_t sys_print(unsigned int fd, char* buffer, unsigned int size) {
    return sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_print_color(unsigned int fd,  char* buffer, unsigned int size, uint64_t color) {
    return sys_call((uint64_t) 2, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, color, (uint64_t) 0);
}

uint64_t sys_clean_screen(){
    return sys_call((uint64_t)6, (uint64_t)0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}
uint64_t sys_time(char *buffer){
    return sys_call((uint64_t)4, (uint64_t)buffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}
uint64_t sys_date(char* buffer){
     return sys_call((uint64_t)5, (uint64_t)buffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}
uint64_t sys_registers(uint64_t * registerBuffer){
    return sys_call((uint64_t) 3, (uint64_t) registerBuffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);

uint64_t sys_play_sound(int freq, int duration, int wait) {
    return sys_call((uint64_t)8, (uint64_t) freq, (uint64_t) duration, (uint64_t) wait, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_wait(uint64_t ms){
    return sys_call((uint64_t)9, (uint64_t) ms, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

}
uint64_t sys_draw_rectangle(int x, int y, uint32_t color) {
    return sys_call((uint64_t) 7, (uint64_t) x, (uint64_t) y, (uint64_t) color, (uint64_t) 0, (uint64_t) 0);
}
uint64_t sys_IncreasePixel() {
    return sys_call((uint64_t) 10, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}
uint64_t sys_draw_board(int x, int y, uint64_t color) {
    return sys_call((uint64_t) 11, (uint64_t) x, (uint64_t) y, (uint64_t) color, (uint64_t) 0, (uint64_t) 0);
}
uint64_t sys_change_cursor() {
    return sys_call((uint64_t) 12, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}