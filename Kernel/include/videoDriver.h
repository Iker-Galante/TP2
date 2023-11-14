#ifndef _VIDEODRIVER_H
#define _VIDEODRIVER_H

#include <colores.h>
#include <stdint.h>

#define DEFAULT_FONT_SIZE 4

void setFontSize(uint32_t new_font_size);

uint32_t getFontSize();

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void drawWhiteLine();

void drawRectangle(int x, int y, int width, int height, uint32_t color);

char getPixel(int x, int y);

char isSpaceEmpty(int x, int y);

void printChar(char c, int x, int y, uint32_t color);

void printStringPlace(char * string, int x, int y, uint32_t color);

void printString(char * string);

void printStringN(char * string, uint64_t length);

void printLn(char * string);

void moveOneLineUp();

void printStringColor(char * string, uint32_t color);

void printStringNColor(char * buffer, uint32_t length, uint32_t color);

void moveCursor();

void eraseCursor();

void clearScreen();

uint16_t getHeight();

uint16_t getWidth();

void toggleCursor();

void drawImage(const unsigned long int * image, int width, int height);

#endif