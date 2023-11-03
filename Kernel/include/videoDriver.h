#ifndef _VIDEODRIVER_H
#define _VIDEODRIVER_H

#include <colors.h>
#include <stdint.h>

#define DEFAULT_FONT_SIZE 4

void setFontSize(uint32_t new_font_size);

uint32_t getFontSize();

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void drawWhiteLine();

void drawRectangle(int x, int y, int width, int height, int color);

char getPixel(int x, int y);

char isSpaceEmpty(int x, int y);

void printChar(char c, int x, int y, Color color);

void printStringPlace(char * string, int x, int y, Color color);

void printString(char * string);

void printStringN(char * string, uint64_t length);

void printLn(char * string);

void moveOneLineUp();

void printStringColor(char * string, Color color);

void printStringNColor(char * buffer, uint64_t length, Color color);

void moveCursor();

void eraseCursor();

void clearScreen();

uint16_t getHeight();

uint16_t getWidth();

void toggleCursor();

void drawImage(const unsigned long int * image, int width, int height);

#endif