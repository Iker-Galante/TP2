#include <stdint.h>
#include "include/videoDriver.h"
#include "include/defs.h"
#include "include/syscall.h"
#include "include/keyboard.h"
#include "include/defs.h"
#include "include/sound.h"
#include "include/time.h"
#include "include/clock.h"
#include "include/colores.h" //REVISAR DUDOSO


//Ver nombres de func y cambiarlos para adecaurlos a los de userland
extern const uint64_t registers[17]; //Funcion en assembly

void syscallHandler(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    switch(id) {
        case 0:
            sys_read(arg0, arg1, arg2);
            break;
        case 1:
            sys_print(arg0, arg1, arg2);
            break;
        case 2:
            sys_print_color(arg0, arg1, arg2, arg3);
            break;
        case 3:
            sys_get_registers(arg0);
            break;
        case 4:
            sys_get_time(arg0);
            break;
        case 5:
            sys_get_date(arg0);
            break;
        case 6:
            sys_clear_screen();
            break;
        case 7:
            sys_draw_rectangle(arg0, arg1, arg2, arg3, arg4);
            break;
        case 8:
            sys_play_sound(arg0, arg1, arg2);
            break;
        case 9:
            sys_wait(arg0);
            break;
    }
    
}


static int64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length) {
    //No deberia ser necesario, pero por las dudas
    if (fd != STDIN) 
        return -1;
    int i = 0;
    char c;
    char * buff = (char *) buffer;
    while(i < length && (c = getChar()) != 0) { //0 es el fin de linea
        buff[i] = c;
        i++;
    }
    return i;
}

static void sys_print(uint64_t fd, uint64_t buffer, uint64_t length) {
    if (fd == STDOUT) {
        printStringN((char *) buffer, length);
    } else if (fd == STDERR) {
        printStringNColor((char *) buffer, length,(uint32_t) 0xFF0000);
    }
}


static void sys_print_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color) {
    //Imprimo en pantalla si es salida estandar o error
    if (fd == STDOUT || fd == STDERR) {
        printStringNColor((char *) buffer, length,(uint32_t) color);
    }
}

static void sys_get_registers(uint64_t regsBuff) {
    //Guargo los registros en el buffer, sacados en codigo assembly
    for(int i = 0; i < 17; i++) {
        ((uint64_t *)regsBuff)[i] = registers[i];
    }
}

static void sys_get_time(uint64_t buffer) {
    timeToStr((char *) buffer);
}

static void sys_get_date(uint64_t buffer) {
    dateToStr((char *) buffer);
}

static void sys_clear_screen() {
    clearScreen();
}

static void sys_draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color) {
    drawRectangle( (int) x, (int) y, (int) width, (int) height, (int) color );
}

static void sys_play_sound(uint64_t freq, uint64_t duration, uint64_t waitAfter) {
    playNoteSound((int) freq, (int) duration, (int) waitAfter);
}


static void sys_get_ticks(uint64_t ticks) {
    uint32_t * t = (uint32_t *) ticks;    
    *t = ticks_elapsed();
}


static void sys_wait(uint64_t ms){

    if (ms > 0){
        int start_ms = ticks_elapsed();
        do { _hlt(); } while (ticks_elapsed() - start_ms < ms);
    }
}