#include "include/colors.h"
#include "include/calls.h"
#include <stdint.h>
#include "include/usyscalls.h"
#include <stdarg.h>
#define WHITE 0xFFFFFF

extern void fillRegis();

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
int strlen(char* v){
    int i=0;
    for(;v[i]!='\0';)
        i++;
    return i;
}
void strcpy(char *dest, char * source){
    while(*source != 0)
        *dest++=*source++;
    *dest=0;
}
void numToString(int num, char* str){
    int i = 0;
    int negativo = 0;

    //Para ver si es negativo
    if (num < 0) {
        negativo = 1;
        num = -num;
    }

    // Convierto los digitos y los guardo en orden inverso
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Si es negativo le pongo un menos
    if (negativo) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Lo doy vuelta el str para que quede bien
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - 1 - j];
        str[len - 1 - j] = temp;
    }
}
//Covierto un numero de 64 bits a hexa
void intToHexa(uint64_t num, char* hex){
int i = 0;
    for (i = 15; i >= 0; i--) {
        int aux = num & 0xF;
        if (aux < 10) {
            hex[i] = aux + '0';
        } else {
            hex[i] = aux - 10 + 'A';
        }
        num >>= 4;
    }
    hex[16] = 0;
}

void printCharWithColor(char c, uint64_t color){
    char v[]={c};
    sys_print_color(1,v,1,color);
}

char bufferTime[9];
char* getTime(){
	sys_time(bufferTime);
	return bufferTime;
}

char bufferDate[9];
char* getDate(){
	sys_date(bufferDate);
	return bufferDate;
}
void printRegisters(){
    uint64_t regs[17];
    char * regsnames[] = {"rax ", "rbx ", "rcx ", "rdx ", "rsi ", "rdi ", "rbp ", "rsp ", "r8  ", "r9  ", "r10 ", "r11 ", "r12 ",
        "r13 ", "r14 ", "r15 ", "rip "};
    print("\n\n");
    sys_registers(regs);
    for (int i = 0; i < 17; i++) {
        char hex[17];
        intToHexa(regs[i], hex);
        printf("%s 0x%s\n", regsnames[i], hex);
    }
}
void fillRegisters(){
    print("\n\n Cargando los registros, pulse \"CTRL\" para guardar su valor\n");
    fillRegis();
}
void printf(char* format,...){ //Funcion de prinf con funcionamiento como stdio
    va_list args;
    va_start(args, format);
    int i = 0;
    while (format[i] != 0) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'd': {
                    int num = va_arg(args, int);
                    char str[11];
                    numToString(num, str);
                    printWithColor(str, WHITE);
                    break;
                }
                case 'x': {
                    int num = va_arg(args, int);
                    char str[9];
                    intToHexa(num, str);
                    printWithColor(str, WHITE);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    printWithColor(str, WHITE);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    printCharWithColor(c, WHITE);
                    break;
                }
                default:
                    printCharWithColor(format[i], WHITE);
                    break;
            }
        } else {
            printCharWithColor(format[i], WHITE);
        }
        i++;
    }
    va_end(args);
}
//TODO: Complete
void zoomIn(){
    // sys_IncreasePixel();
}
void zoomOut(){
    // sys_DecreasePixel();
}