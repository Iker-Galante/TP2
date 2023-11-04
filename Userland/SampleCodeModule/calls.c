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
void printChar(char c){
    char v[]={c};
    sys_print(1,v,1);
}
char getChar(){
    char v[]={0};
    sys_read(0,v,1);
    return v[0];
}
void strcpy(char *dest, char * source){
    while(*source != 0)
        *dest++=*source++;
    *dest=0;
}