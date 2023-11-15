#include "colors.h"
#include <stdint.h>

void print(char * string);
void printWithColor(char * string,uint64_t color);
char getChar();
int strlen(char* v);
void strcpy(char *dest, char* source);
void printChar(char c);
void printf(char* str,...);
void numToString(int num, char* str);
void intToHexa(uint64_t num, char* hexa);
void printCharWithColor(char c, uint64_t color);
char* getTime();
char* getDate();
void printRegisters();
void fillRegisters();
void zoomIn();
void zoomOut();
