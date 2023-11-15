#include <stdint.h>
#include <font.h>
#include "videoDriver.h"
#include <lib.h>
//#include <colores.h>
struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));


typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

#define MAX_LINES VBE_mode_info->height / CHAR_HEIGHT
#define MAX_COLUMNS VBE_mode_info->width / CHAR_WIDTH - 1

uint32_t size = DEFAULT_FONT_SIZE;

void setFontSize(uint32_t new_font_size){
	size = new_font_size;
}

uint32_t getFontSize(){
	return size;
}

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}


void drawRectangle(int x, int y, uint32_t color) { //ver de pasarle ancho y largo
	for (int i = y+1; i < y+23; i++) {
		for (int j= x+1; j < x+31; j++) {
			putPixel(color,j, i);
		}
	}
}

void drawBoard(int x, int y, uint32_t color){ //Borrar o pasar parametros
	//Me hace lineas horizontales
	for(int i=0;i<768;i+=767)
		for(int j=0; j<1024;j++)
			putPixel(color,j,i);

	//Hago lineas verticales
	for(int i=0;i<1024;i+=1023)
		for(int j=0; j<768;j++)
			putPixel(color,i,j);
}

int line = 1, column = 0;
int showCursor = 1;
//Me dice si el pixel esta ocupado o no  
char getPixel(int x, int y) {
	char * videoPtr = (char *) ((uint64_t)VBE_mode_info->framebuffer);
	int offset = y * VBE_mode_info->pitch + x * (VBE_mode_info->bpp / 8);
	return videoPtr[offset];
}

char isSpaceEmpty(int x, int y) {
	for (int i = y; i < y + CHAR_HEIGHT; i++) {
		for (int j = x; j < x + CHAR_WIDTH; j++) {
			if (getPixel(j, i) != 0) {
				return 0;
			}
		}
	}
	return 1;
}

void printChar(char c, int x, int y, uint32_t color) {
    if (c == '\b') {
		if (x <= 0 && y <= 0)
			return;
        // Handle backspace
        if (x >= CHAR_WIDTH) {
            column -= 2; //Espacio que ocupa un caracter en pantalla
			//Borra los pixeles del caracter anterior
			for (int i = y; i < y + CHAR_HEIGHT; i++) {
				for (int j = x - CHAR_WIDTH; j < x; j++) {
					putPixel(0x000000, j, i);
				}
			}
        } else if (line > 0) {
            line--;
            column = MAX_COLUMNS - 2; //Revisar esto
			int c=0;
			//Borro por si escribi mas de una linea
			for (int i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
				for (int j = (column + 1) * CHAR_WIDTH; j < (MAX_COLUMNS + 2) * CHAR_WIDTH; j++) {
					putPixel(0x000000, j, i);
				}
			}
			while (isSpaceEmpty(column * CHAR_WIDTH, line * CHAR_HEIGHT) && column >= 1) {
				column--;
				c++;
			}
        }
    } else if (c == '\t') {
        // Handle tab
        int spaces = 4 - (column % 4); //Revisar pq dudoso
        for (int i = 0; i < spaces; i++) {
            if (x < VBE_mode_info->width) {
                printChar(' ', x, y, color);
                x += CHAR_WIDTH;
            }
        }
    } else {
        // Imprimo el caracter normal (REVISAR)
        if (c < FIRST_CHAR || c > LAST_CHAR)
            return;

        const unsigned char *charMap = font[c - 32];
        for (int i = 0; i < CHAR_HEIGHT; i++) {
            char mask = 0b1000000;
            for (int j = 0; j < CHAR_WIDTH; j++) {
                if (*charMap & mask) {
					//if(font==1){
                    putPixel(color, x + j, y + i);
					//}else{}
                }
                mask >>= 1; 
            }
            charMap++;
        }

        // Move the cursor (REVISA ESTO)
        x += CHAR_WIDTH;
        if (x >= VBE_mode_info->width) {
            x = 0;
            line++;
        }
    }
}


unsigned int strlen(char * str) {
    unsigned int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}

void printStringPlace(char * string, int x, int y, uint32_t color) {
	int i = 0;
	int oldColumn = column;
	int oldLine = line;
	column = x / CHAR_WIDTH;
	line = y / CHAR_HEIGHT;
	while (string[i] != 0) {
		printChar(string[i], x + i * CHAR_WIDTH, y, color);
		i++;
	}
	column = oldColumn;
	line = oldLine;
}
void moveCursor() {
    if (showCursor) {
        uint32_t cursorColor = 0xFFFFFF;  // Color blanco en hexa 
        for (int i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
            for (int j = (column + 1) * CHAR_WIDTH; j < (column + 2) * CHAR_WIDTH; j++) {
                putPixel(cursorColor, j, i);
            }
        }
    }
}

void eraseCursor() {
    if (showCursor) {
        uint32_t bgColor = 0x000000;  // Color negro en hexa
        for (uint64_t i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
            for (uint64_t j = (column + 1) * CHAR_WIDTH; j < (column + 2) * CHAR_WIDTH; j++) {
                putPixel(bgColor, j, i);
            }
        }
    }
}



void printString(char * string) {
	printStringN(string, strlen(string));
}

void printStringN(char * string, uint64_t length) {
	printStringNColor(string, length, (uint32_t)0xFF0000);
}

void printStringColor(char * string, uint32_t color) {
	printStringNColor(string, strlen(string), color);
}

void printStringNColor(char * string, uint64_t length, uint32_t color) {
	int i = 0;
	eraseCursor();
	while (string[i] != 0 && length > 0) {
		if (string[i] == '\n') {
			line++;
			column = 0;
		} else {
			column++;
			printChar(string[i], column * CHAR_WIDTH, line * CHAR_HEIGHT, color);
			if (column >= MAX_COLUMNS - 1) {
				line++;
				column = 0;
			}
		}
		if (line >= MAX_LINES) {
			moveOneLineUp();
		}
		i++;
		length--;
	}
	moveCursor();
}
//Imprimo linea
void printLn(char * string) {
	printString(string);
	line++;
	column = 0;
	if (line >= MAX_LINES) {
		moveOneLineUp();
	}
	moveCursor();
}

void moveOneLineUp() {
	char * dst = (char *) (uint64_t)(VBE_mode_info->framebuffer);
	char * src = dst + VBE_mode_info->pitch * CHAR_HEIGHT;
	memcpy(dst, src, VBE_mode_info->pitch * (VBE_mode_info->height - CHAR_HEIGHT));
	memset((void *) (uint64_t)(VBE_mode_info->framebuffer + VBE_mode_info->pitch * (VBE_mode_info->height - CHAR_HEIGHT)), 0, VBE_mode_info->pitch * CHAR_HEIGHT);
	line--;
}


void clearScreen() {
	memset((void *) (uint64_t)(VBE_mode_info->framebuffer), 0, VBE_mode_info->pitch * VBE_mode_info->height);
	line = 1;
	column = 0;
	moveCursor();
}

void changeit() {
	if (showCursor)
		eraseCursor();
	showCursor = !showCursor;
}

