#include "include/videoDriver.h"
#include "include/keyboard.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_EXCEPTION_ID 6

extern const uint64_t excepRegs[18];
static void zero_division();
static void invalid_op();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else if(exception==INVALID_OP_EXCEPTION_ID)
		invalid_op();
}

static void zero_division() {
	printStringColor("\n\n No se puede dividir por 0\n\n",0xFF0000);
	printRegisters();
    printString("Espere para volver a terminal \n \n");
        for(unsigned long int i=0; i<2000000000;) //Para esperar y poder leer pantalla
                i++;
}
static void invalid_op(){
	printStringColor("\n\nOperacion invalida\n\n", 0xFF0000);
    printRegisters();
    printString("Espere para volver a terminal \n \n");
        for(unsigned long int i=0; i<2000000000;) 
                i++;
}

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
void printRegisters(){
    char * regs[] = {"rip ","rax ", "rbx ", "rcx ", "rdx ", "rsi ", "rdi ", "rbp ", "rsp ", "r8  ", "r9  ", "r10 ", "r11 ", "r12 ",
        "r13 ", "r14 ", "r15 ", "rflags"};
    printString("\n");
    for (int i = 0; i < 18; i++) {
        char hex[17];
        intToHexa(excepRegs[i], hex);
        printStringColor(regs[i], 0xFF0000);
        printStringColor(hex, 0xFF0000);
        printString("\n");
    }
	printString("\n");
}