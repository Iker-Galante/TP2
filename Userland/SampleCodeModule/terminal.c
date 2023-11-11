#include "include/terminal.h"
#include "include/colors.h"
#include "include/usyscalls.h"
#include "include/snake.h"
#include "include/calls.h"

#define COMMANDSQUANTITY 12

// static char* commands[]={"help","time","date","registers","fillregisters","divideby0","invalidoperation","snake","snake2","clear","zoomin","zoomout"};
static char* commandsList[]={
    "\thelp: Muestra una lista de comandos posibles\n",
	"\ttime: Imprime la hora del SO\n",
	"\tdate: Imprime la fecha del SO\n",
	"\tregisters: Imprime el estado de los registros actuales\n",
	"\tfillregs: Llena los registros para probar correcto funcionamiento\n",
	"\tdiv0: Divide por cero para correr una excepcion\n",
	"\tinvalidop: Ejecutar excepcion de operacion Invalida\n",
	"\tsnake: Ejecutar el juego snake de a 1 jugador\n",
    "\tsnake2: Ejecutar el juego snake de a 2 jugadores\n",
	"\tclear: Limpia pantalla del SO\n",
    "\tzoomIn: Aumentar tamaño de letra\n",
    "\tzoomOut: Achicar tamaño de letra\n"
};

void terminal(){
    print("Bienvenidos a venomOS. Escriba \"help\" para una lista de comandos\n");
    print("\n venomOS: ");
    int count = 0;	
	char newbuffer[1024] = {0};
	char oldBuffer[1024] = {0};
while (1) {
    unsigned char c = getChar();

    if (c == '\n') {
        newbuffer[count] = 0;
        analizeBuffer(newbuffer, count);
        printWithColor("\nvenomOS:  ", RED);
        strcpy(oldBuffer, newbuffer);
        count = 0;
    } else if (c == '\b') {
        if (count > 0) {
            printChar(c);
            count--;
        }
    } else if (c > 20 && c < 127) {
        printChar(c);
        newbuffer[count++] = c;
        newbuffer[count] = 0;
    }
}

}

int commandMatch(char *str1, char *command, int count) {
    if (count != strlen(command))
        return 0;

    for (int i = 0; i < count; i++) {
        if (str1[i] != command[i])
            return 0;
    }

    return 1;
}

void analizeBuffer(char *buffer, int count) {
    if (count <= 0) {
        return;
    }

    if (commandMatch(buffer, "help", count) /*|| commandMatch(buffer, "HELP", count)*/) {
        print("\n\nComandos disponibles:\n\n");
        for (int i = 0; i < COMMANDSQUANTITY; i++) {
            printWithColor(commandsList[i], RED);
        }
    } else if (commandMatch(buffer, "time", count)) {
        printf("\n\nHora del SO ", RED);
        printf("%s\n",getTime());
    } else if (commandMatch(buffer, "date", count)) {
        printf("\n\nFecha del SO ");
        printf("%s\n", getDate());
    } else if (commandMatch(buffer, "registers", count)) {
        printRegisters();
    } else if (commandMatch(buffer, "fillregisters", count)) {
        fillRegisters();
    } else if (commandMatch(buffer, "clear", count)) {
        sys_clean_screen();
    } else if (commandMatch(buffer, "snake2", count)) {
        snake2(); //Ver como implementar
    } else if (commandMatch(buffer, "divideby0", count)) {
        divbyZero();
    } else if (commandMatch(buffer, "invalidoperation", count)) {
        invalidOperationCode();
    } else if (commandMatch(buffer, "snake", count)) {
       snake(); //Ver como implementar
    } else if(commandMatch(buffer, "zoomin", count)) {
        zoomIn();
    } else if(commandMatch(buffer, "zoomout", count)) {
        zoomOut();
    } else {
        printf("\nComando no encontrado, escriba \"help\" para ver lista de comandos disponibles\n");
    }
}
