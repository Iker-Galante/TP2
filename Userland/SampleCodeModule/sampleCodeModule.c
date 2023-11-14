/* sampleCodeModule.c */
#include<stdint.h>
#include"include/calls.h"
#include"include/colors.h"
#include"include/snake.h"
#include"include/usyscalls.h"
#include"include/terminal.h"

int main() {
	sys_clean_screen();
	print("Bienvenido a la consola del SO!, por favor ingrese s/t entre Snake y Terminal \n \n");
	unsigned char c;
	do{
		c=getChar();
	}while(c!='s' && c!='t');
	getChar();
	if(c=='s')
		snake();
	sys_clean_screen();
	terminal();
	return 0;
}