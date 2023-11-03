#include <stdint.h>
#include <font.h>
#include "videoDriver.h"
#include <lib.h>

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

void drawWhiteLine() {
    for(int i = 0; i < VBE_mode_info->width * (VBE_mode_info->bpp / 8); i++) {
        putPixel(0xffffff, i, 3);
        putPixel(0xffffff, i, 4);
        putPixel(0xffffff, i, 5);
    }
}

void drawRectangle(int x, int y, int width, int height, int color) {
	for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {
			putPixel(color,j, i);
		}
	}
}

int line = 1, column = 0;
int showCursor = 1;

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

void printChar(char c, int x, int y, Color color) {
	if (c == '\b') {
		if (x <= 0 && y <= 0)
			return;
		if (x > CHAR_WIDTH) {
			column -= 2;
			for (int i = y; i < y + CHAR_HEIGHT; i++) {
				for (int j = x - CHAR_WIDTH; j < x; j++) {
					putPixel(0, 0, 0, j, i);
				}
			}
		} else if (line > 0) {
			line--;
			column = MAX_COLUMNS - 2;
			int c = 0;
			for (int i = line * CHAR_HEIGHT; i < (line + 1) * CHAR_HEIGHT; i++) {
				for (int j = (column + 1) * CHAR_WIDTH; j < (MAX_COLUMNS + 2) * CHAR_WIDTH; j++) {
					putPixel(0, 0, 0, j, i);
				}
			}
			while (isSpaceEmpty(column * CHAR_WIDTH, line * CHAR_HEIGHT) && column >= 1) {
				column--;
				c++;
			}
		}
		return;
	} else if (c == '\t') {
		if (column + 4 < MAX_COLUMNS) {
			column += 4;
		} else {
			line++;
			column = 0;
		}
		return;
	}

	if (c < FIRST_CHAR || c > LAST_CHAR )
		return;

	const unsigned char * charMap = font[c-32];
	for (int i = 0; i < CHAR_HEIGHT; i++) {
		char mask = 0b1000000;
		for (int j = 0; j < CHAR_WIDTH; j++) {
			if (*charMap & mask)
				putPixel(color.r, color.g, color.b, x+j, y+i);
			mask >>= 1;
		}
		charMap++;
	}
}
