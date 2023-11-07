#include <stdint.h>
#include <videodriver.h>
#include <defs.h>
#include <syscalls.h>
#include <keyboard.h>
#include <clock.h>
#include <sound.h>
#include <time.h>
#include <colores.h>


//Ver nombres de func y cambiarlos para adecaurlos a los de userland
extern const uint64_t registers[17];

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
            sys_DecreasePixel();
            break;
        case 9:
            sys_IncreasePixel();
            break;
        case ....:
            sys_play_sound(arg0, arg1, arg2);
            break;
        case ....:
            sys_toggle_cursor();
            break;
        case ....:
            sys_get_ticks(arg0);
            break;
    }
    
}


static uint64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length) {
    if (fd != STDIN) 
        return -1;
    int i = 0;
    char c;
    char * buff = (char *) buffer;
    while(i < length && (c = getChar()) != 0) {
        buff[i] = c;
        i++;
    }
    return i;
}

static void sys_print(uint64_t fd, uint64_t buffer, uint64_t length) {
    if (fd == STDOUT) {
        printStringN((char *) buffer, length);
    } else if (fd == STDERR) {
        printStringNColor((char *) buffer, length, RED);
    }
}


static void sys_print_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color) {
    if (fd == STDOUT || fd == STDERR) {
        Color c;
        c.r = (char) color;
        c.g = (char) (color >> 8);
        c.b = (char) (color >> 16);
        printStringNColor((char *) buffer, length, c);
    }
}

static void sys_get_registers(uint64_t regsBuff) {
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

static void sys_draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color) {
    drawRectangle( (int) x, (int) y, (int) width, (int) height, (int) color );
}

static void sys_play_sound(uint64_t freq, uint64_t duration, uint64_t waitAfter) {
    playNoteSound((int) freq, (int) duration, (int) waitAfter);
}


static void sys_toggle_cursor() {
    toggleCursor();
}

static void sys_get_ticks(uint64_t ticks) {
    uint32_t * t = (uint32_t *) ticks;    
    *t = ticks_elapsed();
}