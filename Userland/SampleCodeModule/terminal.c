#include <terminal.h>
#include <colors.h>
#include <syscalls.h>
#include <calls.h>

#define COMMANDSQUANTITY 10

static char* commands[]={"help","time","date","registers","fillregisters","divideby0","invalidoperation","snake","snake2","clear"};
static char* commandsList={
    "\thelp: Muestra una lista de comandos posibles\n",
	"\ttime: Imprime la hora del SO\n",
	"\tdate: Imprime la fecha del SO\n",
	"\tregisters: Imprime el estado de los registros actuales\n",
	"\tfillregs: Llena los registros para probar correcto funcionamiento\n",
	"\tdiv0: Divide por cero para correr una excepcion\n",
	"\tinvalidop: Ejecutar excepcion de operacion Invalida\n",
	"\tsnake: Ejecutar el juego snake de a 1 jugador\n",
    "\tsnake2: Ejecutar el juego snake de a 2 jugadores\n",
	"\tclear: Limpia pantalla del SO\n"
};

void terminal(){
    print("Bienvenidos a venomOS. Escriba \"help\" para una lista de comandos\n");
    print("\n venomOS: ");
    char flag=0;
    int count = 0;	
	char buffer[1024] = {0};
	char oldBuffer[1024] = {0};
while (1) {
    unsigned char c = getChar();

    if (c == '\n') {
        buffer[count] = 0;
        analizeBuffer(buffer, count);
        printWithColor("\nvenomOS:  ", RED);
        strcpy(oldBuffer, buffer);
        count = 0;
        flag=1;
    } else if (c == '\b') {
        if (count > 0) {
            printChar(c);
            count--;
        }
    } else if (c == '\t') { 
        // Analizar las primeras 'count' letras del buffer y verificar si coinciden con algún comando para autocompletar el comando.
        int i = 0;
        while (i < COMMANDSQUANTITY && !strncmp(buffer, commands[i], count)) {
            i++;
        }
        if (i < COMMANDSQUANTITY) {
            while (commands[i][count] != 0) {
                printChar(commands[i][count]);
                buffer[count] = commands[i][count];
                count++;
            }
        }
    } else if (c == 17 && flag) {
        // Flecha arriba
        while (count > 0) {
            printChar('\b');
            count--;
        }
        strcpy(buffer, oldBuffer);
        count = strlen(buffer);
        print(buffer);
        flag = 0;
    } else if (c == 20 && !flag) {
        // Flecha abajo
        while (count > 0) {
            printChar('\b');
            count--;
        }
        flag = 1;
    } else if (c > 20 && c < 127) {
        printChar(c);
        buffer[count++] = c;
        buffer[count] = 0;
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

    if (commandMatch(buffer, "help", count) || commandMatch(buffer, "HELP", count)) {
        print("\n\nComandos disponibles:\n\n");
        for (int i = 0; i < COMMANDSQUANTITY; i++) {
            printColor(commands[i], RED);
        }
    } else if (commandMatch(buffer, "time", count)) {
        printfColor("\n\nTime of OS: ", RED);
        printfColor("%s\n", RED, getTime());
    } else if (commandMatch(buffer, "date", count)) {
        printfColor("\n\nDate of OS: ", YELLOW);
        printfColor("%s\n", RED, getDate());
    } else if (commandMatch(buffer, "registers", count)) {
        printRegs();
    } else if (commandMatch(buffer, "fillregs", count)) {
        fillRegisters();
    } else if (commandMatch(buffer, "clear", count)) {
        sys_clear_screen();
    } else if (commandMatch(buffer, "snake2", count)) {
        snake2();
    } else if (commandMatch(buffer, "div0", count)) {
        divideByZero();
    } else if (commandMatch(buffer, "invalidop", count)) {
        invalidOpcode();
    } else if (commandMatch(buffer, "snake", count)) {
       snake();
    } else {
        printf("\nComando no encontrado, escriba \"help\" para mas ayuda\n");
    }
}
