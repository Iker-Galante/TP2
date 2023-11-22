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

uint8_t font = 1;
uint16_t line = 0, column = 0;
int showCursor = 1;
void increaseEscale(){
	if(font < 3 )
		font ++;
}
void decreaseEscale(){
	if(font > 1)
		font --;
}

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}


void drawRectangle(int x, int y,int width, int heigth, uint32_t color) { //ver de pasarle ancho y largo
	for (int i = y+1; i < y+heigth-1; i++) {
		for (int j= x+1; j < x+width-1; j++) {
			putPixel(color,j, i);
		}
	}
}

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
static uint32_t* getPixelPtr(uint16_t x, uint16_t y) {
    uint8_t pixelwidth = VBE_mode_info->bpp/8;     //la cantidad de bytes hasta el siguiente pixel a la derecha (bpp: BITS per px)
    uint16_t pixelHeight = VBE_mode_info->pitch;   //la cantidad de bytes hasta el pixel hacia abajo

    uintptr_t pixelPtr = (uintptr_t)(VBE_mode_info->framebuffer) + (x * pixelwidth) + (y * pixelHeight);
    return (uint32_t*)pixelPtr;
}
void setPixel(uint32_t color, int x, int y){
	    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return;

    uint32_t* pixel = getPixelPtr(x, y);
    *pixel = color;
}

void printChar(char c, int x, int y, uint32_t color) {
	int caracterx,caractery;
	if(column >= VBE_mode_info->width){
		column=0;
		if(line + CHAR_HEIGHT * font > VBE_mode_info->height){
			line -= CHAR_HEIGHT * font;
			moveOneLineUp();
		} else{
			line += CHAR_HEIGHT * font;
		}
	}
	    int mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    const unsigned char *glyph = IBM_VGA_8x16_glyph_bitmap + 16 * (c - 32);
        // Imprimo el caracter normal (REVISAR)
        if (c < FIRST_CHAR || c > LAST_CHAR)
            return;

        
        for (caractery = 0; caractery < CHAR_HEIGHT; caractery++) {
            
            for ( caracterx = 0; caracterx < CHAR_WIDTH; caracterx++) {
                
					for(int i=0;i<font;i++)
						for(int j=0;j<font;j++)
							setPixel(glyph[caractery] & mask[caracterx] ? color : 0x000000,column+(8-caracterx)*font+i,line+caractery*font+j);
                } 
            }
			column += CHAR_WIDTH * font;
        }

unsigned int strlen(char * str) {
    unsigned int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}

void moveCursor() {
    if (showCursor) {
        uint32_t cursorColor = 0xFFFFFF;  // Color blanco en hexa 
        for (int i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
            for (int j = (column + 1) * CHAR_WIDTH; j < (column + 2) * CHAR_WIDTH; j++) {
                setPixel(cursorColor, j, i);
            }
        }
    }
}

void eraseCursor() {
    if (showCursor) {
        uint32_t bgColor = 0x000000;  // Color negro en hexa
        for (uint64_t i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
            for (uint64_t j = (column + 1) * CHAR_WIDTH; j < (column + 2) * CHAR_WIDTH; j++) {
                setPixel(bgColor, j, i);
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
			column=0;
			line += CHAR_HEIGHT * font;
		} else if (string[i] == '\b'){
			if(column >= CHAR_WIDTH * font){
				column -= CHAR_WIDTH * font;
			} else{ column=0;}
		printChar(' ',column,line,0x000000);
		column -= CHAR_WIDTH * font;
		}else{	
			//column += CHAR_WIDTH * font; 
			printChar(string[i], column /* CHAR_WIDTH*/, line /* CHAR_HEIGHT*/, color);
			if (column + CHAR_WIDTH * font > VBE_mode_info->width) {
				line +=CHAR_HEIGHT * font ;
				column = 0;
			}
		}
		if(line + CHAR_HEIGHT * font > VBE_mode_info->height){
			line -= CHAR_HEIGHT * font;
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
	char * src = dst + VBE_mode_info->pitch * CHAR_HEIGHT * font;
	memcpy(dst, src, VBE_mode_info->pitch * (VBE_mode_info->height - CHAR_HEIGHT *font));
	memset((void *) (uint64_t)(VBE_mode_info->framebuffer + VBE_mode_info->pitch * (VBE_mode_info->height - CHAR_HEIGHT *font)), 0, VBE_mode_info->pitch * CHAR_HEIGHT *font);

}


void clearScreen() {
	memset((void *) (uint64_t)(VBE_mode_info->framebuffer), 0, VBE_mode_info->pitch * VBE_mode_info->height);
	line = 0;
	column = 0;
	moveCursor();
}

void changeit() {
	if (showCursor)
		eraseCursor();
	showCursor = !showCursor;
}
