#include <colors.h>
#include <calls.h>
#include <stdint.h>
#include <syscalls.h>
#include <string.h>

void print(char * cadena){
    sys_print(1,cadena,strlen(cadena));
}

void printWithColor(char * cadena,uint64_t color){
    sys_print_color(1,cadena,strlen(cadena),color);
}