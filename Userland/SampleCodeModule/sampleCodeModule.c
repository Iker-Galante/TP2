/* sampleCodeModule.c */
#include<stdint.h>
#include"include/calls.h"
#include"include/colors.h"

int main() {
	print("Bienvenido a la consola del SO!, por favor ingrese s/t entre Snake y Terminal \n \n");
	unsigned char c;
	do{
		c=getChar();
	}while(c!='s' && c!='t');
	if(c=='s')
		snake();
	sys_clean_screen();
	terminal();
	return 0;
}